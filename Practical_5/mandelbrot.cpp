#include <complex>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>

static std::vector<std::vector<int>> colors = {
    {68, 1, 84},
    {32, 164, 134},
    {68, 1, 84},
    {57, 86, 140},
    {31, 150, 139},
    {115, 208, 85},
    {253, 231, 37},
    {42, 120, 142},
    {255, 255, 255}
};

int mandel(std::complex<double> z0, int iters) {
    std::complex<double> z = z0;
    int i;
    for (i = 1; i < iters; i++) {
        z = z * z + z0;
        if (std::abs(z) > 2.0) break;
    }
    return i;
}

int getColor(int iter, int totaliters, int index) {
    int no_gradients = static_cast<int>(colors.size()) - 1;
    double var = static_cast<double>(no_gradients) *
                 (1.0 - static_cast<double>(iter) / static_cast<double>(totaliters));
    int gr = static_cast<int>(var);
    if (gr > no_gradients - 1) gr = no_gradients - 1;
    if (gr < 0) gr = 0;

    return colors[gr][index] +
           static_cast<int>(std::round((colors[gr + 1][index] - colors[gr][index]) *
                                       (var - static_cast<double>(gr))));
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int res, iters;
    double xmin, xmax, ymin, ymax;

    if (rank == 0) {
        std::cout << "Enter resolution\n";
        std::cin >> res;

        do {
            std::cout << "Enter xmin\n"; std::cin >> xmin;
            std::cout << "Enter xmax\n"; std::cin >> xmax;
            std::cout << "Enter ymin\n"; std::cin >> ymin;
            std::cout << "Enter ymax\n"; std::cin >> ymax;

            if (xmin >= xmax || ymin >= ymax) {
                std::cout << "Invalid domain boundaries.\n";
            }
        } while (xmin >= xmax || ymin >= ymax);

        std::cout << "Enter iterations\n";
        std::cin >> iters;
    }

    MPI_Bcast(&res,   1, MPI_INT,    0, MPI_COMM_WORLD);
    MPI_Bcast(&iters, 1, MPI_INT,    0, MPI_COMM_WORLD);
    MPI_Bcast(&xmin,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&xmax,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ymin,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ymax,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Row block decomposition
    const int start_row = (res * rank) / size;
    const int end_row   = (res * (rank + 1)) / size;
    const int local_rows = end_row - start_row;

    const double dx = (xmax - xmin) / static_cast<double>(res);
    const double dy = (ymax - ymin) / static_cast<double>(res);

    // Local RGB buffer: local_rows rows, each row has res pixels, 3 bytes per pixel
    std::vector<unsigned char> local_rgb(static_cast<size_t>(local_rows) * 3u * static_cast<size_t>(res));

    // Compute assigned rows
    for (int i = start_row; i < end_row; i++) {
        const int li = i - start_row;
        for (int j = 0; j < res; j++) {
            std::complex<double> z;
            z.real(xmin + j * dx);
            z.imag(ymax - i * dy);

            int m = mandel(z, iters);

            // write RGB
            const size_t base = (static_cast<size_t>(li) * static_cast<size_t>(res) + static_cast<size_t>(j)) * 3u;
            local_rgb[base + 0] = static_cast<unsigned char>(getColor(m, iters, 0));
            local_rgb[base + 1] = static_cast<unsigned char>(getColor(m, iters, 1));
            local_rgb[base + 2] = static_cast<unsigned char>(getColor(m, iters, 2));
        }
    }

    // Prepare gather counts/displacements on rank 0
    std::vector<int> recvcounts, displs;
    std::vector<unsigned char> full_rgb;

    if (rank == 0) {
        recvcounts.resize(size);
        displs.resize(size);

        int offset = 0;
        for (int r = 0; r < size; r++) {
            int start = (res * r) / size;
            int end = (res * (r + 1)) / size;
            int rows = end - start;
            recvcounts[r] = rows * 3 * res;  // bytes
            displs[r] = offset;
            offset += recvcounts[r];
        }

        full_rgb.resize(static_cast<size_t>(3) * static_cast<size_t>(res) * static_cast<size_t>(res));
    }

    // Gather all RGB data onto rank 0
    MPI_Gatherv(local_rgb.data(),
                local_rows * 3 * res,
                MPI_UNSIGNED_CHAR,
                rank == 0 ? full_rgb.data() : nullptr,
                rank == 0 ? recvcounts.data() : nullptr,
                rank == 0 ? displs.data() : nullptr,
                MPI_UNSIGNED_CHAR,
                0,
                MPI_COMM_WORLD);

    if (rank == 0) {
        std::ofstream img("mandel.ppm", std::ios::binary);
        img << "P6\n" << res << " " << res << "\n255\n";
        img.write(reinterpret_cast<const char*>(full_rgb.data()),
                  static_cast<std::streamsize>(full_rgb.size()));
        img.close();
    }

    MPI_Finalize();
    return 0;
}