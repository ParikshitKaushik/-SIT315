#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Function to partition the array
int partition(int* array, int low, int high) {
    int pivot = array[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (array[j] < pivot) {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;
    return (i + 1);
}

// The main function that implements QuickSort
void quickSort(int* array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

// A utility function to print the array
void printArray(int* arr, int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 20; // Size of the array
    int* data = NULL;
    if (rank == 0) {
        data = (int*)malloc(n * sizeof(int));
        // Initialize the array with random values
        for (int i = 0; i < n; i++) {
            data[i] = rand() % 100;
        }
        printf("Initial array: \n");
        printArray(data, n);
    }

    double start_time = MPI_Wtime();

    // Quick sort the array here at the root
    if (rank == 0) {
        quickSort(data, 0, n - 1);
        printf("Sorted array: \n");
        printArray(data, n);
    }

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Execution time: %f seconds\n", end_time - start_time);
    }

    if (data) {
        free(data);
    }

    MPI_Finalize();
    return 0;
}
