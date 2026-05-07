struct StepData {
    n: i32,
    x: f64,
    x_next: f64,
    diff: f64,
}

struct BisectionStep {
    n: i32,
    a: f64,
    b: f64,
    mid: f64,
}

// Функции согласно варианту №6
fn f(x: f64) -> f64 {
    x * x - 3.0 * x.sin()
}

fn df(x: f64) -> f64 {
    2.0 * x - 3.0 * x.cos()
}

fn phi(x: f64) -> f64 {
    (3.0 * x.sin()).sqrt()
}

fn print_table(title: &str, steps: &[StepData]) {
    println!("\n--- {} ---", title);
    println!("  N |     x_n      |    x_{{n+1}}   |    разность  ");
    println!("-------------------------------------------------");
    for s in steps {
        println!("{:3} | {:12.6} | {:12.6} | {:12.6}",
            s.n, s.x, s.x_next, s.diff);
    }
    println!("Результат: {:.8}", steps.last().unwrap().x_next);
}

fn solve_bisection(a: f64, b: f64, eps: f64) {
    let mut history: Vec<BisectionStep> = Vec::new();
    let mut a = a;
    let mut b = b;
    let mut n = 0;
    
    while (b - a) > eps && n < 100 {
        let mid = (a + b) / 2.0;
        history.push(BisectionStep { n, a, b, mid });
        if f(a) * f(mid) < 0.0 {
            b = mid;
        } else {
            a = mid;
        }
        n += 1;
    }
    
    println!("\nМетод половинного деления");
    println!("  N |      a       |      b       |    b - a     |     x_mid    ");
    println!("------------------------------------------------------------------");
    for s in &history {
        println!("{:3} | {:12.6} | {:12.6} | {:12.6} | {:12.6}",
            s.n, s.a, s.b, s.b - s.a, s.mid);
    }
    
    let root = (a + b) / 2.0;
    println!("\nКорень: {:.8}", root);
    println!("f(x*) = {:.8}", f(root));
}

fn solve_newton(mut x: f64, eps: f64) {
    let mut history: Vec<StepData> = Vec::new();
    
    for n in 0..100 {
        let next = x - f(x) / df(x);
        let diff = (next - x).abs();
        history.push(StepData { n, x, x_next: next, diff });
        if diff < eps {
            break;
        }
        x = next;
    }
    
    print_table("Метод Ньютона", &history);
    println!("f(x*) = {:.8}", f(history.last().unwrap().x_next));
}

fn solve_iteration(mut x: f64, eps: f64) {
    // Проверка сходимости метода простых итераций
    let h = 1e-6;
    let phi_deriv = ((phi(x + h) - phi(x)) / h).abs();
    println!("\nПроверка сходимости");
    print!("|φ'(x0)| ≈ {:.10}", phi_deriv);
    if phi_deriv < 1.0 {
        println!(" < 1 условие сходимости выполняется");
    } else {
        println!(" ≥ 1 метод может расходиться");
    }
    
    let mut history: Vec<StepData> = Vec::new();
    
    for n in 0..100 {
        let next = phi(x);
        let diff = (next - x).abs();
        history.push(StepData { n, x, x_next: next, diff });
        if diff < eps {
            break;
        }
        x = next;
    }
    
    print_table("Метод итераций", &history);
    println!("f(x*) = {:.8}", f(history.last().unwrap().x_next));
}

fn main() {
    let eps = 1e-4;
    
    println!("Решение уравнения x^2 - 3sin(x) = 0");
    println!("Вариант 6");
    println!("========================================");
    println!("Интервал с корнем: [1, 2]");
    
    solve_bisection(1.0, 2.0, eps);
    solve_newton(1.8, eps);
    solve_iteration(1.5, eps);
}
