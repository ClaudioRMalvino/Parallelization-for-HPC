#include <iostream>
#include <mpi.h>
#include <vector>

int main() {
    MPI_Init(NULL, NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> temp(size);
    int recvBuf;
    const auto sizeT = static_cast<size_t>(size);

    if (rank == 0) {
        // Construct an NxN matrix where N = numProc = size
        std::vector<int> matrix(size * size);

        for (size_t i{}; i < sizeT; i++) {
            for (size_t j{}; j < sizeT; j++) {
                matrix[i * sizeT + j] = static_cast<int>(i) * size + static_cast<int>(j);
            }
        }

        std::cout << "Original matrix is:" << '\n';
        for (size_t i{}; i < sizeT; i++) {
            for (size_t j{}; j < sizeT; j++) {
                std::cout << matrix[i * sizeT + j] << " ";
            }
            std::cout << '\n';
        }
        // Scatter size one N times, shifting the scattering window by size after each scatter
        // appends the
        for (size_t i{}; i < sizeT; i++) {
            MPI_Scatter(
                matrix.data() + (sizeT * i),
                1,
                MPI_INT,
                &recvBuf,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD);
            temp[i] = recvBuf;
        }
        std::cout << "Rank 0 has scattered" << std::endl;

        // Overwrite the original matrix with its transpose
        MPI_Gather(
            temp.data(),
            size,
            MPI_INT,
            matrix.data(),
            size,
            MPI_INT,
            0,
            MPI_COMM_WORLD);

        std::cout << "Rank 0 has gathered" << '\n';
        std::cout << "Transpose of the matrix is:" << '\n';

        for (size_t i{}; i < sizeT; i++) {
            for (size_t j{}; j < sizeT; j++) {
                std::cout << matrix[i * sizeT + j] << " ";
            }
            std::cout << '\n';
        }
    } else {
        for (size_t i{}; i < sizeT; i++) {
            MPI_Scatter(
                NULL,
                0,
                MPI_INT,
                &recvBuf,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD);
            temp[i] = recvBuf;
        }
        std::cout << "Rank " << rank << " has received scatter" << std::endl;

        MPI_Gather(
            temp.data(),
            size,
            MPI_INT,
            NULL,
            0,
            MPI_INT,
            0,
            MPI_COMM_WORLD);

        std::cout << "Rank " << rank << " has sent gather" << '\n';
    }

    MPI_Finalize();
    return 0;
}
