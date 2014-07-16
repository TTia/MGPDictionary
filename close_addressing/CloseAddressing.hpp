#ifndef CLOSEADDRESSING_HPP
#define CLOSEADDRESSING_HPP

#include "Core.hpp"
#include "ChainingHashTable.hpp"
#include "Hashing.hpp"

template <typename Key, typename Value, typename Method = DivisionMethod>
class CloseAddressingDictionary: public Core<ChainingHashTable<Key, Value, Method>, Key, Value, Method>{
public:
  using Core<ChainingHashTable<Key, Value, Method>, Key, Value, Method>::Core;
};

#endif // CLOSEADDRESSING_HPP
