#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <map>

using namespace std;

// Генератор случайных чисел 
random_device rd;
ranlux48 gen(rd());

// Функция для генерации случайного числа в заданном диапазоне
int getRandomInt(int min, int max) {
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

// Вывод массива на экран
void printArray(const vector<int>& arr, const string& name = "") {
    if (!name.empty()) {
        cout << name << ": ";
    }
    for (const auto& elem : arr) {
        cout << setw(4) << elem << " ";
    }
    cout << endl;
}

// Проверка, является ли число простым
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Пункт 1
void task1(vector<int>& arr1, vector<int>& arr2) {
    cout << "\n Пункт 1" << endl;

    int n, k;
    cout << "Введите n (15 ≤ n ≤ 30): ";
    cin >> n;
    cout << "Введите k (5 ≤ k ≤ 10): ";
    cin >> k;

    // Проверка диапазонов
    if (n < 15) n = 15;
    if (n > 30) n = 30;
    if (k < 5) k = 5;
    if (k > 10) k = 10;

    // Инициализация массивов случайными числами из [10, 30]
    arr1.resize(n);
    arr2.resize(k);

    for (auto& elem : arr1) {
        elem = getRandomInt(10, 30);
    }
    for (auto& elem : arr2) {
        elem = getRandomInt(10, 30);
    }

    printArray(arr1, "Массив arr1");
    printArray(arr2, "Массив arr2");
}

// Пункт 2
void task2(const vector<int>& arr1) {
    cout << "\n Пункт 2" << endl;
    cout << "Чётные элементы, расположенные на нечётных индексах:" << endl;

    vector<int> result;
    for (size_t i = 0; i < arr1.size(); i++) {
        // Индексы: 1, 3, 5, ... (нечётные)
        if (i % 2 == 1 && arr1[i] % 2 == 0) {
            result.push_back(arr1[i]);
        }
    }

    cout << "Количество: " << result.size() << endl;
    if (!result.empty()) {
        cout << "Значения: ";
        for (int val : result) {
            cout << val << " ";
        }
        cout << endl;
    }
}

// Пункт 3
void task3(const vector<int>& arr1, const vector<int>& arr2) {
    cout << "\n Пункт 3 " << endl;
    cout << "Числа из arr2, встречающиеся в arr1:" << endl;

    map<int, int> frequency;

    // Подсчёт частоты каждого элемента в arr1
    for (int val : arr1) {
        frequency[val]++;
    }

    bool found = false;
    for (int val : arr2) {
        if (frequency[val] > 0) {
            cout << "Число " << val << " встречается " << frequency[val] << " раз(а)" << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "Ни одно число из arr2 не встречается в arr1" << endl;
    }
}

// Пункт 4
void task4(vector<int>& arr1) {
    cout << "\n Пункт 4 " << endl;

    printArray(arr1, "Массив до сдвига");

    if (arr1.empty()) return;

    // Циклический сдвиг влево на 1 позицию
    int first = arr1[0];
    for (size_t i = 0; i < arr1.size() - 1; i++) {
        arr1[i] = arr1[i + 1];
    }
    arr1[arr1.size() - 1] = first;

    printArray(arr1, "Массив после сдвига влево на 1");
}

// Пункт 5
void task5() {
    cout << "\n Пункт 5" << endl;

    int n;
    cout << "Введите n (n ≥ 15): ";
    cin >> n;
    if (n < 15) n = 15;

    // Создание массива из n чисел [10, 100]
    vector<int> original(n);
    for (auto& elem : original) {
        elem = getRandomInt(10, 100);
    }
    printArray(original, "Исходный массив");

    // Поиск простых чисел
    vector<int> primes;
    for (int val : original) {
        if (isPrime(val)) {
            primes.push_back(val);
        }
    }

    // Сортировка простых чисел по убыванию
    sort(primes.begin(), primes.end(), greater<int>());

    // Вывод простых чисел
    cout << "\nПростые числа (отсортированы по убыванию): ";
    if (primes.empty()) {
        cout << "нет простых чисел";
    }
    else {
        for (int val : primes) {
            cout << val << " ";
        }
    }
    cout << endl;

    // Замена составных чисел на 0 в исходном массиве
    vector<int> modified = original;
    for (int& val : modified) {
        if (!isPrime(val)) {
            val = 0;
        }
    }
    printArray(modified, "Модифицированный массив (составные заменены на 0)");

    // Вычисление сумм
    int sumOriginal = 0;
    for (int val : original) {
        sumOriginal += val;
    }

    int sumModified = 0;
    for (int val : modified) {
        sumModified += val;
    }

    cout << "\nСумма элементов исходного массива: " << sumOriginal << endl;
    cout << "Сумма элементов модифицированного массива: " << sumModified << endl;

    if (sumOriginal > sumModified) {
        cout << "Сумма исходного массива больше на " << (sumOriginal - sumModified) << endl;
    }
    else if (sumModified > sumOriginal) {
        cout << "Сумма модифицированного массива больше на " << (sumModified - sumOriginal) << endl;
    }
    else {
        cout << "Суммы равны" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "RUS");

    cout << "Вычисления в массивах" << endl;
  
    vector<int> arr1, arr2;

    // Пункт 1: Создание массивов
    task1(arr1, arr2);

    // Пункт 2: Чётные элементы на нечётных индексах
    task2(arr1);

    // Пункт 3: Числа из arr2 в arr1
    task3(arr1, arr2);

    // Пункт 4: Циклический сдвиг влево (модифицируем исходный arr1)
    task4(arr1);

    // Пункт 5: Простые числа
    task5();

    return 0;
}
