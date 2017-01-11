#include <iostream>
#include <algorithm>

using namespace std;

class Athlet {
public:
    int s = 0, m = 0;

    Athlet() {

    }

    Athlet(int m, int s) {
        this->s = s;
        this->m = m;
    }

    ~Athlet() {

    }
};

class Array {
    int size = 0;
    int currentSize = 0;
    Athlet *array = new Athlet[size];

public:
    Array() : size(4), currentSize(0) {

    }

    void BufferUp() {
        Athlet *buff = new Athlet[2 * size];
        int i = 0;
        for (; i < size; ++i) {
            buff[i] = array[i];
        }
        delete[] array;
        array = buff;
    }

    void Add(Athlet athlet) {
        if (currentSize == size) {
            BufferUp();
            size *= 2;
        }
        array[currentSize++] = athlet;
    }

    Athlet GetAthlet(int i) {
        return array[i];
    }

    void SetAthlet(Athlet athlet, int i) {
        array[i] = athlet;
    }

    int GetSize() {
        return currentSize;
    }

    ~Array() {
        delete[] array;
    }
};

//сортирую по массе
void ShiftDown(Array &array, int i, int j)
{
    int MaxNodeId;
    bool ShiftDone = false;
    while (((i * 2 + 1) < j) && !ShiftDone)
    {
        if (i * 2 + 1 == j - 1 || array.GetAthlet(i * 2 + 1).m > array.GetAthlet(i * 2 + 2).m || array.GetAthlet(i * 2 + 1).s > array.GetAthlet(i * 2 + 2).s)
        {
            MaxNodeId = i * 2 + 1;
        }
        else
        {
            MaxNodeId = i * 2 + 2;
        }

        if (array.GetAthlet(i).m < array.GetAthlet(MaxNodeId).m || array.GetAthlet(i).s < array.GetAthlet(MaxNodeId).s)
        {
            //swap
            Athlet buff = array.GetAthlet(i);
            array.SetAthlet(array.GetAthlet(MaxNodeId), i);
            array.SetAthlet(buff, MaxNodeId);
            i = MaxNodeId;
        }
        else
        {
            ShiftDone = true;
        }
    }
}

void HeapSort(Array &array, int l)
{
    int i;
    //Строим дерево поиска
    for (i = (l / 2) - 1; i > -1; i--)
    {
        ShiftDown(array, i, l);
    }

    //Забираем максимальный (0) элемент дерева в i-ю позицию
    //Перемещаем новый 0 элемент на правильную позицию в дереве
    for (i = l - 1; i > 0; i--)
    {
        //swap
        Athlet buff = array.GetAthlet(i);
        array.SetAthlet(array.GetAthlet(0), i);
        array.SetAthlet(buff, 0);
        ShiftDown(array, 0, i);
    }
}

int main() {
    Array array;
    int a, b;
    while (cin >> a >> b) {
        Athlet athlet(a, b);
        array.Add(athlet);
    }

    //посмотрел пирамидальную сортировку и взял готовую реализацию
    HeapSort(array, array.GetSize());

    int sumM = 0;

    int count = 0;
    for (int i = 0; i < array.GetSize(); ++i) {
        if (array.GetAthlet(i).s >= sumM) {
            sumM += array.GetAthlet(i).m;
            count++;
        }
    }

    cout << count << endl;
    return 0;
}





















