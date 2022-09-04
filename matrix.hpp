#ifndef MATRIX_EXAMPLE_MATRIX_H
#define MATRIX_EXAMPLE_MATRIX_H
#include <iostream>
#include <type_traits>
#include <utility>
#include <functional>
#include <initializer_list>
#include <array>

namespace matrix_example {

  /**
     A class definition for an M-by-N matrix of numbers.
     
     A "number" is defined here as any data type with definitions for every 
     arthmetic operator and a zero-assignment operator, i.e. "number x = 0;".
     A number could be a POD integer or floating point number, an imaginary number, 
     a complex, a quarternion, or even an n-ternion, so long as it has definitions 
     for every arithmetic operator.
  */
  template<typename T=double, int M=3, int N=3> class matrix {
  public:
    matrix() {
      for (int i=0; i<M; ++i) {
	for (int j=0; j<N; ++j) {
	  arr[i][j] = static_cast<T>(0);
	}
      }
    }

    matrix(const std::array<T, M*N>& a) {
      for (int i=0; i < a.size(); ++i) {
	arr[i/3][i%3] = a[i];
      }
    }
    
    std::array<T, N>& operator[](int i) {
      return std::ref(arr[i]);
    }

    const std::array<T, N>& operator[](int i) const {
      return std::ref(arr[i]);
    }
    
    /// Scalar multiply
    matrix<T, M, N>& operator*=(T c) {
      for (int i=0; i<M; ++i) {
	for (int j=0; j<N; ++j) {
	  arr[i][j] *= c;
	}
      }
      return std::ref(*this);
    }

    constexpr int rows() const { return M; }
    constexpr int columns() const { return N; }

    /// Row reduce the matrix if it is reducible.
    matrix<T, M, N> reduce() const {
      assert(false && "TODO: implement matrix::reduce()");
      matrix<T, M, N> result;
      return result;
    }

    /// Compute the singular value decomposition of a matrix.
    T svd() const {
      assert(false && "TODO: implement matrix::svd()");
      T result = 0;
      return result;
    }

    /// Compute the nth power of the matrix using repeated squaring,
    /// (i.e. O(log(n)) matrix multiplications)
    matrix<T, M, N> pow(int n) const{
      assert(false && "TODO: implement matrix::pow()");
      matrix<T, M, N> result;
      return result;
    }
    
  private:
    std::array<std::array<T, N>, M> arr;
  };

  
  template<typename T, int M, int N> matrix<T, M, N>
  operator+(const matrix<T, M, N>& A, const matrix<T, M, N>& B) {
    matrix<T,M,N> out;
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	out = A[i][j] + B[i][j];
      }
    }
    return out;
  }
  
  template<typename T, int M, int N> matrix<T, M, N>
  operator-(const matrix<T, M, N>& A, const matrix<T, M, N>& B) {
    matrix<T, M, N> out;
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	out = A[i][j] - B[i][j];
      }
    }
    return out;
  }

  /**
     Multiply a matrix and a scalar
     @{
   */
  template<typename T, int M, int N> matrix<T, M, N>
  operator*(T c, const matrix<T, M, N>& A) {
    matrix<T, M, N> out;
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	out[i][j] = c*A[i][j];
      }
    }
    return out;
  }
  template<typename T, int M, int N> matrix<T, M, N>
  operator*(const matrix<T, M, N>& A, T c) {
    return c*A;
  }
  ///@}

  /// Multiply two matrices together.
  /// TODO: Use Strassen's algorithm for resultant matrices with more than 100 rows.
  template<typename T, int M1, int N1, int M2, int N2> matrix<T, M1, N2>
  operator*(const matrix<T, M1, N1>& A, const matrix<T, M2, N2>& B) {
    static_assert(N1 == M2, "matrix A must have the same number of columns as matrix B has rows");
    matrix<T, M1, N2> out;

    if (M1 > 100) {
      // Strassen's algorithm for big matrices
      assert(false && "TODO: Implement Strassen's algorithm for very big matrix multiplication.");
    } else {
      // Straightforward algorithm for small matrices
      for (int i=0; i < M1; ++i) {
	for (int j=0; j < N2; ++j) {
	  for (int k=0; k < N1; ++k) {
	    out[i][j] += A[i][k] * B[k][j];
	  }
	}
      }
    }
    return out;
  }

  template<typename T, int M, int N> static constexpr matrix<T, M, N> identity_matrix() {
    matrix<T,M,N> out;
    for (int i=0; i<M; ++i) {
      out[i][i] = static_cast<T>(1);
    }
    return out;
  }

  template<typename T, int M, int N> std::ostream& operator<<(std::ostream& os, const matrix<T, M, N>& A) {
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	os << A[i][j] << "\t";
      }
      os << std::endl;
    }
    return os;
  }
};

#endif // header guard
