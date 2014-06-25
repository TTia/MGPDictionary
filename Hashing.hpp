#ifndef HASHING_HPP
#define HASHING_HPP

#include <stdlib.h>
#include <math.h>

/*!
 * \brief The Hashing class
 * Data l'impronta hash della chiave e la dimensione della tabella, viene calcolata
 * la normalizzazione.
 */
class Hashing{
public:
  virtual unsigned long hash(const unsigned long m, const unsigned long k) const = 0;
  unsigned long operator()(const unsigned long m, const unsigned long k) const{
    return this->hash(m,k);
  }
};

class DivisionMethod: public Hashing{
public:
  inline unsigned long hash(unsigned long m, unsigned long k) const{
    return k % m;
  }
};

class KnuthDivisionMethod: public Hashing{
public:
  inline unsigned long hash(unsigned long m, unsigned long k) const{
    return (k*(k+3))%m;
  }
};

class MultiplicationMethod: public Hashing{
private:
  unsigned long long s;
public:
  MultiplicationMethod(): s{(unsigned long long)floor(0.5*(sqrt(5)-1) * pow(2,32))}{}

  inline unsigned long hash(unsigned long m, unsigned long k) const{
    unsigned long long p = log2(m);
    return k*s >> (32-p);
  }
};

class UniversalHashing: public Hashing{
private:
  double a, b;
public:
  UniversalHashing(): a{double(rand())/RAND_MAX}, b{double(rand())/RAND_MAX}{

  }
  inline unsigned long hash(unsigned long m, unsigned long b) const{
    return 0;
  }
};

#endif // HASHING_HPP
