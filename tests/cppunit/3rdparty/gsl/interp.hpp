#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

//#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
//#include <vector>

TEST(gsl, interp_spline) {
  constexpr int n = 10;
  double x[n], y[n];

  printf("#m=0,S=17\n");
  for (int i = 0; i < n; i++) {
    x[i] = i + 0.5 * sin(i);
    y[i] = i + cos(i * i);
    printf("%g %g\n", x[i], y[i]);
  }
  printf("#m=1,S=0\n");

  gsl_interp_accel* acc = gsl_interp_accel_alloc();
  gsl_spline* spline = gsl_spline_alloc(gsl_interp_cspline, n);

  gsl_spline_init(spline, x, y, n);

  for (double xi = x[0]; xi < x[9]; xi += 0.5) {
    double yi = gsl_spline_eval(spline, xi, acc);
    printf("%g %g\n", xi, yi);
  }

  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);
}