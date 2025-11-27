#include <cmath>
#include <iomanip>
#include <iostream>
#include <mpi.h>

double gaussianQuadrature(double xVar, int steps);

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);

  double steps{1000.0};

  MPI_Finalize();
  return 0;
}

double gaussianQuadrature(double xVar, double steps) {
  double total = 0.0;

  for (int i = 0; i < steps; i++) {
    total += std::exp(
        -std::pow(10.0 * ((static_cast<double>(i) + 0.5) / (steps + 1)), 2));
  }
  return total;
}
