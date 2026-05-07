#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct StepData {
    int n;
    double x, x_next, diff;
};

struct BisectionStep {
    int n;
    double a, b, mid;
};

double f(double x) { 
    return x * x - 3 * sin(x); 
}
double df(double x) {
    return 2 * x - 3 * cos(x); 
}
double phi(double x) {
    return sqrt(3 * sin(x)); 
}

void printTable(const string& title, const vector<StepData>& steps) {
    cout << "\n--- " << title << " ---\n";
    cout << "  N |     x_n      |    x_{n+1}   |    разность  \n";
    for (const auto& s : steps) {
        cout << setw(3) << s.n << " | " << fixed << setprecision(8)
            << setw(12) << s.x << " | " << setw(12) << s.x_next << " | "
            << setw(12) << s.diff << endl;
    }
    cout << "Результат: " << setprecision(8) << steps.back().x_next << endl;
}

void solveBisection(double a, double b, double eps) {
    vector<BisectionStep> history;
    int n = 0;
    while ((b - a) > eps && n < 100) {
        double mid = (a + b) / 2;
        history.push_back({ n, a, b, mid });
        if (f(a) * f(mid) < 0) b = mid;
        else a = mid;
        n++;
    }
    cout << "\nМетод половинного деления\n";
    cout << "  N |      a       |      b       |    b - a     |     x_mid    \n";
    cout << "------------------------------------------------------------------\n";
    for (const auto& s : history) {
        cout << setw(3) << s.n << " | "
            << setw(12) << fixed << setprecision(6) << s.a << " | "
            << setw(12) << s.b << " | "
            << setw(12) << (s.b - s.a) << " | "
            << setw(12) << s.mid << endl;
    }
    double root = (a + b) / 2;
    cout << "\nКорень: " << setprecision(8) << root << endl;
    cout << "f(x*) = " << setprecision(10) << f(root) << endl;
}

void solveNewton(double x, double eps) {
    vector<StepData> history;
    for (int n = 0; n < 100; ++n) {
        double next = x - f(x) / df(x);
        double diff = abs(next - x);
        history.push_back({ n, x, next, diff });
        if (diff < eps) break;
        x = next;
    }
    printTable("Метод Ньютона", history);
    cout << "f(x*) = " << f(history.back().x_next) << endl;
}

void solveIteration(double x, double eps) {
    // Проверка сходимости метода простых итераций
    double h = 1e-6;
    double phi_deriv = abs((phi(x + h) - phi(x)) / h);
    cout << "\nПроверка сходимости\n";
    cout << "|φ'(x0)| ≈ " << fixed << setprecision(10) << phi_deriv;
    if (phi_deriv < 1) {
        cout << " < 1 условие сходимости выполняется\n";
    }
    else {
        cout << " ≥ 1 метод может расходиться\n";
    }

    vector<StepData> history;
    for (int n = 0; n < 100; ++n) {
        double next = phi(x);
        double diff = abs(next - x);
        history.push_back({ n, x, next, diff });
        if (diff < eps) break;
        x = next;
    }
    printTable("Метод итераций", history);
    cout << "f(x*) = " << f(history.back().x_next) << endl;
}

int main() {
    setlocale(LC_ALL, "ru");
    double eps = 1e-4;

    cout << "Решение уравнения x^2 - 3sin(x) = 0\n";
    cout << "Интервал с корнем: [1, 2]\n";

    solveBisection(1.0, 2.0, eps);
    solveNewton(1.8, eps);
    solveIteration(1.5, eps);

    return 0;
}
