#include "matplotlib-cpp/matplotlibcpp.h"
#include <vector>
#include <cmath>

namespace plt = matplotlibcpp;

int main() {
    std::vector<double> x, y;

    for (double i = -20; i <= 20; i += 0.5) {
        x.push_back(i);
        y.push_back(i * i * i);  
    }

    // Plot
    plt::plot(x, y);
    plt::xlabel("x");
    plt::ylabel("y = x^2");
    plt::title("Parabola y = x²");
    plt::grid(true);
    plt::show();

    return 0;
}