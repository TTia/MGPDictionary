#ifndef PROBING_HPP
#define PROBING_HPP

#include "Hashing.hpp"

class ProbingMethod{
public:
  ProbingMethod(): hm{new UniversalMethod()}{}
  ProbingMethod(HashingMethod hm): hm{&hm}{}
  virtual unsigned long probe(unsigned long i, unsigned long m, unsigned long k) const = 0;
  unsigned long operator()(unsigned long i, const unsigned long m, const unsigned long k) const{
    return this->probe(i,m,k);
  }
protected:
  virtual unsigned long hash(const unsigned long m, const unsigned long k) const{
    return (*hm)(m,k);
  }
private:
  HashingMethod* hm;
};

class LinearProbing: public ProbingMethod{
public:
  inline unsigned long probe(unsigned long i, unsigned long m, unsigned long k) const{
    //h(k,i) = (h'(k)+i)%m
    return (hash(m, k)+i)%m;
  }
};

class QuadraticProbing: public ProbingMethod{
public:
  inline unsigned long probe(unsigned long i, unsigned long m, unsigned long k) const{
    //h(k, i) = (h'(k) + i^2)%m
    return (hash(m, k) + i*i)%m;
  }
};

class DoubleHashing: public ProbingMethod{
public:
  DoubleHashing(): ProbingMethod(), h2{new UniversalMethod()}{}
  DoubleHashing(HashingMethod h1, HashingMethod h2): ProbingMethod(h1), h2{&h2}{}
  inline unsigned long probe(unsigned long i, unsigned long m, unsigned long k) const{
    //h(k,i) = (h1(k) + i*(h2(k)))%m
    return (hash(m,k) + i*((*h2)(m,k)))%m;
  }
private:
  HashingMethod* h2;
};

#endif // PROBING_HPP
