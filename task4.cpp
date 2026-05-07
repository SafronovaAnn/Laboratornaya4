#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <limits>

using namespace std;

// Метод Борда
void bordaMethod(const vector<string>& candidates, const vector<vector<int>>& rankings,
    const vector<int>& votesCount) {

    int n = candidates.size();
    map<string, int> scores;

    for (const auto& cand : candidates) {
        scores[cand] = 0;
    }

    // Подсчёт очков
    for (size_t place = 0; place < rankings.size(); place++) {
        for (size_t col = 0; col < rankings[place].size(); col++) {
            int candidateIndex = rankings[place][col];
            int points = n - 1 - place;
            scores[candidates[candidateIndex]] += points * votesCount[col];
        }
    }

    // Поиск победителя
    string winner;
    int maxScore = -1;
    for (const auto& pair : scores) {
        if (pair.second > maxScore) {
            maxScore = pair.second;
            winner = pair.first;
        }
    }

    cout << "По Борду: " << winner << " (" << maxScore << " очков)" << endl;
}

// Метод Кондорсе 
void condorcetMethod(const vector<string>& candidates, const vector<vector<int>>& rankings,
    const vector<int>& votesCount) {

    int n = candidates.size();
    int m = votesCount.size();
    vector<vector<int>> wins(n, vector<int>(n, 0));

    // Подсчёт парных сравнений
    for (int group = 0; group < m; group++) {
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int candidateHigher = rankings[i][group];
                int candidateLower = rankings[j][group];
                wins[candidateHigher][candidateLower] += votesCount[group];
            }
        }
    }

    // Поиск победителя Кондорсе
    string winner;
    bool found = false;

    for (int i = 0; i < n; i++) {
        bool beatsAll = true;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                if (wins[i][j] <= wins[j][i]) {
                    beatsAll = false;
                    break;
                }
            }
        }
        if (beatsAll) {
            winner = candidates[i];
            found = true;
            break;
        }
    }

    if (found) {
        cout << "По Кондорсе: " << winner << endl;
    }
    else {
        cout << "По Кондорсе: не определён" << endl;
    }
}

// Вывод таблицы
void printTable(const vector<string>& candidates, const vector<int>& votesCount,
    const vector<vector<int>>& rankings) {

    int n = candidates.size();
    int m = votesCount.size();

    // Задаем фиксированную ширину колонок
    const int firstColWidth = 25; // Для колонки "Место"
    const int dataColWidth = 10;  // Для колонок с группами

    // Вспомогательная функция для отрисовки горизонтальной линии
    auto drawLine = [&]() {
        cout << "+-" << string(firstColWidth, '-') << "-+";
        for (int i = 0; i < m; i++) {
            cout << string(dataColWidth + 1, '-') << "+";
        }
        cout << endl;
        };

    cout << "\nРезультаты ранжирования:" << endl;

    drawLine();

    // Шапка таблицы: Количество избирателей
    cout << "| " << left << setw(firstColWidth) << "Количество избирателей" << " |";
    for (int count : votesCount) {
        cout << " " << right << setw(dataColWidth - 1) << count << "  |";
    }
    cout << endl;

    drawLine();

    // Строки: Места и кандидаты
    for (int place = 0; place < n; place++) {
        int points = n - 1 - place;

        // Формируем текст для первой колонки
        string placeText = to_string(place + 1) + "-е место (" + to_string(points) + ")";

        cout << "| " << left << setw(firstColWidth) << placeText << " |";

        for (int col = 0; col < m; col++) {
            // Выводим имя кандидата, ограничивая его ширину
            string candName = candidates[rankings[place][col]];
            cout << " " << left << setw(dataColWidth - 1) << candName << "  |";
        }
        cout << endl;
    }

    drawLine();
}

// Ввод данных
void inputData(vector<string>& candidates, vector<int>& votesCount,
    vector<vector<int>>& rankings) {

    cout << "Теория голосований" << endl;

    int n;
    cout << "\nВведите количество кандидатов: ";
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    candidates.resize(n);
    cout << "\nВведите имена кандидатов:\n";
    for (int i = 0; i < n; i++) {
        cout << "  Кандидат " << i + 1 << ": ";
        getline(cin, candidates[i]);
    }

    int m;
    cout << "\nВведите количество групп избирателей: ";
    cin >> m;

    votesCount.resize(m);
    cout << "\nВведите количество избирателей в каждой группе:" << endl;
    for (int i = 0; i < m; i++) {
        cout << "  Группа " << i + 1 << ": ";
        cin >> votesCount[i];
    }

    rankings.resize(n, vector<int>(m));
    cout << "\nВвод предпочтений избирателей \n";
    cout << "Кандидаты: ";
    for (const auto& cand : candidates) {
        cout << cand << " ";
    }
    cout << "\n\n";

    for (int place = 0; place < n; place++) {
        cout << place + 1 << "-е место:\n";
        for (int group = 0; group < m; group++) {
            string candidateName;
            cout << "  Группа " << group + 1 << " (" << votesCount[group] << " изб.): ";
            cin >> candidateName;

            int idx = -1;
            for (size_t i = 0; i < candidates.size(); i++) {
                if (candidates[i] == candidateName) {
                    idx = i;
                    break;
                }
            }

            if (idx != -1) {
                rankings[place][group] = idx;
            }
            else {
                cout << "    Ошибка! Кандидат не найден. Повторите ввод.\n";
                group--;
            }
        }
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "ru");

    vector<string> candidates;
    vector<int> votesCount;
    vector<vector<int>> rankings;

    inputData(candidates, votesCount, rankings);
    printTable(candidates, votesCount, rankings);

    cout << "\n";
    bordaMethod(candidates, rankings, votesCount);
    condorcetMethod(candidates, rankings, votesCount);

    return 0;
}
