#include <cmath>
#include <fstream>
#include <iostream>
#include <mpi.h>

// Forward function declaration
int isPrime(int num);

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Status status;
  int maxPrime{10000000};
  int startEnd[2];
  int step{std::stoi(argv[1])};
  int totalPrimes{0};
  bool working{true};

  if (rank == 0) {
    double startTime = MPI_Wtime();
    int primesFound;
    startEnd[0] = 0;
    startEnd[1] = step;
    int i = step * (size - 1);
    int start = i;
    int end = start + step;
    std::ofstream dataFile("runTimevsSteps.dat", std::ios::app);

    for (int j = 1; j < size; j++) {
      MPI_Send(&startEnd, 2, MPI_INT, j, 0, MPI_COMM_WORLD);
      startEnd[0] = startEnd[1];
      startEnd[1] = (j + 1) * step;
    }
    while (i < maxPrime) {
      MPI_Recv(&primesFound, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
               &status);
      totalPrimes += primesFound;
      int finishedRank = status.MPI_SOURCE;
      startEnd[0] = start;
      startEnd[1] = end;
      MPI_Send(&startEnd, 2, MPI_INT, finishedRank, 1, MPI_COMM_WORLD);
      start = end;
      i += step;
      end += step;
    }
    for (int k = 1; k < size; k++) {
      int stopWorking{1};
      MPI_Send(&stopWorking, 1, MPI_INT, k, 2, MPI_COMM_WORLD);
      MPI_Recv(&primesFound, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
               &status);
      totalPrimes += primesFound;
    }
    std::cout << "Total primes found: " << totalPrimes << std::endl;

    double endTime = MPI_Wtime();
    double elapsedTime = endTime - startTime;
    std::cout << "Run time = " << elapsedTime << " sec" << std::endl;

    dataFile << step << " " << elapsedTime << '\n';
    dataFile.close();

  } else {
    while (working) {
      int primesFound;
      MPI_Recv(&startEnd, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if (status.MPI_TAG == 2) {
        MPI_Send(&primesFound, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        break;
      } else {
        int primesFound = 0;
        for (int i = startEnd[0]; i < startEnd[1]; i++) {
          int primeNum = isPrime(i);
          if (primeNum == 0) {
            continue;
          } else {
            primesFound += 1;
          }
        }
        MPI_Send(&primesFound, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
    }
  }

  MPI_Finalize();

  return 0;
}

int isPrime(int num) {

  int sqNum = std::sqrt(num);
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
