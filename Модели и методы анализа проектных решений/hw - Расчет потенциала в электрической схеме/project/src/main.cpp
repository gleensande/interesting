#include "csv_parser.hpp"
#include "matrix.hpp"
#include <cmath>

#define RESULT_FILENAME "res/result.dat"

#define NUMBER_CAUSE 0
#define NORM_CAUSE 1

#define GO_NEWTON   10
#define GO_FORWARD  11

// сложение для словарей
map<string, double> operator+ (map<string, double> a, map<string, double> b) {
    map<string, double> c = a;

    for (auto e : c) {
        c[e.first] += b[e.first];
    }

    return c;
}

// вычитание для словарей
map<string, double> operator- (map<string, double> a, map<string, double> b) {
    map<string, double> c = a;

    for (auto e : c) {
        c[e.first] -= b[e.first];
    }

    return c;
}

// деление словаря на число
map<string, double> operator/ (map<string, double> a, double b) {
    map<string, double> c = a;

    for (auto e : c) {
        c[e.first] /= b;
    }

    return c;
}

// создание вектора невязок
Matrix<double> create_residual_vector(map<string, double>& C, map<string, double>& S, map<string, double>& pS, double dt) {
    Matrix<double> V(14, 1);

    V.set(0, 0, S["dUc1_dt"] - (S["Uc1"] - pS["Uc1"]) / dt);
    V.set(1, 0, S["dUc2_dt"] - (S["Uc2"] - pS["Uc2"]) / dt);
    V.set(2, 0, S["dUcb_dt"] - (S["Ucb"] - pS["Ucb"]) / dt);
    V.set(3, 0, S["dIl1_dt"] - (S["Il1"] - pS["Il1"]) / dt);
    V.set(4, 0, S["Uc1"] + S["phi_1"]);
    V.set(5, 0, S["Uc2"] - S["phi_3"]);
    V.set(6, 0, S["Ucb"] - S["phi_4"] + S["phi_5"]);
    V.set(7, 0, C["L1"] * S["dIl1_dt"] - S["phi_2"] + S["phi_3"]);
    V.set(8, 0, -C["C1"] * S["dUc1_dt"] - S["Ie1"]);
    V.set(9, 0, S["Ie1"] + S["Il1"]);
    V.set(10, 0, -S["Il1"] + C["C2"] * S["dUc2_dt"] + (S["phi_3"] - S["phi_4"]) / C["Rb"]);
    V.set(11, 0, -(S["phi_3"] - S["phi_4"]) / C["Rb"] + 
                C["Cb"] * S["dUcb_dt"] + 
                (S["phi_4"] - S["phi_5"]) / C["Ru"] +
                C["It"] * (exp(S["Ucb"] / C["MFt"]) - 1)
    );
    V.set(12, 0, -C["Cb"] * S["dUcb_dt"] + 
                -(S["phi_4"] - S["phi_5"]) / C["Ru"] +
                -C["It"] * (exp(S["Ucb"] / C["MFt"]) - 1) +
                S["phi_5"] / C["R1"]
    );
    V.set(13, 0, C["E"] - S["phi_2"] + S["phi_1"]);

    return V;
}

