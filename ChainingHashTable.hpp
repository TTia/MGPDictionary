#ifndef CHAININGHASHTABLE_HPP
#define CHAININGHASHTABLE_HPP

#include "Dictionary.hpp"
#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>

template<typename Key, typename Value, typename HashingMethod = KnuthDivisionMethod>
class ChainingHashTable/*: public Dictionary<Key, Value>*/{
private:
  int m, n;
  double loadFactorThreshold;
  std::hash<Key> h;
  std::vector<std::pair<Key, Value>>** table;
  HashingMethod hm;

public:
  ChainingHashTable() = delete;
  ChainingHashTable(std::hash<Key>, double loadFactorThreshold = 0.5, int m = 17);

  bool insert(const Key, Value&, Value** = nullptr);
  bool del(const Key, Value** = nullptr);
  Value* search(const Key);
  Value* operator[](const Key);

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
};



template<typename Key, typename Value, typename HashingMethod>
ChainingHashTable<Key, Value, HashingMethod>::ChainingHashTable(std::hash<Key> h, double loadFactorThreshold, int m):
  m{m}, n{0}, loadFactorThreshold{loadFactorThreshold}, h{h}, table{nullptr}{
  if(m <= 0 || loadFactorThreshold <= 0 || loadFactorThreshold > 1){
      throw std::logic_error("m should be greater than 0 and the load factor should be in (0,1].");
    }
  table = new std::vector<std::pair<Key, Value>>*[m];
  for(int i = 0; i<m; i++){
      table[i] = nullptr;
    }
}

template<typename Key, typename Value, typename HashingMethod>
bool ChainingHashTable<Key, Value, HashingMethod>::insert(const Key key, Value &value, Value** output){
//  int i = h(key)%m;
  auto hashValue = h(key);
  int i = hm(m, hashValue);
  std::pair<Key,Value>* pair = new std::pair<Key, Value>(key, value);
  if(!table[i]){
      table[i] = new std::vector<std::pair<Key, Value>>();
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
//  int i = hashValue%m;
  auto hashValue = h(key);
  int i = hm(m, hashValue);
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
Value* ChainingHashTable<Key, Value, HashingMethod>::search(const Key key){
  auto hashValue = h(key);
  int i = hm(m, hashValue);
  if(table[i] == nullptr){
      return nullptr;
    }
  for(size_t j = 0; j != table[i]->size(); j++) {
      if(table[i]->at(j).first == key){
          Value* oldValue = &table[i]->at(j).second;
          return oldValue;
        }
    }
  return nullptr;
}

#endif // CHAININGHASHTABLE_HPP
