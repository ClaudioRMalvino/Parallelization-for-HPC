#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {

    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        matrix[i][j] = i + 1;
      }
    }
  }
  double *recbuf = (double *)malloc(size * sizeof(double));

  MPI_Scatterv()
}