// создание матрицы Якоби
Matrix<double> create_Jacobi_matrix(map<string, double>& C, map<string, double>& S, double dt) {
    Matrix<double> J(14, 14);

    // единицы
    for (int i = 0; i < 7; i++) {
        J.set(i, i, 1);
    }
    J.set(4, 8, 1);
    J.set(6, 12, 1);
    J.set(7, 10, 1);
    J.set(9, 7, 1);
    J.set(9, 13, 1);
    J.set(13, 8, 1);


    // - единицы
    J.set(5, 10, -1);
    J.set(6, 11, -1);
    J.set(7, 9, -1);
    J.set(8, 13, -1);
    J.set(10, 7, -1);
    J.set(13, 9, -1);


    // -1 / dt
    for (int i = 0; i < 4; i++) {
        J.set(i, i + 4, -1 / dt);
    }
    

    // R
    J.set(10, 10, 1 / C["Rb"]);
    J.set(10, 11, -1 / C["Rb"]);
    J.set(11, 10, -1 / C["Rb"]);
    J.set(11, 11, 1 / C["Rb"] + 1 / C["Ru"]);
    J.set(11, 12, -1 / C["Ru"]);
    J.set(12, 11, -1 / C["Ru"]);
    J.set(12, 12, 1 / C["Ru"] + 1 / C["R1"]);


    // Diod
    const double dId_dt = C["It"] * exp(S["Ucb"] / C["MFt"]) / C["MFt"];
    J.set(11, 6, dId_dt);
    J.set(12, 6, -dId_dt);


    // L & C
    J.set(7, 3, C["L1"]);
    J.set(8, 0, -C["C1"]);
    J.set(10, 1, C["C2"]);
    J.set(11, 2, C["Cb"]);
    J.set(12, 2, -C["Cb"]);

    return J;
}

// расчет равномерной нормы вектора
double Inf_norm(Matrix<double>& v) {
    if (v.get_cols() != 1) {
        cout << "ОШИБКА: вектор имеет более одного столбца, а именно: " << v.get_cols() << endl;
        return -1;
    }

    double max_el = 0;
    for (int i = 0; i < v.get_rows(); i++) {
        if(v.get(i, 0) > max_el) {
            max_el = v.get(i, 0);
        }
    }

    return max_el;
}

// расчет равномерной нормы для словаря состояния
double Inf_norm(map<string, double>& v) {
    double max_el = 0;
    for (auto el : v) {
        if (el.first[0] != 'd') {
            if (el.second > max_el) {
                max_el = el.second;
            };
        }
    }


    return max_el;
}

// добавляет рассчитанные приращения к состоянию
map<string, double> append_d_to_state(map<string, double>& S, Matrix<double>& d) {
    map<string, double> S_new = S;

    S_new["dUc1_dt"] += d.get(0, 0);
    S_new["dUc2_dt"] += d.get(1, 0);
    S_new["dUcb_dt"] += d.get(2, 0);
    S_new["dIl1_dt"] += d.get(3, 0);
    S_new["Uc1"] += d.get(4, 0);
    S_new["Uc2"] += d.get(5, 0);
    S_new["Ucb"] += d.get(6, 0);
    S_new["Il1"] += d.get(7, 0);
    S_new["phi_1"] += d.get(8, 0);
    S_new["phi_2"] += d.get(9, 0);
    S_new["phi_3"] += d.get(10, 0);
    S_new["phi_4"] += d.get(11, 0);
    S_new["phi_5"] += d.get(12, 0);
    S_new["Ie1"] += d.get(13, 0);

    return S_new;
}

// расчет локальной точности
double calc_local_acc(
    map<string, double>& ppS, 
    map<string, double>& pS, 
    map<string, double>& S, 
    double pdt, 
    double dt
) {
    // создание матрицы с теми же полями,
    // обнуляем значения; погрешности для каждого элемента
    map<string, double> acr = S;
    for (auto e : acr) {
        acr[e.first] = 0;
    }

    // расчет погрешностей
    for (auto e : S) {
        if (e.first[0] == 'd') {
            continue;
        }

        string key = e.first;

        acr[key] = (S[key] - pS[key]) / dt;
        acr[key] += (pS[key] - ppS[key]) / pdt;
        acr[key] /= dt;

        acr[key] *= dt * dt / 2;
    }


    // выбор максимальной погрешности
    return Inf_norm(acr);
}

