#ifndef PROBING_HPP
#define PROBING_HPP

#include "Hashing.hpp"
#include <memory>

class ProbingMethod{
public:
  ProbingMethod(HashingMethod hm): hm{&hm}{}
  ProbingMethod(): hm{new DivisionMethod()}{}

  ProbingMethod(const ProbingMethod& other) = delete;

  ProbingMethod& operator=(ProbingMethod& other) = delete;

  ProbingMethod(ProbingMethod&& other){
    hm = std::move(other.hm);
    other.hm = 0;
  }

  ProbingMethod& operator=(ProbingMethod&& other){
    delete hm;
    hm = std::move(other.hm);
    other.hm = 0;
    return *this;
  }

  virtual ProbingMethod* clone() = 0;

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
  using ProbingMethod::ProbingMethod;
  inline long int probe(long int i, long int m, long int k) const{
    //h(k,i) = (h'(k)+i)%m
    return (hash(m, k)+i)%m;
  }
  ProbingMethod* clone(){
    return new LinearProbing();
  }
};

class QuadraticProbing: public ProbingMethod{
public:
  using ProbingMethod::ProbingMethod;
  inline long int probe(long int i, long int m, long int k) const{
    //h(k, i) = (h'(k) + i^2)%m
    return (hash(m, k) + i*i)%m;
  }
  ProbingMethod* clone(){
    return new QuadraticProbing();
  }
};

class DoubleHashing: public ProbingMethod{
public:
  using ProbingMethod::ProbingMethod;
  DoubleHashing(): ProbingMethod(), hm2{new DivisionMethod()}{}
  DoubleHashing(HashingMethod h1, HashingMethod h2): ProbingMethod(h1), hm2{&h2}{}

  DoubleHashing(ProbingMethod&& other): ProbingMethod(std::move(other)), hm2{new DivisionMethod()}{}

  DoubleHashing& operator=(ProbingMethod&& other){
    ProbingMethod::operator =(std::move(other));
    return *this;
  }

  DoubleHashing(DoubleHashing&& other): ProbingMethod(std::move(other)){
    other.hm2 = std::move(other.hm2);
    other.hm2 = 0;
  }

  DoubleHashing& operator=(DoubleHashing&& other){
    ProbingMethod::operator =(std::move(other));
    delete hm2;
    other.hm2 = std::move(other.hm2);
    other.hm2 = 0;
    return *this;
  }

  inline long int probe(long int i, long int m, long int k) const{
    //h(k,i) = (h1(k) + i*(h2(k)))%m
    return (hash(m,k) + i*((*hm2)(m,k)))%m;
  }
  ~DoubleHashing(){
    delete hm2;
  }
  ProbingMethod* clone(){
    return new DoubleHashing();
  }

private:
  HashingMethod* hm2;
};

#endif // PROBING_HPP

