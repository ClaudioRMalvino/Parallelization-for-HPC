import matplotlib.pyplot as plt
import numpy as np
def plot_run_time_vs_processes(num_processes, run_time):
    plt.plot(num_processes, run_time)
    plt.title("Run Time vs Processes")
    plt.xlabel("Processes")
    plt.ylabel("Run Time, [s]")
    plt.show()

def plot_accuracy_vs_processes(num_processes, results):
    plt.plot(num_processes, results)
    plt.title("Accuracy vs Processes")
    plt.xlabel("Processes")
    plt.ylabel("Percent Error")
    plt.show()

def main():
    run_time_data = np.loadtxt("runTimevsProcessors.txt", dtype=float, delimiter=" ")
    accuracy_data = np.loadtxt("resultvsProcessors.txt", dtype=float, delimiter=" ")
    num_processes = run_time_data[:,0]
    run_time = run_time_data[:,1]
    accuracy = accuracy_data[:,1]

    plot_run_time_vs_processes(num_processes, run_time)
    plot_accuracy_vs_processes(num_processes, accuracy)
    plt.show()

if __name__ == "__main__":
    main()