// реализация метода Ньютона
map<string, double> Newton(
    map<string, double>& C,
    map<string, double>& pS,
    map<string, double>& S,
    double dt,
    bool* stop_cause
) {
    double norm = C["eps"] + 1;
    map<string, double> S_new = S;
    int n = 0;      // счетчик на случай зацикливания метода

    while (norm > C["eps"] && n <= C["max_steps"]) {
        n++;

        Matrix<double> J = create_Jacobi_matrix(C, S_new, dt);
        Matrix<double> R = create_residual_vector(C, S_new, pS, dt);
        R.mul_num(-1);
        
        J.Gauss(&R);

        norm = Inf_norm(R);

        S_new = append_d_to_state(S_new, R);
    };

    if (norm <= C["eps"]) {
        *stop_cause = NORM_CAUSE;
    } else {
        *stop_cause = NUMBER_CAUSE;
    }

    return S_new;
}


int main() {
    // задание начального состояния 
    map<string, double> C = create_map_from_file("input/constants.dat");
    map<string, double> S = create_map_from_file("input/initial_state.dat");
    map<string, double> ppS = S, pS = S, nS; // предпред-, пред- и следующий шаги
    
    double t = 0, dt = C["dt_start"], pdt = dt;

    bool stop_cause = NORM_CAUSE;


    // открытие файла и запись в этот файл начального состояния
    std::ofstream result_file;
    result_file.open(RESULT_FILENAME);
    if (!result_file) {
        cout << "ОШИБКА: Невозможно открыть файл для записи результатов: " << RESULT_FILENAME << endl;
        return -1;
    }


    // расчет на каждом временном шаге
    int step_num = 0;   // заход в цикл
    int where_to_go = GO_NEWTON;
    while (t <= C["T"]) {
        // записываем в файл только на нулевой итерации
        // или когда переходим к следующему шагу по времени
        result_file << t << "\t" << nS["phi_5"] << endl;


        // на разгонных шагах цикла выполняем только
        // метод Ньютона
        if (step_num < C["racing_steps"]) {

            do {
                // метод Ньютона, пока итераций не слишком много, либо до нормы
                nS = Newton(C, pS, S, dt, &stop_cause);

                // выясняем, почему метод Ньютона остановился и изменяем шаг и время в зависимости от этого
                if (stop_cause == NORM_CAUSE) {
                } else if (stop_cause == NUMBER_CAUSE) {
                    dt /= 2;
                }
            } while (stop_cause != NORM_CAUSE);
            
            // предсказание метода Ньютона
            S = nS + (nS - pS) / (pdt / dt);

            // переходим к следующему шагу по времени
            ppS = pS;
            pS = nS;
            t += dt;
            pdt = dt;
            step_num++;
            
        } else {

            // когда разгонные шаги прекращаются, начинаем анализировать
            // длину шага по времени и менять ее
            do {
                // метод Ньютона, пока итераций не слишком много, либо до нормы
                nS = Newton(C, pS, S, dt, &stop_cause);

                // выясняем, почему метод Ньютона остановился и изменяем шаг и время в зависимости от этого
                if (stop_cause == NORM_CAUSE) {
                    double acr = calc_local_acc(ppS, pS, nS, pdt, dt);

                    if (acr < C["xi_1"]) {
                        t += dt;
                        dt *= 2;
                        where_to_go = GO_FORWARD;
                    } else if (acr < C["xi_2"]) {
                        t += dt;
                        where_to_go = GO_FORWARD;
                    } else {
                        dt /= 2;
                    }
                } else if (stop_cause == NUMBER_CAUSE) {
                    dt /= 2;
                }
            } while (where_to_go != GO_FORWARD); 


            // Проверка: не стал ли шаг слишком маленьким
            if (dt < C["dt_min"]) {
                cout << "ОШИБКА: Шаг сократился слишком сильно: " << endl;
                cout << "dt = " << dt << endl;
                cout << "dt_min = " << C["dt_min"] << endl;
                return -1;
            }

            // предсказание метода Ньютона
            S = nS + (nS - pS) / (pdt / dt);

            // переход к следующему шагу
            ppS = pS;
            pS = nS;
            pdt = dt;
            step_num++;
        }
    }

    result_file.close();
    
    return 0;
}
