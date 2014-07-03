#ifndef CHAININGHASHTABLE_HPP
#define CHAININGHASHTABLE_HPP

#include "Dictionary.hpp"
#include "ChainingHashTableIterator.hpp"

#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>

template<typename Key, typename Value, typename HashingMethod = DivisionMethod>
class ChainingHashTable{
  friend class CHTBidirectionalIterator<Key, Value, HashingMethod>;

public:
  typedef std::vector<std::pair<Key, Value>>* Table;
  typedef std::hash<Key> Hash;
  typedef std::pair<Key, Value> Pair;
  typedef CHTBidirectionalIterator<Key, Value, HashingMethod> iterator;
  typedef CHTBidirectionalIterator_Key<Key, Value, HashingMethod> iterator_key;
  typedef CHTBidirectionalIterator_Value<Key, Value, HashingMethod> iterator_value;

  ChainingHashTable() = delete;
  ChainingHashTable(Hash, double loadFactorThreshold = 0.5, long int m = 17);

  bool insert(const Key, Value&, Value** = nullptr);
  bool del(const Key, Value** = nullptr);
//  iterator search(const Key);
  Value* search(const Key);
//  iterator operator[](const Key);
  Value* operator[](const Key);

  inline iterator begin(){
    if(!n){
        return this->end();
      }
    auto it = *new CHTBidirectionalIterator<Key, Value, HashingMethod>(this);
    return it;
  }

  inline iterator_key begin_key(){
    if(!n){
        return this->end_key();
      }
    auto it = *new CHTBidirectionalIterator_Key<Key, Value, HashingMethod>(this);
    return it;
  }

  inline iterator_value begin_value(){
    if(!n){
        return this->end_value();
      }
    auto it = *new CHTBidirectionalIterator_Value<Key, Value, HashingMethod>(this);
    return it;
  }

  inline iterator end(){
    return *new iterator(this, m);
  }

  inline iterator_key end_key(){
    return *new iterator_key(this, m);
  }

  inline iterator_value end_value(){
    return *new iterator_value(this, m);
  }

  inline int countValues(){
    return n;
  }
  inline double loadFactor(){
    return double(n)/m;
  }

  ~ChainingHashTable(){
    if(!table){
        return;
      }
    for(int i = 0; i<m; i++){
        if(!table[i]){
            delete table[i];
          }
      }
    delete [] table;
  }

private:
  typedef std::vector<std::pair<Key, Value>> __vector;

  long int m, n;
  double loadFactorThreshold;
  Hash h;
  Table* table;
  HashingMethod hm;
};



template<typename Key, typename Value, typename HashingMethod>
ChainingHashTable<Key, Value, HashingMethod>::ChainingHashTable(Hash h, double loadFactorThreshold, long int m):
  m{m}, n{0}, loadFactorThreshold{loadFactorThreshold}, h{h}, table{nullptr}{
  if(m <= 0 || loadFactorThreshold <= 0 || loadFactorThreshold > 1){
      throw std::logic_error("m should be greater than 0 and the load factor should be in (0,1].");
    }
  table = new Table[m];
  for(int i = 0; i<m; i++){
      table[i] = nullptr;
    }
}

template<typename Key, typename Value, typename HashingMethod>
bool ChainingHashTable<Key, Value, HashingMethod>::insert(const Key key, Value &value, Value** output){
  int i = hm(m, h(key));
  Pair* pair = new Pair(key, value);
  if(!table[i]){
      table[i] = new std::vector<Pair>();
      table[i]->push_back(*pair);
      n++;
      return false;
    }
  for(size_t j = 0; j != table[i]->size(); j++) {
      if(table[i]->at(j).first == key){
          if(output)
            *output = new Value(std::move(table[i]->at(j).second));
          table[i]->at(j) = *pair;
          return true;
        }
    }
  table[i]->push_back(*pair);
  n++;
  return false;
}

template<typename Key, typename Value, typename HashingMethod>
bool ChainingHashTable<Key, Value, HashingMethod>::del(const Key key, Value** output){
  int i = hm(m, h(key));
  if(!table[i] || table[i]->size() == 0){
      return false;
    }
  for(size_t j = 0; j != table[i]->size(); j++) {
      if(table[i]->at(j).first == key){
          if(output != nullptr)
            *output = new Value(std::move(table[i]->at(j).second));
          table[i]->erase(table[i]->begin()+j);
          n--;
          break;
        }
    }
  return true;
}

template<typename Key, typename Value, typename HashingMethod>
//typename ChainingHashTable<Key, Value, HashingMethod>::iterator
Value* ChainingHashTable<Key, Value, HashingMethod>::search(const Key key){
  int i = hm(m, h(key));
  if(table[i] == nullptr){
      return nullptr;
    }
  for(size_t j = 0; j != table[i]->size(); j++) {
      if(table[i]->at(j).first == key){
//          auto it = new ChainingHashTable<Key, Value, HashingMethod>::iterator(this, i, j);
//          return *it;
          Value* oldValue = &table[i]->at(j).second;
          return oldValue;
        }
    }
  return nullptr;
}

#endif // CHAININGHASHTABLE_HPP
