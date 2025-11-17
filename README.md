# Parallelization for HPC

This repository contains practical exercises for the "Parallelization for HPC" course. The projects focus on implementing parallelization techniques for scientific computing using the Message Passing Interface (MPI) standard.

## Course Overview

Parallelisation of algorithms and software is a vital part of ensuring that scientific codes run quickly on modern computer hardware. This course will give an overview of parallelisation approaches within modern scientific computing. Some general aspects of parallelisation will be covered, including theoretical performance, hardware, and potential parallelisation errors. Further, the MPI parallelisation standard will be introduced, and a range of features, sufficient for writing a wide range of scientific applications, will be taught. 

Students will leave the course knowing about modern parallel architectures, the advantages and disadvantages of various parallelisation techniques, and a good understanding of MPI as used for parallelising common scientific computing algorithms. 

Topics covered will include: 
* Parallelism software frameworks and differences between them.
* Distributed vs Shared memory architectures.
* Parallel scaling: Amdahl’s Law, Gustafson’s Law, bandwidth, and latency.
* Evaluating parallel scaling: Strong and weak scaling.
* MPI Point-to-Point: Blocking, Non-blocking, Sending modes
* MPI Collective communication: Synchronisation, Reduction, Data distribution 
* MPI Deadlocks 
* MPI Error Handling 

*(Note: Course description from the official course syllabus.)*
`[Link to official course description](https://mphil.csc.cam.ac.uk/parallelisation-for-high-performance-computing-m14/)`

---

## Practicals

The exercises in this repository implement core MPI concepts on a distributed-memory model.

### Practical 1: MPI Environment Setup

* **Files:** `Practical_1/helloworld.cpp`, `Practical_1/version.cpp`
* **Concepts:** Demonstrates the fundamental setup for any MPI program:
    * `MPI_Init`: Initializing the MPI environment.
    * `MPI_Comm_size`: Getting the total number of processes (`size`).
    * `MPI_Comm_rank`: Getting the unique ID (`rank`) for the current process.
    * `MPI_Finalize`: Shutting down the MPI environment.
    * `MPI_Get_version`: Querying the system's MPI library version.

### Practical 2: Point-to-Point Communication

* **File:** `Practical_2/sendReceive.cpp`
* **Concepts:** Implements a "ring" communication pattern to practice with blocking point-to-point operations, a key topic from the course.
    * `MPI_Send`: Sending data from one process to another.
    * `MPI_Recv`: Blocking to wait for and receive data from another process.
    * `MPI_Status`: Inspecting the status of a received message.
    * This program creates a chain where `rank 0` sends a value, each process adds its rank, and the final result is sent back to `rank 0` for verification.

### Practical 4: Collective Communication

* **File:** `Practical_4/scatterAndGather.cpp`
* **Concepts:** This exercise explores collective data distribution, another core topic.
    * **`MPI_Scatterv`**: Setting up the parameters to scatter (distribute) portions of a matrix from the root process (`rank 0`) to all other processes. (File is a work-in-progress demonstrating this setup).

---

## How to Build and Run

### Prerequisites

You must have an MPI implementation (like [OpenMPI](https://www.open-mpi.org/) or [MPICH](https://www.mpich.org/)) installed on your system.

### Compilation

MPI C++ programs are compiled using the `mpic++` wrapper, which automatically links the necessary MPI libraries.

```bash
# Example for Practical 1
mpic++ Practical_1/helloworld.cpp -o helloworld 

# Example for Practical 2
mpic++ Practical_2/sendReceive.cpp -o sendReceive 
```

### Execution

MPI programs are executed using `mpiexec` (or `mpirun`), which launches the specified number of parallel processes.

```bash
# Example: Run 'helloworld' with 4 parallel processes
mpiexec -n 4 ./helloworld

# Example Output:
# Hello parallel world, I am process 1 out of 4
# Hello parallel world, I am process 3 out of 4
# Hello parallel world, I am process 0 out of 4
# Hello parallel world, I am process 2 out of 4

# Example: Run 'sendReceive' with 8 parallel processes
mpiexec -n 8 ./sendReceive

# Example Output:
# Status of rank 1: 
# v = 1
# Status of rank 2: 
# v = 3
# Status of rank 3: 
# v = 6
# Status of rank 4: 
# v = 10
# Status of rank 5: 
# v = 15
# Status of rank 6: 
# v = 21
# Status of rank 7: 
# v = 28
# The code is correct. V = 28
```
