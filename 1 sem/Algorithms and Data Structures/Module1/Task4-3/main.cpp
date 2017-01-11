/* Во всех задачах из следующего списка следует написать структуру данных, 
 * обрабатывающую команды  push* и pop*.  Формат входных данных. 
 * В первой строке количество команд n. n ≤ 1000000. 
 * Каждая команда задаётся как 2 целых числа: a b. 
 * a = 1 ­ push front 
 * a = 2 ­ pop front 
 * a = 3 ­ push back 
 * a = 4 ­ pop back 
 * Для очереди используются команды 2 и 3. 
 * Для дека используются все четыре команды.  Если дана команда pop*, то число b ­ ожидаемое значение.
 * Если команда pop вызвана для пустой структуры  данных, то ожидается “­1”.   Формат выходных данных. 
 * Требуется напечатать YES ­ если все ожидаемые значения совпали. 
 * Иначе, если хотя бы одно ожидание не  оправдалось, то напечатать NO. 
 *
 *
 * 4_3.
 * Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью 
 * динамического буфера. */
#include <iostream>
#include <assert.h>

using namespace std;

//исправил стек, сделал как надо :)
class Stack {
private:
    int *buffer;
    int bufferSize;
    int top;

public:
    bool IsEmpty() const { return top == -1; }

    Stack(int _bufferSize) : bufferSize(_bufferSize), top(-1) {
        buffer = new int[bufferSize];
    }

    void BufferUp() {
        int *buff = new int[2 * bufferSize];
        int i = 0;
        for (; i < bufferSize; ++i) {
            buff[i] = buffer[i];
        }
        bufferSize *= 2;

        delete[] buffer;
        buffer = buff;
    }

    void Push(int a) {
        if (top == bufferSize) {
            BufferUp();
        }
        buffer[++top] = a;
    }

    int Pop() {
        if (top != -1) {
            int check = buffer[top--];
            return check;
        } else {
            return -1;
        }
    }

    ~Stack() {
        delete[] buffer;
    }
};

class Queue {
    Stack in;
    Stack out;

public:
    Queue() : in(4), out(4) {

    }

    void Enqueue(int value) {
        in.Push(value);
    }

    int Decqueue() {
        if(out.IsEmpty()) {
            while (!in.IsEmpty()) {
                out.Push(in.Pop());
            }
        }
        return out.Pop();
    }
};

struct Task {
    int command = 0, value = 0;
};

int main() {
    int n = 0;
    cin >> n;
    assert(n <= 1000000 && n > 0);

    //создаем структуру Task чтобы записать последовательность команд
    Task *task = new Task[n];
    for (int i = 0; i < n; ++i) {
        cin >> task[i].command >> task[i].value;
    }


    Queue queue;
    bool check = true;
    for (int i = 0; i < n; ++i) {
        if (task[i].command == 2) {
            if (queue.Decqueue() != task[i].value) {
                check = false;
                break;
            }
        } else if (task[i].command == 3) {
            queue.Enqueue(task[i].value);
        }
    }

    if (check) {
        cout << "YES";
    } else {
        cout << "NO";
    }

    delete[] task;

    return 0;
}