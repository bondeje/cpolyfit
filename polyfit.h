#ifndef NDEBUG
void print_matrix(int n, int m, double const * A);
#endif

int polyval(int n, double const * __restrict pars, int m, double const * __restrict x, double * __restrict y /* out */);
int polyfit_w(int m, double const * __restrict x, double const * __restrict y, int n, double * __restrict pars /* out */, double * __restrict ws);
int polyfit(int m, double const * __restrict x, double * __restrict y, int n, double * __restrict pars /* out */);