/* ​1_6. ​
 * Дан массив целых чисел A[0..n). 
 * Не используя других массивов переставить элементы массива A  в  обратном порядке за O(n). 
 * n ≤ 10000.  */

#include <iostream>
#include <assert.h>

using namespace std;

//в массиве, идя по циклу, меняю местами последний и первый элементы
void change(int *array, int n){
    int buff = 0;
    for (int i = 0; i < n / 2; ++i) {
        buff = array[i];
        array[i] = array[n - i - 1];
        array[n - i - 1] = buff;
    }
}

int main() {
    int n = 0;
    cin >> n;
    assert(n > 0 && n <=10000);

    //ввод
    int *array = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> array[i];
    }

    change(array, n);

    //вывод
    for (int i = 0; i < n; ++i) {
        cout << array[i] << " ";
    }

    delete[] array;
    return 0;
}