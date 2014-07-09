#ifndef HASHING_HPP
#define HASHING_HPP

#include <stdlib.h>
#include <math.h>

class HashingMethod{
public:
  virtual unsigned long hash(const unsigned long m, const unsigned long k) const = 0;
  unsigned long operator()(const unsigned long m, const unsigned long k) const{
    return this->hash(m,k);
  }
};

class DivisionMethod: public HashingMethod{
public:
  inline unsigned long hash(unsigned long m, unsigned long k) const{
    return k % m;
  }
};

class KnuthDivisionMethod: public HashingMethod{
public:
  inline unsigned long hash(unsigned long m, unsigned long k) const{
    return (k*(k+3))%m;
  }
};

class MultiplicationMethod: public HashingMethod{
private:
  double a;
public:
  MultiplicationMethod() : a{double(rand())/RAND_MAX}{
  }
  inline unsigned long hash(unsigned long m, unsigned long k) const{
    //w = k in bits, r = m in bits
    int w = !k ? 1 : log2(k), r = !m ? 1 : log2(m);
    //A in [2^(w-1), 2^w]
    unsigned long long A = a * (pow(2, w) - pow(2,w-1)) + pow(2,w-1);
    return ((A*k)% int(pow(2, w))) >> (w-r);
  }
};

class UniversalMethod: public HashingMethod{
private:
  static const unsigned long PRETTY_BIG_PRIME_NUMBER = 104729;
  unsigned long p, a, b;
public:
  UniversalMethod(): p{PRETTY_BIG_PRIME_NUMBER}, a(double(rand())/RAND_MAX*p), b(double(rand())/RAND_MAX*p){
  }
  inline unsigned long hash(unsigned long m, unsigned long k) const{
    return ((a*k+b)%p)%m;
  }
};

#endif // HASHING_HPP
