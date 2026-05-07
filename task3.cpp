#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdint>

using namespace std;

// Алгоритм Xorshift32 (сдвиги: 13, 17, 5)
uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main() {
    setlocale(LC_ALL, "ru");
    cout << "Генератор Xorshift" << endl;

    int N;
    uint32_t seed;

    cout << "Введите начальное значение: ";
    cin >> seed;

    // Проверка: seed не должен быть равен 0
    while (seed == 0) {
        cout << "Ошибка: начальное значение не может быть равно 0 ." << endl;
        cout << "Введите начальное значение ≠ 0: ";
        cin >> seed;
    }

    cout << "Введите количество чисел для генерации: ";
    cin >> N;

    // Начальное значение
    uint32_t current_state = seed;

    vector<uint32_t> sequence;
    sequence.reserve(N);

    // Генерация последовательности
    for (int i = 0; i < N; ++i) {
        sequence.push_back(xorshift32(current_state));
    }

    // Вывод
    cout << "\nВывод: " << endl;

    int count = 0;
    for (const auto& val : sequence) {
        cout << setw(12) << val << " ";
        if (++count % 5 == 0) cout << endl;
    }

    return 0;
}
