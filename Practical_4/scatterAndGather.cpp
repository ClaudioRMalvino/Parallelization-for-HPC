#include <algorithm>
#include <array>
#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    auto *recvbuf = static_cast<int *>(malloc(size * sizeof(int)));
    int partialSum{0};

    if (rank == 0) {
        int totalSum{0};
        std::vector<int> matrix(size * size);

        for (int i{0}; i < size; i++) {
            for (int j{0}; j < size; j++) {
                matrix[i * size + j] = i * size + j;
                std::cout << matrix[i * size + j] << " ";
            }
        }

        std::cout << "Matrix created by rank " << rank << std::endl;

        MPI_Scatter(matrix.data(), size,MPI_INT, recvbuf, size, MPI_INT, 0, MPI_COMM_WORLD);
        std::cout << "Rank 0 has scattered the matrix" << '\n';
        for (size_t i{0}; i < static_cast<unsigned int>(size); i++) {
            partialSum += recvbuf[i];
        }
        MPI_Gather(&partialSum, 1, MPI_INT, recvbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::cout << "Rank 0 has gathered " << '\n';

        for (size_t i{0}; i < static_cast<unsigned int>(size); i++) {
            totalSum += recvbuf[i];
        }
        std::cout << "Total sum " << totalSum << '\n';
    } else {
        MPI_Scatter(NULL, size,MPI_INT, recvbuf, size, MPI_INT, 0, MPI_COMM_WORLD);
        std::cout << "rank " << rank << " received scatter from rank 0" << std::endl;
        for (size_t i{0}; i < static_cast<unsigned int>(size); i++) {
            partialSum += recvbuf[i];
        }
        std::cout << "Rank " << rank << " has partial sum " << partialSum << std::endl;
        MPI_Gather(&partialSum, 1, MPI_INT, NULL, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
