#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <mpi.h>

using namespace std;

void multiplyMatrices(int **A, int **B, int **C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) { // Fixed the loop condition
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char **argv) {
    const int N = 4;

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int **A = new int *[N];
    int **B = new int *[N];
    int **C = new int *[N];
    for (int i = 0; i < N; ++i) {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    srand(time(0) + rank); // Add rank to ensure different seed for each process
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    clock_t startTime = clock();
    multiplyMatrices(A, B, C, N);
    clock_t endTime = clock();

    double executionTime = double(endTime - startTime) / CLOCKS_PER_SEC;

    if (rank == 0) {
        ofstream outputFile("output.txt");
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                outputFile << C[i][j] << " ";
            }
            outputFile << endl;
        }
        outputFile.close();

        cout << "Matrix multiplication executed in " << executionTime << " seconds." << endl;
    }

    for (int i = 0; i < N; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    MPI_Finalize();

    return 0;
}
