#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;
using namespace chrono;

const int N = 3; // 3x3 matrix for clarity

// Sample matrices
vector<vector<int>> A = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

vector<vector<int>> B = {
    {9, 8, 7},
    {6, 5, 4},
    {3, 2, 1}
};

vector<vector<int>> C_single(N, vector<int>(N, 0));
vector<vector<int>> C_multi(N, vector<int>(N, 0));

// Print a matrix
void printMatrix(const vector<vector<int>>& mat, string name) {
    cout << name << ":\n";
    for (int i = 0; i < N; ++i) {
        cout << "[ ";
        for (int j = 0; j < N; ++j) {
            cout << mat[i][j] << " ";
        }
        cout << "]\n";
    }
    cout << endl;
}

// Element-wise print of how result was computed
void printComputationDetails(const vector<vector<int>>& result, string title) {
    cout << title << " Computation Steps:\n";
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            cout << "C[" << i << "][" << j << "] = ";
            for (int k = 0; k < N; ++k) {
                cout << A[i][k] << "*" << B[k][j];
                if (k < N - 1) cout << " + ";
            }
            cout << " = " << result[i][j] << endl;
        }
    cout << endl;
}

// Single-threaded multiplication
void singleThreadMultiply() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                C_single[i][j] += A[i][k] * B[k][j];
}

// Thread function for each row
void multiplyRow(int row) {
    for (int j = 0; j < N; ++j)
        for (int k = 0; k < N; ++k)
            C_multi[row][j] += A[row][k] * B[k][j];
}

void multiThreadMultiply() {
    vector<thread> threads;
    for (int i = 0; i < N; ++i)
        threads.push_back(thread(multiplyRow, i));
    for (auto &t : threads)
        t.join();
}

int main() {
    cout << "========== MATRIX MULTIPLICATION ==========\n\n";

    printMatrix(A, "Matrix A");
    printMatrix(B, "Matrix B");

    // Single-threaded
    auto start1 = high_resolution_clock::now();
    singleThreadMultiply();
    auto end1 = high_resolution_clock::now();
    auto time1 = duration_cast<microseconds>(end1 - start1);

    printMatrix(C_single, "Matrix C (Single-threaded)");
    printComputationDetails(C_single, "Single-threaded");

    // Multithreaded
    auto start2 = high_resolution_clock::now();
    multiThreadMultiply();
    auto end2 = high_resolution_clock::now();
    auto time2 = duration_cast<microseconds>(end2 - start2);

    printMatrix(C_multi, "Matrix C (Multithreaded)");
    printComputationDetails(C_multi, "Multithreaded");

    // Comparison
    cout << "Execution Time:\n";
    cout << "Single-threaded:  " << time1.count() << " microseconds\n";
    cout << "Multithreaded:    " << time2.count() << " microseconds\n";

    if (time2 < time1)
        cout << "✅ Multithreaded approach is faster.\n";
    else
        cout << "ℹ️ Multithreaded approach is not faster for small matrices.\n";

    return 0;
}
