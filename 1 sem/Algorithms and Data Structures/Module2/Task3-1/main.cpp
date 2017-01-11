/* 3_1. Реклама.
 * В супермаркете решили оптимизировать показ рекламы.
 * Известно расписание прихода и ухода покупателей (два целых числа).
 * Каждому покупателю необходимо показать минимум 2 рекламы.
 * Рекламу можно транслировать только в целочисленные моменты времени.
 * Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
 * В каждый момент времени может показываться только одна реклама.
 * Считается, что реклама показывается мгновенно.
 * Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
 * Требуется определить минимальное число показов рекламы.
 */
#include <iostream>
#include <vector>

using namespace std;

struct Pare {
    int start = 0;
    int finish = 0;

    Pare(int s, int f) : start(s), finish(f) {};
};

bool operator<(Pare &x, Pare &y) {
    if (x.finish < y.finish) return true;
    if ((x.finish == y.finish) && (x.start < y.start)) return true;
    return false;
}

bool Compare(Pare &x, Pare &y) {
    return x < y;
}

template<class T>
void ShiftDown(vector<T> &arr, int i, int j) {
    int MaxNodeId;
    bool ShiftDone = false;
    while (((i * 2 + 1) < j) && !ShiftDone) {
        if (i * 2 + 1 == j - 1 || !Compare(arr[i * 2 + 1], arr[i * 2 + 2])) {
            MaxNodeId = i * 2 + 1;
        } else {
            MaxNodeId = i * 2 + 2;
        }

        if (Compare(arr[i], arr[MaxNodeId])) {
            swap(arr[i], arr[MaxNodeId]);
            i = MaxNodeId;
        } else {
            ShiftDone = true;
        }
    }
}

template<class T>
void HeapSort(vector<T> &arr) {
    int l = arr.size();
    int i;
    //Строим дерево поиска
    for (i = (l / 2) - 1; i > -1; i--) {
        ShiftDown(arr, i, l);
    }

    //Забираем максимальный (0) элемент дерева в i-ю позицию
    //Перемещаем новый 0 элемент на правильную позицию в дереве
    for (i = l - 1; i > 0; i--) {
        swap(arr[i], arr[0]);
        ShiftDown(arr, 0, i);
    }
}

template<class T>
int GetResult(vector<T> &arr) {
    int n = arr.size();
    int result = 0;

    int adv1 = 0, adv2 = 0; // две рекламы
    adv1 = arr[0].finish;
    adv2 = arr[0].finish - 1;
    result = 2;

    for (int i = 1; i < n; ++i) { // идем по промежуткам времени посетителей
        if(adv1 < arr[i].start){  // если реклама предыдущего посетителя не попадает в период посещения текущему
            if(adv2 == arr[i].finish){ // если вторая реклама уже стоит в конце, то первую ставим в предконец, если нет, то в конец
                adv1 = arr[i].finish - 1;
            } else {
                adv1 = arr[i].finish;
            }
            ++result;
        }

        if(adv2 < arr[i].start){ // если реклама предыдущего посетителя не попадает в период посещения текущему
            if(adv1 == arr[i].finish){
                adv2 = arr[i].finish - 1;
            } else {
                adv2 = arr[i].finish;
            }
            ++result;
        }
    }

    return result;
}

int main() {
    int n = 0;
    cin >> n;
    vector<Pare> arr;
    for (int i = 0; i < n; ++i) {
        int s = 0, f = 0;
        cin >> s >> f;
        arr.push_back(Pare(s, f));
    }

    HeapSort(arr);

    cout << GetResult(arr);
    return 0;
}