#include <cassert>
#include <iostream>
#include "matrix.hpp"

using namespace matrix_example;

using complex = std::complex<double>;

void matrix_test() {
  matrix<complex,3,3> identity3 = identity_matrix<complex,3>();
  
  matrix<complex,3,3> A;
  for (int i=0; i<A.rows(); ++i) {
    for (int j=0; j<A.columns(); ++j) {
      if ((i+j)%2 == 0) {
	A[i][j] = complex(1.0, 0.0);
      } else {
	A[i][j] = complex(0.0, 1.0);
      }
    }
  }
  
  matrix<complex,3,3> B = complex(2.0)*identity3;
  std::cout << B*B << std::endl;
  
  matrix<complex,3,3> C = A*A;
  std::cout << C << std::endl;
}

int main(int argc, char** argv) {
  matrix_test();
  return 0;
}
