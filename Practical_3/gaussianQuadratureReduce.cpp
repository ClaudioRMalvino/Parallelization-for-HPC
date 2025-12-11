#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include "quadrature.h"

double percentError(const double trueVal, const double expVal);

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double startTime = MPI_Wtime();

    const double gsteps{std::stod(argv[1])};
    double total{0.0};
    double globalTotal;
    const int start = static_cast<int>(gsteps) * rank / size;
    const int end = static_cast<int>(gsteps) * (rank + 1) / size;

    total = gaussianQuadrature(start, end, gsteps);

    MPI_Reduce(&total, &globalTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double endTime = MPI_Wtime();
    double elapsedTime = endTime - startTime;

    if (rank == 0) {
        double trueVal{0.8862269255};

        std::ofstream runTimeFile("runTimevsProcessors.txt", std::ios::app);
        std::ofstream resultFile("resultvsProcessors.txt", std::ios::app);
        runTimeFile << size << " " << elapsedTime << '\n';
        resultFile << size << " " << percentError(trueVal, globalTotal) << '\n';
        std::cout << std::fixed << std::setprecision(10) << globalTotal << '\n';
    }

    MPI_Finalize();
    return 0;
}

double percentError(const double trueVal, const double expVal) {
    return std::abs(trueVal - expVal) / trueVal * 100;
}
