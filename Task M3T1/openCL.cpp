#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <CL/cl.h>

using namespace std;

void multiplyMatrices(int *A, int *B, int *C, int N, cl_context context, cl_command_queue queue, cl_kernel kernel) {
    cl_int err;
    size_t global_work_size[2] = {N, N};

    // Create OpenCL buffers
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * N * N, A, &err);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * N * N, B, &err);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * N * N, NULL, &err);

    // Set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    err |= clSetKernelArg(kernel, 3, sizeof(int), &N);

    // Execute the kernel
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);

    // Read the result back to host memory
    err = clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, sizeof(int) * N * N, C, 0, NULL, NULL);

    // Clean up
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
}

int main() {
    const int N = 4;
    int A[N][N], B[N][N], C[N][N];

    srand(time(0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    // OpenCL initialization
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueue(context, device, 0, NULL);

    const char *source = "__kernel void matrixMul(__global int *A, __global int *B, __global int *C, int N) { \
                            int row = get_global_id(0); \
                            int col = get_global_id(1); \
                            int sum = 0; \
                            for (int k = 0; k < N; ++k) { \
                                sum += A[row * N + k] * B[k * N + col]; \
                            } \
                            C[row * N + col] = sum; \
                        }";

    program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "matrixMul", NULL);

    clock_t startTime = clock();
    multiplyMatrices((int *)A, (int *)B, (int *)C, N, context, queue, kernel);
    clock_t endTime = clock();

    double executionTime = double(endTime - startTime) / CLOCKS_PER_SEC;

    // Output and cleanup
    ofstream outputFile("output.txt");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            outputFile << C[i][j] << " ";
        }
        outputFile << endl;
    }
    outputFile.close();

    cout << "Matrix multiplication executed in " << executionTime << " seconds." << endl;

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
