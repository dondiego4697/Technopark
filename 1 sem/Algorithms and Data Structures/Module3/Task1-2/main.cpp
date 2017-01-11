/* 1_2.​ Для разрешения коллизий используйте двойное хеширование.
 Реализуйте структуру данных типа “множество строк” на основе динамической
 хеш-таблицы с открытой адресацией.
 Хранимые строки непустые и состоят из строчных латинских букв.
 Начальный размер таблицы должен быть равным 8-ми.
 Перехеширование выполняйте при добавлении элементов в случае,
 когда коэффициент заполнения таблицы достигает 3/4.
 Структура данных должна поддерживать операции добавления строки в множество,
 удаления строки из множества и проверки принадлежности данной строки множеству.
 */
#include <iostream>
#include <string>
#include <vector>

#define START_SIZE 8
#define REHASH 3.0/4.0
#define HCODE1 1111
#define HCODE2 2222

using std::string;
using std::vector;

class HashTable {
public:
    HashTable();

    ~HashTable() = default;

    // Проверка наличия ключа в таблице
    bool Has(const string &key) const;

    // Добавление ключа в таблицу. Если ключ уже есть, возвращает false
    bool Add(const string &key);

    // Удаление ключа из таблицы. Если ключа не было, возвращает false
    bool Remove(const string &key);

    // Перехеширование таблицы
    void Rehash();

private:
    struct Node {
        string Data;
        bool IsDeleted;

        explicit Node(const string &data) : Data(data), IsDeleted(false) {}
    };

    vector<Node *> table; // хеш-таблицы
    int fact_size; // кол-во ячеек, в которых записано значения
    // в vector table - хранится размер таблицы
};

HashTable::HashTable() {
    table.resize(START_SIZE, NULL);
    fact_size = 0;
}

HashTable::~HashTable() {

}

// CreateH1 и CreateH2 - две независимые хеш-функции, value - ключ, size - размер хеш-таблицы
int CreateH1(string value, int size) {
    int result = 0;
    for (int i = 0; i < value.size(); ++i) {
        result = (result * HCODE1 + value[i]) % size;
    }
    return result;
}

int CreateH2(string value, int size) {
    int result = 0;
    for (int i = 0; i < value.size(); ++i) {
        result = (result * HCODE2 + value[i]) % size;
    }
    return (2 * result + 1) % size;
}


bool HashTable::Has(const string &value) const {
    int hash1 = CreateH1(value, table.size());
    int hash2 = CreateH2(value, table.size());

    int i = 0;
    while (table[hash1] != NULL && i < table.size()) {
        if (table[hash1]->Data == value && !table[hash1]->IsDeleted) {
            return true;
        }
        hash1 = (hash1 + hash2) % table.size();
        ++i;
    }
    return false;
}

bool HashTable::Add(const string &value) {
    if ((fact_size / table.size() >= REHASH)) {
        Rehash();
    }

    int hash1 = CreateH1(value, table.size());
    int hash2 = CreateH2(value, table.size());

    int index = -1; // чтобы поймать первое нахождение пустого элемента

    int i = 0;
    while (table[hash1] != NULL && i < table.size()) {
        if (table[hash1]->Data == value && !table[hash1]->IsDeleted) {
            return false;
        }

        if (table[hash1]->IsDeleted && index < 0) {
            index = hash1;
        }

        hash1 = (hash1 + hash2) % table.size();
        ++i;
    }


    if (index >= 0) {
        table[index]->Data = value;
        table[index]->IsDeleted = false;
    } else {
        table[hash1] = new Node(value);
    }

    ++fact_size;
    return true;
}

bool HashTable::Remove(const string &value) {
    int hash1 = CreateH1(value, table.size());
    int hash2 = CreateH2(value, table.size());

    int i = 0;
    while (table[hash1] != NULL && i < table.size()) {
        if (table[hash1]->Data == value && !table[hash1]->IsDeleted) {
            table[hash1]->IsDeleted = true;
            return true;
        }
        hash1 = (hash1 + hash2) % table.size();
        ++i;
    }
    return false;
}

void HashTable::Rehash() {
    unsigned int newSize = table.size() * 2;
    vector<Node *> newTable;
    newTable.resize(newSize, NULL);

    for (int i = 0; i < table.size(); ++i) {
        if (table[i] != NULL && !table[i]->IsDeleted) {
            string value = table[i]->Data;

            int hash1 = CreateH1(value, newSize);
            int hash2 = CreateH2(value, newSize);

            int j = 0;
            while (newTable[hash1] != NULL && j < newSize) {
                hash1 = (hash1 + hash2) % newSize;
                ++j;
            }
            newTable[hash1] = new Node(value);
        }
    }

    table = newTable;
}


int main() {

    HashTable table;

    char command = 0;
    string value;

    while (true) {
        std::cin >> command >> value;
        if (std::cin.fail()) {
            break;
        }

        switch (command) {
            case '?':
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }

    return 0;
}