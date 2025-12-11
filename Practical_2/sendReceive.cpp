#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    int v;

    if (rank == 0) {
        v = 0;
        MPI_Send(&v, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == size - 1) {
        MPI_Recv(&v, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        std::cout << "Status of rank " << rank << ": " << '\n';
        v += rank;
        std::cout << "v = " << v << std::endl;

        MPI_Send(&v, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&v, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        std::cout << "Status of rank " << rank << ": " << '\n';
        v += rank;
        std::cout << "v = " << v << std::endl;

        MPI_Send(&v, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        MPI_Recv(&v, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
        int result{(size * (size - 1)) / 2};
        if (v == result) {
            std::cout << "The code is correct. V = " << v << std::endl;
        } else {
            std::cout << "The code is inccorrect. V = " << v
                    << " and result = " << result << std::endl;
        }
    }
    MPI_Finalize();
    return 0;
}
