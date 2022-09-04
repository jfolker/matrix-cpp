#include <cassert>
#include <iostream>
#include "complex.hpp"
#include "matrix.hpp"

using namespace matrix_example;

void complex_test() {
  complex c{1.0,1.0};
  complex c2{1,1};
  assert(c == c2);
  complex c3 = 3.0*c;
  assert(c3.real == 3 && c3.imaginary == 3);
  complex c4 = c*c;
  assert(c4.real == 0 && c4.imaginary == 2);
}

void matrix_test() {
  matrix<complex<double>,3,3> identity3 = identity_matrix<complex<double>,3,3>();
  
  matrix<complex<double>,3,3> A;
  for (int i=0; i<A.rows(); ++i) {
    for (int j=0; j<A.columns(); ++j) {
      if ((i+j)%2 == 0) {
	A[i][j] = complex<double>(1.0, 0.0);
      } else {
	A[i][j] = complex<double>(0.0, 1.0);
      }
    }
  }
  
  matrix<complex<double>,3,3> B = complex(2.0)*identity3;
  std::cout << B*B << std::endl;
  
  matrix<complex<double>,3,3> C = A*A;
  std::cout << C << std::endl;
}

int main(int argc, char** argv) {
  complex_test();
  matrix_test();
  return 0;
}
