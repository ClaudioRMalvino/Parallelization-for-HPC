#include "primes.h"
#include <cmath>

int isPrime(const int num) {
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
