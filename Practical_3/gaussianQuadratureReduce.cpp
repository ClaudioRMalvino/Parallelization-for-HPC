#include <cmath>
#include <iomanip>
#include <iostream>
#include <mpi.h>

double gaussianQuadrature(int startSteps, int endSteps, double globalSteps);

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const double gsteps{std::stod(argv[1])};
  double total{0.0};
  double globalTotal;
  const int start = 10 * rank / size;
  const int end = 10 * (rank + 1) / size;
  total = gaussianQuadrature( start, end, gsteps);
  MPI_Reduce(&total, &globalTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    std::cout << std::fixed << std::setprecision(10) << globalTotal << '\n';
  }

  MPI_Finalize();
  return 0;
}

double gaussianQuadrature(const int startSteps, const int endSteps, const double globalSteps) {
  double total = 0.0;

  for (int i = startSteps; i < endSteps; i++) {
    total += std::exp(
        -std::pow(10.0 * ((static_cast<double>(i) + 0.5) / (globalSteps + 1)), 2));
  }
  return total;
}
