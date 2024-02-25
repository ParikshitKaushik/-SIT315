#include <chrono>
#include <cstdlib>
#include <iostream>
#include <omp.h>

using namespace std::chrono;
using namespace std;

void quickSort(int arr[], int start, int end)
{
    if (start >= end)
        return;

    int pivot = arr[start];
    int count = 0;
    for (int i = start + 1; i <= end; i++)
    {
        if (arr[i] <= pivot)
            count++;
    }

    int pivotIndex = start + count;
    swap(arr[start], arr[pivotIndex]);

    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex)
    {
        while (arr[i] <= pivot)
        {
            i++;
        }
        while (arr[j] > pivot)
        {
            j--;
        }
        if (i < pivotIndex && j > pivotIndex)
        {
            swap(arr[i++], arr[j--]);
        }
    }

#pragma omp task
    quickSort(arr, start, pivotIndex - 1);

#pragma omp task
    quickSort(arr, pivotIndex + 1, end);
}

int main()
{
    const int size = 1000000;
    int arr[size];

    srand(time(0));
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % 100;
    }

    auto start = high_resolution_clock::now();

#pragma omp parallel
    {
#pragma omp single nowait
        quickSort(arr, 0, size - 1);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by quick sort: " << duration.count() << " microseconds" << endl;

    return 0;
}