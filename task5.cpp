#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <iomanip>

using namespace std;

// Генерируем случайное число от 100 до 200 для раундов
int getRandomRounds() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(100, 200);
    return dist(gen);
}

// Функции алгоритмов
// Алгоритм 1: Всегда сотрудничает
bool alwaysCooperate(int32_t round_number, const vector<bool>& self_choices,
    const vector<bool>& enemy_choices) {
    // Не смотрим на историю, всегда возвращаем true (сотрудничество)
    return true;
}

// Алгоритм 2: Всегда предаёт
bool alwaysDefect(int32_t round_number, const vector<bool>& self_choices,
    const vector<bool>& enemy_choices) {
    // Не смотрим на историю, всегда возвращаем false (предательство)
    return false;
}

// Алгоритм 3: Око за око
// Правила: 
//   1. В первом раунде сотрудничает
//   2. В следующих раундах повторяет ход противника из предыдущего раунда
bool titForTat(int32_t round_number, const vector<bool>& self_choices,
    const vector<bool>& enemy_choices) {
    // Первый раунд (номер 0) - сотрудничаем
    if (round_number == 0) {
        return true;  // сотрудничество
    }
    // В следующих раундах делаем то же, что противник делал в прошлый раз
    return enemy_choices[round_number - 1];
}

// Функция подсчета очков
// Возвращает очки первого и второго игрока
pair<int, int> getRoundPoints(bool choice1, bool choice2) {
    // Оба сотрудничают
    if (choice1 == true && choice2 == true) {
        return { 24, 24 };
    }
    // Первый сотрудничает, второй предаёт
    else if (choice1 == true && choice2 == false) {
        return { 0, 20 };
    }
    // Первый предаёт, второй сотрудничает
    else if (choice1 == false && choice2 == true) {
        return { 20, 0 };
    }
    // Оба предают
    else {
        return { 4, 4 };
    }
}

// Функция печати выбора
string choiceToString(bool choice) {
    if (choice == true) {
        return "Сотрудничает";
    }
    else {
        return "Предаёт     ";
    }
}

// Запуск одной игры
void runGame(bool (*algo1)(int32_t, const vector<bool>&, const vector<bool>&),
    bool (*algo2)(int32_t, const vector<bool>&, const vector<bool>&),
    const string& name1, const string& name2,
    int rounds, int gameNumber) {

    // Векторы для хранения истории ходов
    vector<bool> history1;  // история выборов первого алгоритма
    vector<bool> history2;  // история выборов второго алгоритма

    int totalScore1 = 0;  // общие очки первого
    int totalScore2 = 0;  // общие очки второго

    cout << "\n========================================\n";
    cout << "Игра " << gameNumber << ": " << name1 << " против " << name2 << "\n";
    cout << "Всего раундов: " << rounds << "\n\n";

    cout << "  Раунд | " << left << setw(14) << name1 << " | "
        << left << setw(14) << name2 << " | Очки1 | Очки2\n";
    cout << "  ------+----------------+----------------+-------+-------\n";

    // Проходим по всем раундам
    for (int32_t round = 0; round < rounds; round++) {
        // Запрашиваем выбор каждого алгоритма
        // Передаём им номер раунда и историю предыдущих ходов
        bool choice1 = algo1(round, history1, history2);
        bool choice2 = algo2(round, history2, history1);

        // Сохраняем выборы в историю
        history1.push_back(choice1);
        history2.push_back(choice2);

        // Подсчитываем очки за раунд
        auto [points1, points2] = getRoundPoints(choice1, choice2);
        totalScore1 += points1;
        totalScore2 += points2;

        // Выводим результат раунда (показываем первые 10 и последние 5)
        if (round < 10 || round >= rounds - 5) {
            cout << "  " << setw(4) << round + 1 << " | "
                << left << setw(14) << choiceToString(choice1) << " | "
                << left << setw(14) << choiceToString(choice2) << " | "
                << right << setw(5) << points1 << " | " << setw(5) << points2 << "\n";
        }
        else if (round == 10) {
            cout << "   ... | ...            | ...            |  ... |  ...\n";
        }
    }

    cout << "\n" << string(50, '-') << "\n";
    cout << "Итоги игры " << gameNumber << ":\n";
    cout << "  " << name1 << ": " << totalScore1 << " очков\n";
    cout << "  " << name2 << ": " << totalScore2 << " очков\n";

    if (totalScore1 > totalScore2) {
        cout << "  Победитель: " << name1 << "\n";
    }
    else if (totalScore2 > totalScore1) {
        cout << "  Победитель: " << name2 << "\n";
    }
    else {
        cout << "  Ничья\n";
    }
}

// ГЛАВНАЯ ФУНКЦИЯ
int main() {
    setlocale(LC_ALL, "ru");

    cout << "Дилемма заключенного \n";


    // Случайное количество раундов от 100 до 200
    int rounds = getRandomRounds();
    cout << "\nКоличество раундов: " << rounds << "\n";

    // ===== ЗАПУСКАЕМ 3 ИГРЫ =====
    // Игра 1: alwaysCooperate vs alwaysDefect
    runGame(alwaysCooperate, alwaysDefect,
        "alwaysCooperate", "alwaysDefect",
        rounds, 1);

    // Игра 2: alwaysCooperate vs titForTat
    runGame(alwaysCooperate, titForTat,
        "alwaysCooperate", "titForTat",
        rounds, 2);

    // Игра 3: alwaysDefect vs titForTat
    runGame(alwaysDefect, titForTat,
        "alwaysDefect", "titForTat",
        rounds, 3);

    // Вывод лучшего алгоритма
    cout << "\n Лучшим алгоритмом оказался: titForTat\n";

    return 0;
}
