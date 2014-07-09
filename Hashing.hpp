#ifndef HASHING_HPP
#define HASHING_HPP

#include <stdlib.h>
#include <math.h>

class HashingMethod{
public:
  virtual long int hash(const long int m, const long int k) const = 0;
  long int operator()(const long int m, const long int k) const{
    return this->hash(m,k);
  }
  virtual ~HashingMethod(){}
};

class DivisionMethod: public HashingMethod{
public:
  inline long int hash(long int m, long int k) const{
    return std::abs(k % m);
  }
};

class KnuthDivisionMethod: public HashingMethod{
public:
  inline long int hash(long int m, long int k) const{
    return std::abs((k*(k+3))%m);
  }
};

class MultiplicationMethod: public HashingMethod{
private:
  double a;
public:
  MultiplicationMethod() : a{double(rand())/RAND_MAX}{
  }
  inline long int hash(long int m, long int k) const{
    //w = k in bits, r = m in bits
    int w = !k ? 1 : log2(k), r = !m ? 1 : log2(m);
    //A in [2^(w-1), 2^w]
    long int long A = a * (pow(2, w) - pow(2,w-1)) + pow(2,w-1);
    return std::abs(((A*k)% int(pow(2, w))) >> (w-r));
  }
};

class UniversalMethod: public HashingMethod{
private:
  static const long int PRETTY_BIG_PRIME_NUMBER = 104729;
  long int p, a, b;
public:
  UniversalMethod(): p{PRETTY_BIG_PRIME_NUMBER}, a(double(rand())/RAND_MAX*p), b(double(rand())/RAND_MAX*p){
  }
  inline long int hash(long int m, long int k) const{
    return std::abs(((a*k+b)%p)%m);
  }
};

#endif // HASHING_HPP
