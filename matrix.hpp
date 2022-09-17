#ifndef MATRIX_EXAMPLE_MATRIX_H
#define MATRIX_EXAMPLE_MATRIX_H
#include <array>
#include <complex>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <type_traits>
#include <utility>

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
    constexpr matrix() {
      for (int i=0; i<M; ++i) {
	for (int j=0; j<N; ++j) {
	  arr[i][j] = zero;
	}
      }
    }

    constexpr int rows() const { return M; }
    constexpr int columns() const { return N; }

    /// @return A row vector of the ith row, can be chained with the
    /// std::array bracket operator to index individual elements.
    /// @{
    constexpr std::array<T, N>& operator[](int i) {
      return std::ref(arr[i]);
    }
    
    constexpr const std::array<T, N>& operator[](int i) const {
      return std::ref(arr[i]);
    }
    /*@}*/
    
    /// Scalar multiply
    constexpr matrix<T, M, N>& operator*=(T c) {
      for (int i=0; i<M; ++i) {
	for (int j=0; j<N; ++j) {
	  arr[i][j] *= c;
	}
      }
      return std::ref(*this);
    }

    /// Reduce to row-echelon form in-place.
    /// @note The current implementation uses Gaussian elimination.
    /// @warning { The current implementation works only for real numbers. A partial template
    /// specialization must be made for complex numbers. }
    /// @todo Implement support for complex numbers by multiplying by the complex conjugate.
    void reduce() {
      int pivot_row = 0;
      int pivot_col = 0;

      // Sort by leading zeros first.
      std::sort(arr.begin(), arr.end(), matrix::row_compare_leading_zeros);
      
      while (pivot_row < M-1 && pivot_col < N) {
	if (arr[pivot_row][pivot_col] == static_cast<T>(0)) {
	  ++pivot_col;
	  continue;
	}
	
	for (int i=pivot_row+1; i<M; ++i) {
	  int factor = arr[i][pivot_col]/arr[pivot_row][pivot_col];
	  for (int j=pivot_col; j<N; ++j) {
	    arr[i][j] -= arr[pivot_row][j]*factor;
	  }
	}
	
	++pivot_row;
	++pivot_col;
      }
    }
    
  private:
    // Compares number of leading zeros between two row vectors.
    // Returns whichever row has the most leading zeros, with the tie going to a.
    static constexpr bool row_compare_leading_zeros(const std::array<T,N>& a,
						    const std::array<T,N>& b) {
      for (int j=0; j<N; ++j) {
	if (b[j] != zero) {
	  return true;
	} else if (a[j] != zero) {
	  return false;
	}
      }
    }

    constexpr void row_swap(int i, int j) {
      std::swap(arr[i], arr[j]);
    }
    
    // Because the array sizes are known at compile time and identical for
    // each row, this structure will always have ideal spatial locality.
    std::array<std::array<T, N>, M> arr;
    static constexpr T zero = static_cast<T>(0);
  };
  
  
  template<typename T, int M, int N> matrix<T, M, N>
  constexpr operator+(const matrix<T, M, N>& A, const matrix<T, M, N>& B) {
    matrix<T,M,N> out;
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	out = A[i][j] + B[i][j];
      }
    }
    return out;
  }
  
  template<typename T, int M, int N> matrix<T, M, N>
  constexpr operator-(const matrix<T, M, N>& A, const matrix<T, M, N>& B) {
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
  constexpr operator*(T c, const matrix<T, M, N>& A) {
    matrix<T, M, N> out;
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	out[i][j] = c*A[i][j];
      }
    }
    return out;
  }
  template<typename T, int M, int N> matrix<T, M, N>
  constexpr operator*(const matrix<T, M, N>& A, T c) {
    return c*A;
  }
  ///@}

  /// Multiply two matrices together.
  template<typename T, int M1, int N1, int M2, int N2> matrix<T, M1, N2>
  constexpr operator*(const matrix<T, M1, N1>& A, const matrix<T, M2, N2>& B) {
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

  /// Return a row-reduced copy of this matrix.
  template<typename T, int M, int N> constexpr matrix<T, M, N> reduce(const matrix<T, M, N>& A){
    matrix<T, M, N> result(A);
    result.reduce();
    return result;
  }

  /// Return a transposed copy of this matrix.
  template<typename T, int M, int N> constexpr matrix<T, N, M> transpose(const matrix<T, M, N>& A){
    matrix<T, N, M> result;
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	result[j][i] = A[i][j];
      }
    }
    return result;
  }

  /// Return an exponentiated copy of the user-specified NxN matrix.
  /// @precondition n > 0, behavior is undefined if n is negative or zero.
  /// @note { pow(A, 0) is the NxN identity matrix, but this trivial case is
  /// disregarded to save 1 branch instruction. }
  template<typename T, int N> constexpr matrix<T, N, N> pow(const matrix<T, N, N>& A, int n) {
    if (n < 2) {
      return A;
    }
    
    if ((n & 1) > 0) { // n is odd
      return A * pow(A, n/2) * pow(A, n/2);
    } else { 
      return pow(A, n/2) * pow(A, n/2);
    }
  }

  /// Return an NxN identity matrix.
  template<typename T, int N> constexpr matrix<T, N, N> identity_matrix() {
    matrix<T,N,N> out;
    for (int i=0; i<N; ++i) {
      out[i][i] = static_cast<T>(1);
    }
    return out;
  }

  // Write the matrix out as a CSV; simple and practical.
  template<typename T, int M, int N> std::ostream& operator<<(std::ostream& os, const matrix<T, M, N>& A) {
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	os << A[i][j] << ",";
      }
      os << std::endl;
    }
    return os;
  }
};

#endif // header guard
