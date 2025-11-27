#!/bin/bash
mpicxx parallelizedDynamicPrimes.cpp -o parallelizedDynamicPrimes

for i in 5000 10000 15000 20000 40000 80000 100000 150000 200000 250000 300000 350000 400000 450000 500000; do
  echo "Running with step size: $i"
  mpiexec -n 4 parallelizedDynamicPrimes $i 
done
