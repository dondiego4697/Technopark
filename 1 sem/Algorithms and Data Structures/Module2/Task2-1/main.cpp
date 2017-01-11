/* 2_1. Жадина.
 * Вовочка ест фрукты из бабушкиной корзины.
 * В корзине лежат фрукты разной массы.
 * Вовочка может поднять не более K грамм. Каждый фрукт весит не более K грамм.
 * За раз он выбирает несколько самых тяжелых фруктов, которые может поднять одновременно,
 * откусывает от каждого половину и кладет огрызки обратно в корзину.
 * Если фрукт весит нечетное число грамм, он откусывает большую половину.
 * Фрукт массы 1 гр он съедает полностью. Определить за сколько подходов Вовочка съест все фрукты в корзине.
 * */
#include <iostream>
#include <vector>
#include <assert.h>

class Heap {
private:
    std::vector<int> arr;
    int size;

public:
    Heap() : size(0) {
        arr.push_back(0); // добавляем, но не учитываем нулевой элемент в будущем
    }

    bool IsEmpty() {
        return size == 0;
    }

    void Insert(int x) {
        arr.push_back(x);
        for (int i = size + 1; i > 1;) {
            if (arr[i] > arr[i / 2]) {
                std::swap(arr[i], arr[i / 2]);
                i = i / 2;
            } else {
                break;
            }
        }
        ++size;
    }

    int Pop() {
        int a = arr[1];
        arr[1] = arr[size];
        arr.pop_back();

        for (int i = 1; 2 * i < size;) {
            i *= 2;
            if (i + 1 < size && arr[i] < arr[i + 1]) {
                i += 1;
            }
            if (arr[i / 2] < arr[i]) {
                std::swap(arr[i / 2], arr[i]);
            }
        }
        --size;
        return a;
    }

    int Top() {
        return arr[1];
    }
};

int GetStep(Heap &heap, int k) {
    int result = 0;
    std::vector<int> currentArr; //Массив яблок, взятых при одном подходе

    while (!heap.IsEmpty()) {
        int sum = 0;
        //Берем самые тяжелые яблоки, соблюдая грузоподъемность
        do {
            int apple = heap.Pop();
            sum += apple;
            currentArr.push_back(apple);
        } while (sum + heap.Top() <= k && !heap.IsEmpty());

        //Каждое яблоко делим по полам и возвращаем оставшееся
        for (int i = currentArr.size() - 1; i >= 0; --i) {
            if (currentArr[i] != 1) {
                heap.Insert(currentArr[i] / 2);
            }
            currentArr.pop_back();
        }
        result++;
    }
    return result;
}

int main() {
    int n = 0;
    std::cin >> n;
    Heap heap;

    for (int i = 0; i < n; ++i) {
        int x = 0;
        std::cin >> x;
        heap.Insert(x);
    }

    int k = 0;
    std::cin >> k;
    std::cout << GetStep(heap, k);

    return 0;
}