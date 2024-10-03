#include <stdio.h>

#include "polyfit.h"

double x[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
//double x[] = {0.0, 1.0, 2.0, 3.0};

int test_cubic(void) {
    int m = sizeof(x)/sizeof(x[0]);
    int nerrors = 0;
    double y[sizeof(x)/sizeof(x[0])] = {0.0};
    double pars[4] = {0.3, -0.25, 1.5, -0.5};
    double pars_out[4] = {0.0};
    int n = sizeof(pars) / sizeof(pars[0]) - 1;

    polyval(n, pars, m, x, y);
#ifndef NDEBUG
    printf("y values (truth):\n");
    print_matrix(1, m, y);
#endif

#ifndef NDEBUG
    printf("parameters before:\n");
    print_matrix(1, n + 1, pars);
#endif

    polyfit(m, x, y, n, pars_out);

    return nerrors;
}

int main(void) {
    test_cubic();
    return 0;
}