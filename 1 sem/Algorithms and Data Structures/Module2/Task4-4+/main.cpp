#include <iostream>
#include <vector>

template<class T>
int partition(std::vector<T> &arr, int start, int finish) {
    //в нашем оставшемся отрезке меняем число на рандомной позиции с числом в начале
    std::swap(arr[start + rand() % (finish - start)], arr[start]);
    int i = finish - 1, j = finish - 1; // ставим индекс последнего элемента в индексы опорного(i) и меняющегося элемента(j)
    while (j != start) {
        if (arr[j] >= arr[start]) {
            std::swap(arr[i], arr[j]);
            i--;
        }
        j--;
    }
    std::swap(arr[start], arr[i]);
    return i;

}

template<class T>
int GetK(std::vector<T> &arr, int k) {
    int start = 0, finish = arr.size();
    bool check = true;
    while (check) {
        int pos = partition(arr, start, finish); //возвращает индекс опорного элемента
        if (pos < k) {
            start = pos + 1; //оставляем непроверенную область
        } else if (pos > k) {
            finish = pos; //оставляем непроверенную область
        } else {
            return arr[k];
        }
    }
}

int main() {
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<int> arr;
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        arr.push_back(x);
    }

    std::cout << GetK(arr, k) << std::endl;
}