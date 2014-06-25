#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "Hashing.hpp"

template <typename Key, typename Value>
class Dictionary{
public:
  virtual Value insert(const Key, Value&) = 0;
  virtual Value del(const Key key) = 0;
  virtual Value* search(const Key) = 0;
  virtual int countValues() = 0;
  virtual double loadFactor() = 0;
};

#endif // DICTIONARY_HPP
