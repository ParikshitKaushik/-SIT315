#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int N = 10;

// Function to generate random values for matrix
void generateRandomMatrix(int** matrix) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 100; // Generate random values between 0 and 99
        }
    }
}

// Function to perform matrix multiplication
void matrixMultiply(int** A, int** B, int** C) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0; // Initialize the element in C to 0
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j]; // Perform the multiplication
            }
        }
    }
}

int main() {
    // Dynamically allocate memory for matrices
    int** A = new int*[N];
    int** B = new int*[N];
    int** C = new int*[N];
    for (int i = 0; i < N; ++i) {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    // Generate random values for matrices A and B
    srand(time(0)); // Seed for random number generation
    generateRandomMatrix(A);
    generateRandomMatrix(B);

    // Perform matrix multiplication
    clock_t start_time = clock(); // Start timing
    matrixMultiply(A, B, C);
    clock_t end_time = clock(); // End timing

    // Calculate execution time in seconds
    double execution_time = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Deallocate memory for matrices
    for (int i = 0; i < N; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    // Print execution time
    cout << "Execution time (excluding initialization): " << execution_time << " seconds" << endl;

    return 0;
}
