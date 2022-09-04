#ifndef MATRIX_EXAMPLE_COMPLEX_H
#define MATRIX_EXAMPLE_COMPLEX_H
#include <iostream>

namespace matrix_example {
  template<typename T=double> struct complex {
    T real, imaginary;
    
    complex() : real(static_cast<T>(0)), imaginary(static_cast<T>(0)) { }
    complex(T r) : real(r), imaginary(static_cast<T>(0)) { }
    complex(T r, T i) : real(r), imaginary(i) { }
    
    complex<T>& operator=(T r) {
      real = r;
      imaginary = static_cast<T>(0);
      
      return std::ref(*this);
    }
    
    complex<T>& operator+=(complex<T> b) {
      real += b.real;
      imaginary += b.imaginary;
      return std::ref(*this);
    }
    
    complex<T>& operator-=(complex<T> b) {
      real -= b.real;
      imaginary -= b.imaginary;
      return std::ref(*this);
    }
    
    complex<T>& operator*=(complex<T> b) {
      real *= b.real;
      imaginary *= b.imaginary;
      return std::ref(*this);
    }
  };
  
  template<typename T1, typename T2> bool operator==(complex<T1> a, T2 c) {
    return (a.real == c && a.imaginary == 0);
  }

  template<typename T1, typename T2> bool operator==(T1 c, complex<T2> a) {
    return (a.real == c && a.imaginary == 0);
  }
  
  template<typename T1, typename T2> bool operator==(complex<T1> a, complex<T2> b) {
    return (a.real == b.real && a.imaginary == b.imaginary);
  }

  template<typename T> complex<T> operator*(T a, complex<T> b) {
    return complex(a*b.real, a*b.real);
  }

  template<typename T> complex<T> operator*(complex<T> a, T b) {
    return b*a;
  }

  template<typename T> complex<T> operator*(complex<T> a, complex<T> b) {
    return complex(a.real*b.real - a.imaginary*b.imaginary,
		   a.real*b.imaginary + a.imaginary*b.real);
  }
  
  template<typename T> complex<T> operator+(complex<T> a, complex<T> b) {
    return complex(a.real+b.real, a.imaginary+b.imaginary);
  }
  
  template<typename T> complex<T> operator-(complex<T> a, complex<T> b) {
    return complex(a.real-b.real, a.imaginary-b.imaginary);
  }
  
  template<typename T> std::ostream& operator<<(std::ostream& os, complex<T> c) {
    return os << c.real << "+" << c.imaginary << "i";
  }
};
#endif
