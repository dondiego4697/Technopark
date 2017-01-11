#include <iostream>
#include <vector>

// Габариты
struct Dimension {
    int max;
    int index;

    Dimension(int n) :
            max(n),
            index(0) {}

    ~Dimension() {}
};

// Сортировка вставкой
template<class T>
void insertion_sort(std::vector<T>& input) {
    for (int i = 1; i <= input.size() - 1; ++i) {
        Dimension buf = input[i];
        int index = i;

        while ((index > 0) && (input[index - 1].max > buf.max)) {
            std::swap(input[index], input[index - 1]);
            index--;
        }
    }
}


int main() {
    Dimension input_dimension(0);
    int number_of_inputs = 0;
    std::cin >> number_of_inputs;

    std::vector<Dimension> box_array;

    for (int i = 0; i < number_of_inputs; ++i) {
        int input_max = 0;
        int input_curr = 0;

        for (int j = 0; j < 3; ++j) {
            std::cin >> input_curr;
            input_max = std::max(input_max, input_curr);
        }

        input_dimension.max = input_max;
        input_dimension.index = i;

        box_array.push_back(input_dimension);
    }

    insertion_sort(box_array);

    for (int i = 0; i < box_array.size(); i++) {
        std::cout << box_array[i].index << " ";
    }
    return 0;
}