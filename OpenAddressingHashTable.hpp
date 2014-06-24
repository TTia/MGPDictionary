#ifndef OPENHASHTABLE_HPP
#define OPENHASHTABLE_HPP

#include "Dictionary.hpp"

template<typename Key, typename Value>
class OpenAddressingcHashTable: public Dictionary<Key, Value>{
public:
  Value insert(const Key, Value&);
  Value del(const Key);
  Value search(const Key);
};

#endif // OPENHASHTABLE_HPP
