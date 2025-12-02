#include <cmath>
#include <iostream>
#include <vector>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// Model constants
const double k_g = 0.1;   // Tumor growth rate constant
const double k_d = 0.04;   // drug kill coefficient
const double d   = 0.10;   // dead-cell clearance rate
const double e_max = 30.0; // maximum exposure level
const double T_max = 120.0; // carrying capacity
const bool simulaute_treatment_end = false;
const double treatment_end_time = 30.0;

// Exposure function 
double exposure(double t) {
    if (simulaute_treatment_end && t >= treatment_end_time){
        return 0.0;
    }
    //hills equation
    return e_max * std::pow(t, 0.5) /
           (std::pow(100.0, 0.5) + std::pow(t, 0.5));
}

struct State {
    double S1;
    double S2;
    double D;
};

double logisticGrowth(double S1) {
    return k_g * S1 * (1.0 - S1 / T_max);
}
// Exponential growth
double exponentialGrowth(double S1) {
    return k_g * S1;
}

// ODEs
State derivatives(double t, const State& y) {
    double S1 = y.S1;
    double S2 = y.S2;
    double D  = y.D;
    double exp = exposure(t);

    State dydt;
    //dydt.S1 = logisticGrowth(S1) - k_d * exp * S1;
    dydt.S1 = logisticGrowth(S1) - k_d * exp * S1;

    dydt.S2 = k_d * exp * S1 - k_d * S2;

    dydt.D  = k_d * S2 - d * D;

    return dydt;
}

// Runge-Kutta 4th order step
State rk4_step(double t, double dt, const State& y) {
    State k1 = derivatives(t, y);

    State k2 = derivatives(t + dt/2.0, {
        y.S1 + dt * k1.S1 / 2.0,
        y.S2 + dt * k1.S2 / 2.0,
        y.D  + dt * k1.D  / 2.0
    });

    State k3 = derivatives(t + dt/2.0, {
        y.S1 + dt * k2.S1 / 2.0,
        y.S2 + dt * k2.S2 / 2.0,
        y.D  + dt * k2.D  / 2.0
    });

    State k4 = derivatives(t + dt, {
        y.S1 + dt * k3.S1,
        y.S2 + dt * k3.S2,
        y.D  + dt * k3.D
    });

    State next;
    next.S1 = y.S1 + (dt/6.0) * (k1.S1 + 2*k2.S1 + 2*k3.S1 + k4.S1);
    next.S2 = y.S2 + (dt/6.0) * (k1.S2 + 2*k2.S2 + 2*k3.S2 + k4.S2);
    next.D  = y.D  + (dt/6.0) * (k1.D  + 2*k2.D  + 2*k3.D  + k4.D );

    return next;
}

int main() {
    double t0 = 0.0, t_end = 100.0, dt = 0.1;

    // Initial conditions
    State y;
    y.S1 = 30.0;
    y.S2 = 0.0;
    y.D  = 0.0;

    std::vector<double> time_values, S1_values, S2_values, D_values, T_values;

    for (double t = t0; t <= t_end; t += dt) {
        double T = y.S1 + y.S2;

        time_values.push_back(t);
        S1_values.push_back(y.S1);
        S2_values.push_back(y.S2);
        D_values.push_back(y.D);
        T_values.push_back(T);

        y = rk4_step(t, dt, y);
    }

    // Plotting
    plt::figure_size(900, 600);

    plt::plot(time_values, S1_values, {{"label", "S1 (Viable Cells)"}, { "color", "green"}});
    plt::plot(time_values, S2_values, {{"label", "S2 (Affected Cells)"}, { "color", "orange"}});
    plt::plot(time_values, D_values,  {{"label", "D (Dying/Dead Cells)"}, { "color", "red"}});
    plt::plot(time_values, T_values,  {{"label", "T = S1 + S2"}, { "color", "blue"}});
    
    plt::xlabel("Time");
    plt::ylabel("Tumor Burden");
    plt::title("Tumor Growth Model");
    plt::legend();
    plt::grid(true);
    
    plt::show();
    
    return 0;
}