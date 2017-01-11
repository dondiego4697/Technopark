/* 3_1.
 * Дан отсортированный массив целых чисел A[0..n­1] и массив целых чисел B[0..m­1]. 
 * Для каждого  элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, 
 * равного или  превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. 
 * Время работы поиска k для каждого  элемента B[i]: O(log(k)). 
   n, m ≤ 10000.  */
#include <iostream>
#include <assert.h>

using namespace std;

int FindInsertionPoint(int *arr, int first, int last, int element) {
    if ((last - first) == 1)
        return element <= arr[first] ? first : last;
    int mid = (first + last) / 2;
    if (element <= arr[mid])
        return FindInsertionPoint(arr, first, mid, element);
    else
        return FindInsertionPoint(arr, mid, last, element);
}

void find(int *A, int *B, int *C, int n, int m) {
    int i = 0;
    while (i != m) {
        if (B[i] <= A[n - 1]) {
            int k = 1, prevK = 0;
            while (k < n) {
                if (A[prevK] <= B[i] && A[k] >= B[i]) {
                    break;
                } else if (B[i] <= A[prevK]) {
                    break;
                } else {
                    prevK = k;
                    k *= 2;
                    if(k > n){
                        k = n;
                    }
                }
            }

            C[i] = FindInsertionPoint(A, prevK, k, B[i]);
        } else {
            C[i] = n;
        }
        ++i;
    }
}


int main() {
    int n = 0, m = 0;
    cin >> n >> m;
    assert(n > 0 && m > 0 && n <= 10000 && m <= 10000);

    //два основных массива
    int *A = new int[n];
    int *B = new int[m];

    //массив индексов
    int *C = new int[m];

    //заполняем из, ввод
    for (int i = 0; i < n; ++i) {
        cin >> A[i];
    }
    for (int i = 0; i < m; ++i) {
        cin >> B[i];
    }

    find(A, B, C, n, m);

    //вывод
    for (int i = 0; i < m; ++i) {
        cout << C[i] << " ";
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}