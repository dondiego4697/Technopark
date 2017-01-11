/* 1_4. Строки.
 * Напишите программу, печатающую набор строк в лексикографическом порядке.
 * Строки разделяются символом перевода строки '\n'. Если последний символ в потоке ввода '\n',
 * считать, что после него нет пустой строки. Максимальная длина строки 255 символов.
 * Написать свою функцию сравнения строк.
 */
#include <iostream>
#include <vector>
#include <string.h>

bool compare(std::string str1, std::string str2) {
    int min = (int) std::min(str1.size(), str2.size()); // идем по короткой символьной последовательности
    for (int i = 0; i < min; ++i) {  // сравниваем символы
        if (str1[i] > str2[i]) {
            return true;
        } else if (str1[i] < str2[i]) {
            return false;
        }
    }
    // если все символы были одинаковыми, то проверяем на размер слов
    if (str1.size() == str2.size()) {
        return false;
    }
    // если первое слово окажется больше размера минимального слова
    if (str1.size() == min) {
        return false;
    } else {
        return true;
    }
}

template<class T>
void InsertionSort(std::vector<T> &arr) {
    for (int i = 1; i <= arr.size() - 1; ++i) {
        std::string temp = arr[i]; //запоминаем элемент
        int j = i;
        while (j > 0 && compare(arr[j-1], temp)) {
            arr[j] = arr[j-1]; // поднимаем элемент
            --j;
        }
        arr[j] = temp; //в пустое место ставим элемент, который запомнили
    }
}

int main() {
    std::vector<std::string> arr;
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string str;
        std::cin >> str;
        arr.push_back(str);
    }

    InsertionSort(arr);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << std::endl;
    }

    return 0;
}