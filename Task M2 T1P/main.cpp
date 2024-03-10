#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

void multiplyMatrices(int **A,int **B,int **C, int N){
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            C[i][j] = 0;
            for (int k = 0; i < N; ++k){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(){
    const int N = 4
    
    int **A = new int*[N];
    int **B = new int*[N];
    int **C = new int*[N];
    for (int i = 0; i < N; ++i){
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }
    
    srand(time(0));
    for (int i  =  0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }
    
    
    clock_t startTime = clock();
    muultiplyMatrices(A,B,C,N);
    clock_t endTime = clock();
    
    double executionTime = double(endTime - startTime) / CLOCK_PER_SEC;
    
    ofstream outputFile("output.txt");
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            outputFile << C[i][j] << " ";
        }
        outputFile << endl;
    }
    outputFile.close();
    
    cout << "Matrix multiplication executed in " << executionTime << " seconds." << endl;
    
    for (int i = 0; i < N; ++i){
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;
    
    return 0;
}