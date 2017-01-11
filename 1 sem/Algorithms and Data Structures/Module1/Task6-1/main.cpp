/*
 * Дано N кубиков. Требуется определить каким количеством способов можно выстроить из этих кубиков  пирамиду. 
 * Формат входных данных:  На вход подается количество кубиков N. 
 * Формат выходных данных:  Вывести число различных пирамид из N кубиков.   
 *
 * 6.1
 * ​Высокая пирамида. ​Каждый вышележащий слой пирамиды должен быть не больше нижележащего.  N ​≤ 200. 
 */
#include <iostream>
#include <assert.h>

using namespace std;

//рекурсивный алгоритм, который находи кол-во вариантов построения высокой пирамиды
//на вход n - кол-во кубиков оставшихся, o - кол-во кубиков сверху, array - массив кешированных значений
long long calculate(int n, int o, long long** array)
{
    long long result = 0;

    for (int i = o; i <= n / 2; ++i)
    {
        //если значение нет, продолжаем рекурсию и записываем ее значение в кеширующий массив
        if (array[n - 1][i] == -1) {
            array[n - 1][i] = calculate(n - i, i, array);
        }
        // прибавляем значение из кеша
        result += array[n - 1][i];
    }

    return result + 1;
}


int main() {
    //отмечу, что использую long long, так как значения большие
    //при n = 200
    // в long long было 3972999029388 :)
    // в int было 154280588 :(
    long long result = 0;
    int n = 0;

    cin >> n;
    assert(n > 0 && n <= 200);

    //массив для кеширования результатов
    int strs = n + 1;
    int rows = n/2 + 1;
    long long **array = new long long* [strs];
    for (int i = 0; i < strs; i++) {
        array[i] = new long long [rows];
    }

    //заполняем его пустыми значениями
    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= n/2; ++j)
            array[i][j] = -1;

    result = calculate(n, 1, array);

    //вывод
    cout << result;

    for(int i = 0; i < n; ++i){
        delete[] array[i];
    }
    delete[] array;
    return 0;
}