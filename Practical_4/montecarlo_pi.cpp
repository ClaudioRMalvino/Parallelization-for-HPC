#include<iostream>
#include<iomanip>
#include<cstdlib>
#include <cmath>
#include <mpi.h>

int main(int argc, char **argv) {

  MPI_Init(&argc, &argv);
  int rank, size;
  long double globalResult{0.0};
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::srand(rank);

  std::cout << "Rank " << rank << " initialized MPI" << std::endl;

  long int hit = 0;
  long double x, y;

  const long int count = std::atol(argv[1]);

  std::cout << "Rank " << rank << " will generate " << count << " points" << std::endl;

  for(long int i{}; i < count; i++){
    x = std::rand() / static_cast<long double>(RAND_MAX);
    y = std::rand() / static_cast<long double>(RAND_MAX);
    if (x*x + y*y < 1.0)
    {
      hit++;
    }
  }

  const long double countD {static_cast<long double>(count)};
  const long double hitD {static_cast<long double>(hit)};
  const long double result {(hitD / countD)};
  if (rank == 0) {
    MPI_Reduce(&result, &globalResult, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    std::cout << std::fixed << std::setprecision(16);
    std::cout << "Ratio: " << globalResult / 4.0 << std::endl;
    std::cout << "Pi/4 = " << static_cast<long double>(M_PI / 4.0)
              << "\tError = " << static_cast<long double>(M_PI / 4.0) - (globalResult/4.0) << std::endl;
  } else {
    MPI_Reduce(&result, NULL, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}
