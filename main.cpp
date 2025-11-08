#include <iostream>
#include <cmath>
#include <vector>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

const double k_g = 0.1;       // Tumor growth rate constant
const double T_max = 1.0;     // Carrying capacity
const double k_d = 0.04;       // Drug kill coefficient
const double d = 0.1;        // Clearance rate of dead cells


struct State {
    double S;
    double D;
};

double Exposure(double t) {
    return 30.0 * pow(t, 0.5) / (pow(100.0, 0.5) + pow(t, 0.5));
}

double f(double S) {
    return k_g * S;//exponential growth function
    //return k_g * T * (1.0 - T / T_max); //Logistic growth function
}

// ODEs
State derivatives(double t, const State &y) {
    double S = y.S;
    double D = y.D;
    double exp = Exposure(t);

    State dydt;
    dydt.S = f(S) - k_d * exp * S;
    dydt.D = k_d * exp * S - d * D;
    return dydt;
}

// runge kutta
State rk4_step(double t, double dt, const State &y) {
    State k1 = derivatives(t, y);
    State k2 = derivatives(t + dt / 2.0, {y.S + dt * k1.S / 2.0, y.D + dt * k1.D / 2.0});
    State k3 = derivatives(t + dt / 2.0, {y.S + dt * k2.S / 2.0, y.D + dt * k2.D / 2.0});
    State k4 = derivatives(t + dt, {y.S + dt * k3.S, y.D + dt * k3.D});

    State y_next;
    y_next.S = y.S + (dt / 6.0) * (k1.S + 2 * k2.S + 2 * k3.S + k4.S);
    y_next.D = y.D + (dt / 6.0) * (k1.D + 2 * k2.D + 2 * k3.D + k4.D);
    return y_next;
}

int main() {
    double t0 = 0.0, t_end = 50.0, dt = 0.1;

    State y;
    y.S = 30;  // initial viable cells
    y.D = 0.0;  // initial dead cells

    std::vector<double> time_values, S_values, D_values, T_values;

    for (double t = t0; t <= t_end; t += dt) {
        double T = y.S + y.D;
        time_values.push_back(t);
        S_values.push_back(y.S);
        D_values.push_back(y.D);
        T_values.push_back(T);

        y = rk4_step(t, dt, y);
    }


    plt::figure_size(800, 600);
    //plt::plot(time_values, S_values, {{"label", "S (viable cells)"}});
    //plt::plot(time_values, D_values, {{"label", "D (damaged cells)"}});
    plt::plot(time_values, T_values, {{"label", "T (total tumor size)"}});

    plt::xlabel("Time");
    plt::ylabel("Cell Population");
    plt::title("Tumor Growth and Drug Response Model");
    plt::legend();
    plt::grid(true);
    plt::show();

    return 0;
}