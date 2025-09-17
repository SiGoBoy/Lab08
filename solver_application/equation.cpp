#include <iostream>
#include "formatter_ex.h"
#include "solver.h"

int main() {
    float a, b, c;
    std::cout << "Enter coefficients a, b, c: ";
    std::cin >> a >> b >> c;

    float x1, x2;
    solve(a, b, c, x1, x2);

    std::cout << "Roots: x1 = " << x1 << ",
