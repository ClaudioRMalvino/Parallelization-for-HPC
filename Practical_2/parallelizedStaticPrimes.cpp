#include "primes.h"
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

    long maxPrime{10000000};

    int localStart = static_cast<int>(maxPrime * rank / size);
    int localEnd = static_cast<int>(maxPrime * (rank + 1) / size);
    std::vector<int> primes;

    if (rank == (size - 1)) {
        localEnd += 1;
    }

    int primeNum{0};

    for (int i = localStart; i < localEnd; i++) {
        int primeNum = isPrime(i);
        if (primeNum == 0) {
            continue;
        } else {
            primes.push_back(primeNum);
        }
    }
    if (rank != 0) {
        MPI_Send(primes.data(), primes.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        int primesFound = primes.size();

        for (int process = 1; process < size; process++) {
            int incomingCount;
            MPI_Probe(process, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &incomingCount);
            std::vector<int> recvBuffer(incomingCount);
            MPI_Recv(recvBuffer.data(), incomingCount, MPI_INT, process, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            primesFound += incomingCount;
        }
        std::cout << "Total primes found: " << primesFound << std::endl;
    }

    MPI_Finalize();
    return 0;
}
