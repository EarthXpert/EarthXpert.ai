#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <gsl/gsl_fft_real.h>

//#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
//#include <vector>

TEST(gsl, fft_real) {
  constexpr int n = 100;
  double data[n]{0.0};

  int i;
  for (i = n / 3; i < 2 * n / 3; i++) {
    data[i] = 1.0;
  }
  for (i = 0; i < n; i++) {
    std::cout << i << ":\t" << data[i] << std::endl;
  }
  std::cout << std::endl;

  gsl_fft_real_workspace* work = gsl_fft_real_workspace_alloc(n);
  gsl_fft_real_wavetable* real = gsl_fft_real_wavetable_alloc(n);
  gsl_fft_real_transform(data, 1, n, real, work);
  gsl_fft_real_wavetable_free(real);

  for (i = 11; i < n; i++) {
    data[i] = 0;
  }

  gsl_fft_halfcomplex_wavetable* hc = gsl_fft_halfcomplex_wavetable_alloc(n);
  gsl_fft_halfcomplex_inverse(data, 1, n, hc, work);
  gsl_fft_halfcomplex_wavetable_free(hc);

  for (i = 0; i < n; i++) {
    std::cout << i << ":\t" << data[i] << std::endl;
  }

  gsl_fft_real_workspace_free(work);
}

#define REAL(z,i) ((z)[2*(i)])
#define IMAG(z,i) ((z)[2*(i)+1])

TEST(gsl, fft_complex) {
  constexpr int n = 128;
  double data[2 * n];

  int i; 
  for (i = 0; i < n; i++) {
    REAL(data, i) = 0.0; IMAG(data, i) = 0.0;
  }
  REAL(data, 0) = 1.0;

  for (i = 1; i <= 10; i++) {
    REAL(data, i) = REAL(data, 128 - i) = 1.0;
  }

  for (i = 0; i < n; i++) {
    printf("%d %e %e\n", i, REAL(data, i), IMAG(data, i));
  }
  printf("\n");

  gsl_fft_complex_radix2_forward(data, 1, n);

  for (i = 0; i < n; i++) {
    printf("%d %e %e\n", i, REAL(data, i) / sqrt(n), IMAG(data, i) / sqrt(n));
  }
}
