#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include "quadrature.h"


int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const double gsteps{std::stod(argv[1])};
    double total{0.0};
    const int start = static_cast<int>(gsteps) * rank / size;
    const int end = static_cast<int>(gsteps) * (rank + 1) / size;

    total = gaussianQuadrature(start, end, gsteps);

    if (rank == 0) {
        double rbuffer[size];
        double globalTotal{total};

        for (int i = 1; i < size; i++) {
            MPI_Recv(&rbuffer, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (double partial : rbuffer) {
            globalTotal += partial;
        }

        std::cout << std::fixed << std::setprecision(10) << globalTotal << '\n';

    } else {
        MPI_Send(&total, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        std::cout << rank << " sent out packet" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
