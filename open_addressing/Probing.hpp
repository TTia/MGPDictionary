#ifndef PROBING_HPP
#define PROBING_HPP

#include "Hashing.hpp"

class ProbingMethod{
public:
  ProbingMethod(HashingMethod hm): hm{&hm}{}
  ProbingMethod(): hm{new DivisionMethod()}{}
  virtual long int probe(long int i, long int m, long int k) const = 0;
  long int operator()(long int i, const long int m, const long int k) const{
    return this->probe(i,m,k);
  }
  ~ProbingMethod(){
    delete hm;
  }
protected:
  virtual long int hash(const long int m, const long int k) const{
    return (*hm)(m,k);
  }
private:
  HashingMethod* hm;
};

class LinearProbing: public ProbingMethod{
public:
  inline long int probe(long int i, long int m, long int k) const{
    //h(k,i) = (h'(k)+i)%m
    return (hash(m, k)+i)%m;
  }
};

class QuadraticProbing: public ProbingMethod{
public:
  inline long int probe(long int i, long int m, long int k) const{
    //h(k, i) = (h'(k) + i^2)%m
    return (hash(m, k) + i*i)%m;
  }
};

class DoubleHashing: public ProbingMethod{
public:
  DoubleHashing(): ProbingMethod(), h2{new DivisionMethod()}{}
  DoubleHashing(HashingMethod h1, HashingMethod h2): ProbingMethod(h1), h2{&h2}{}
  inline long int probe(long int i, long int m, long int k) const{
    //h(k,i) = (h1(k) + i*(h2(k)))%m
    return (hash(m,k) + i*((*h2)(m,k)))%m;
  }
private:
  HashingMethod* h2;
};

#endif // PROBING_HPP

