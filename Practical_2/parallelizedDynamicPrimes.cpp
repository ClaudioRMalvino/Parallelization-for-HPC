#include <cmath>
#include <iostream>
#include <mpi.h>
#include <vector>

// Forward function declaration
int isPrime(long num);

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Status status;
  int primesFound{0};
  std::vector<int> primes;

  if (rank == 0) {
    int maxPrime{10000000};

    for (int process = 1; process < size; process++) {
      MPI_Send()
    }
  }

  if (rank != 0) {
  }
  MPI_Finalize();

  return 0;
}

int isPrime(long num) {

  long sqNum = std::sqrt(num);
  if (num == 1) {
    return 0;
  }
  if (num == 2) {
    return num;
  }
  if (num % 2 == 0) {
    return 0;
  }

  for (long i = 3; i <= sqNum; i += 2) {
    if (num % i == 0) {
      return 0;
    }
  }
  return num;
}
