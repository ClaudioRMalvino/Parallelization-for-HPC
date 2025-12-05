#include "quadrature.h"
#include <cmath>

double gaussianQuadrature(const int startSteps, const int endSteps, const double globalSteps) {
    double total = 0.0;
    double dx = 10.0 / globalSteps;

    for (int i = startSteps; i < endSteps; i++) {
        total += std::exp(
            -std::pow(10.0 * ((static_cast<double>(i) + 0.5) / (globalSteps + 1.0)), 2.0));
    }
    return total * dx;
}
