#include <stdlib.h>

#include "polyfit.h"

#define index_2D(i, j, jmax) ((i) * (jmax) + (j))

#ifndef NDEBUG
#include <stdio.h>
void print_matrix(int n, int m, double const * A) {
    int i, j;
    for (i = 0; i < n; i++) {
        printf("\t");
        for (j = 0; j < m - 1; j++) {
            printf("%.6lf ", A[index_2D(i, j, m)]);
        }
        printf("%.6lf\n", A[index_2D(i, j, m)]);
    }
}
#endif

// n is the number of parameters
// requires n * (n + 1) + m workspace
void build_A_matrix(int n, int m, double const * __restrict x, double const * __restrict y, double ** A_, double * __restrict * __restrict ws) {
    double * scratch, * A;
    double sum;
    int i, j, N = n + 1;
    A = *ws;
    *ws += n * (n + 1); // A is permanently removing from workspace
    scratch = *ws; // scratch for calculation. must be at least m in length

    // initialize final column
    for (i = 0; i < n; i++) {
        A[index_2D(i, n, N)] = 0.0;
    }

    j = 0;
    for (i = 0; i < m; i++) {
        scratch[i] = 1.0;
        A[index_2D(j, n, N)] -= y[i]/* * scratch[i]*/;
    }
    A[index_2D(j, j, n + 1)] = m;

    for (j = 1; j < n; j++) {
        sum = 0.0;
        for (i = 0; i < m; i++) {
            scratch[i] *= x[i];
            sum += scratch[i];
            A[index_2D(j, n, N)] -= y[i] * scratch[i];
        }

        for (i = 0; i <= j; i++) {
            A[index_2D(j - i, i, N)] = sum;
        }
    }

    for (j = n; j < 2 * n - 1; j++) {
        sum = 0.0;
        for (i = 0; i < m; i++) {
            scratch[i] *= x[i];
            sum += scratch[i];
        }

        for (i = 0; i < 2 * n - 1 - j; i++) {
            A[index_2D(n - 1 - i, j - (n - 1) + i, N)] = sum;
        }
    }

    // set A to output
    *A_ = A;
}

// A is an n * (n + 1) consisting of [ X^TX | X^Ty ] where X^TX is a square (n) symmetric invertible matrix (provided all x are unique) and X^Ty is 1xn
void GJ_elim(int n, double * A) {
    double mult;
    int i, j, k, N = n + 1;
    i = 0;
    mult = A[0];
    for (j = 0; j < N; j++) {
        A[index_2D(i, j, N)] /= mult;
    }
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            mult = A[index_2D(i, j, N)];
            for (k = 0; k < N; k++) {
                A[index_2D(i, k, N)] -= mult * A[index_2D(j, k, N)];
            }
        }
        mult = A[index_2D(i, j, N)];
        for (j = i; j < N; j++) {
            A[index_2D(i, j, N)] /= mult;
        }
    }

    // A should be upper triangular now

    // remove above diagonal
    for (i = n - 2; i >= 0; i--) {
        for (j = i + 1; j < n; j++) {
            A[index_2D(i, n, N)] -= A[index_2D(i, j, N)] * A[index_2D(j, n, N)];
            A[index_2D(i, j, N)] = 0.0;
        }
    }
}

// gauss elimination. if NULL is passed to ws, returns the size of the workspace needed, 
// n is the order of the polynomial. pars is of length n + 1
int polyfit_w(int m, double const * __restrict x, double const * __restrict y, int n, double * __restrict pars /* out */, double * __restrict ws) {
    double * A;
    int i;
    n++; // n is now the number of pars
    // build matrix. permanently consumes n * (n + 1) and temporarily requires m workspace
    if (!ws) {
        return n * (n + 1) + m;
    }
    build_A_matrix(n, m, x, y, &A, &ws);

    GJ_elim(n, A);

    i = 0;
    while (i < n) {
        pars[i] = -A[index_2D(n - 1 - i, n, n + 1)];
        i++;
    }

#ifndef NDEBUG
    printf("parameters:\n");
    print_matrix(1, n, pars);
#endif

    return 0;
}

// n is the order of the polynomial. pars is of length n + 1
int polyfit(int m, double const * __restrict x, double * __restrict y, int n, double * __restrict pars /* out */) {
    double * ws;
    int status = 0, nws = polyfit_w(m, NULL, NULL, n, NULL, NULL);
    ws = malloc(nws * sizeof(double));

    status = polyfit_w(m, x, y, n, pars, ws);

    free(ws);

    return status;
}

int polyval(int n, double const * __restrict pars, int m, double const * __restrict x, double * __restrict y /* out */) {
    int i;
    n++;
    while (m--) {
        y[m] = 0.0;
        for (i = 0; i < n; i++) { 
            y[m] = y[m] * x[m] + pars[i];
        }
    }
    return 0;
}

