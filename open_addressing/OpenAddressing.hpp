#ifndef OPENADDRESSING_HPP
#define OPENADDRESSING_HPP

#include "Core.hpp"
#include "ProbingHashTable.hpp"
#include "Probing.hpp"

template <typename Key, typename Value, typename Method = LinearProbing>
class OpenAddressingDictionary: public Core<ProbingHashTable<Key, Value, Method>, Key, Value, Method>{
public:
  using Core<ProbingHashTable<Key, Value, Method>, Key, Value, Method>::Core;
};

#endif // OPENADDRESSING_HPP
