#!/usr/bin/bash
mpicxx gaussianQuadrature.cpp gaussianQuadratureReduce.cpp -o gaussianQuadratureReduce -O2

for ((i = 2; i < 20 ; i += 2)); do
  mpiexec -n $i gaussianQuadratureReduce 200000
done
