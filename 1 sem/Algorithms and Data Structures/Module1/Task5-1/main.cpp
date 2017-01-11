/* 5_1
* Дан фрагмент последовательности скобок, состоящей из символов (){}[]. 
* Требуется определить, возможно ли продолжить фрагмент в обе стороны, получив корректную  последовательность. 
* Длина исходной последовательности ≤ 200000.  Формат входных данных.​ Строка, содержащая символы (){}[] и, 
* возможно, перевод строки.  Формат выходных данных.​ Если возможно ­ вывести минимальную корректную последовательность, 
* иначе ­  напечатать "​IMPOSSIBLE​". 
* */
#include <iostream>
#include <assert.h>
#include <cstring>

using namespace std;

class CStack {
private:
    char *buffer;
    int bufferSize;
    int top; // Индекс верхнего элемента.

public:
    bool IsEmpty() const { return top == -1; }

    CStack(int _bufferSize) : bufferSize(_bufferSize), top(-1) {
        buffer = new char[bufferSize];
    }

    void Push(char a) {
        assert(top + 1 < bufferSize);
        buffer[++top] = a;
    }

    char Pop() {
        assert(top != -1);
        return buffer[top--];
    }

    char GetAt() {
        return buffer[top];
    }

    ~CStack() {
        delete[] buffer;
    }
};


//класс массива, который может добавлять элемент в конец, возвращать элемент, возвращать размер массива
class CArray {
private:
    char *Array = NULL;
    int size;

public:
    CArray(int size) {
        this->size = size;
        Array = new char[size];
    }

    void Add(int i, char elem) {
        Array[i] = elem;
    }

    char GetAt(int index) {
        return Array[index];
    }

    int GetSize() {
        return size;
    }

    ~CArray() {
        delete[] Array;
    }
};


char GetPair(char value){
    switch (value) {
        case '(':
            return ')';
        case '{':
            return '}';
        case '[':
            return ']';
        case ')':
            return '(';
        case '}':
            return '{';
        case ']':
            return '[';
        default:break;
    }
    return 0;
}

void RecountOpen(char value, int &OPEN1, int &OPEN2, int &OPEN3){
    switch (value) {
        case '(':
            ++OPEN1;
            break;
        case '{':
            ++OPEN2;
            break;
        case '[':
            ++OPEN3;
            break;
        case ')':
            --OPEN1;
            break;
        case '}':
            --OPEN2;
            break;
        case ']':
            --OPEN3;
            break;
        default:break;
    }
}

int GetOpenCount(char value, int &OPEN1, int &OPEN2, int &OPEN3){
    switch (value) {
        case ')':
            return OPEN1;
        case '}':
            return OPEN2;
        case ']':
            return OPEN3;
        default:break;
    }
    return 0;
}

//основная функция на проверку скобок, на входные данные приходит текущий элемент, stack - и открывающих и закрывающих скобок,
// и количество открывающих скобок всех типов
bool Check(char element, CStack &ArrOpen, CStack &ArrClose, int &OPEN1, int &OPEN2, int &OPEN3) {
    switch (element) {
        case '}':
        case ')':
        case ']':
            if (GetOpenCount(element, OPEN1, OPEN2, OPEN3) > 0) {
                if (ArrClose.GetAt() == element) {
                    ArrClose.Pop();
                    RecountOpen(element, OPEN1, OPEN2, OPEN3);
                } else {
                    return false;
                }
            } else {
                if (!ArrClose.IsEmpty()) {
                    return false;
                } else {
                    ArrOpen.Push(GetPair(element));
                }
            }
            break;
        case '{':
        case '(':
        case '[':
            RecountOpen(element, OPEN1, OPEN2, OPEN3);
            ArrClose.Push(GetPair(element));
            break;
        default:
            break;
    }
    return true;
}

int main() {
    // счетчики открытых скобок(OPEN1 - '{' OPEN2 - '(' OPEN3 -  '[')
    int OPEN1 = 0, OPEN2 = 0, OPEN3 = 0;
    //bool переменная, говорящая, можно ли достроить скобочноую последовательность
    bool check = true;

    //создаем два стека Открывающих скобок и Закрывающих скобок
    CStack ArrOpen(10000), ArrClose(10000);

    //ввод
    string in;
    cin >> in;

    //создаем массив из скобок
    CArray List(in.length());
    //добавляем в массив каждую скобку
    for (int i = 0; i < in.length(); ++i) {
        List.Add(i, in[i]);
    }

    //идем по массиву скобок, и проверяем на возможность добавления новых
    for (int i = 0; i < List.GetSize(); ++i) {
        //если не возможно достроить, то закрываем цикл и записываем false в check
        if (!Check(List.GetAt(i), ArrOpen, ArrClose, OPEN1, OPEN2, OPEN3)) {
            check = false;
            break;
        }
    }

    //вывод
    if (check) {
        //массив открывающихся
        while (!ArrOpen.IsEmpty()) {
            cout << ArrOpen.Pop();
        }

        //изначально данный массив
        for (int i = 0; i < List.GetSize(); ++i) {
            cout << List.GetAt(i);
        }
        //массив закрывающихся
        while (!ArrClose.IsEmpty()) {
            cout << ArrClose.Pop();
        }
    } else {
        cout << "IMPOSSIBLE";
    }
    return 0;
}