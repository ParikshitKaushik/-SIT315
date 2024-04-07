#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

int partition(int arr[], int start, int end) {
    // Random pivot selection
    int pivotIndex = start + rand() % (end - start + 1);
    int pivot = arr[pivotIndex];
    
    // Swap pivot with the first element
    swap(arr[start], arr[pivotIndex]);
    
    int i = start + 1;
    int j = end;

    while (i <= j) {
        while (i <= j && arr[i] <= pivot)
            i++;
        while (i <= j && arr[j] > pivot)
            j--;
        if (i < j)
            swap(arr[i], arr[j]);
    }

    swap(arr[start], arr[j]);
    return j;
}

void quickSort(int arr[], int start, int end) {
    if (start >= end)
        return;

    // Use insertion sort for small arrays
    if (end - start + 1 <= 10) {
        for (int i = start + 1; i <= end; ++i) {
            int key = arr[i];
            int j = i - 1;
            while (j >= start && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }

    int p = partition(arr, start, end);
    quickSort(arr, start, p - 1);
    quickSort(arr, p + 1, end);
}

int main() {
    const int n = 1000000; // Adjust array size as needed
    int *arr = new int[n];

    srand(time(0));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100;

    auto start = high_resolution_clock::now();

    quickSort(arr, 0, n - 1);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by quick sort: " << duration.count() << " microseconds" << endl;

    delete[] arr;
    return 0;
}
