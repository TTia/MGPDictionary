#ifndef CHAININGHASHTABLE_HPP
#define CHAININGHASHTABLE_HPP

#include "Dictionary.hpp"
#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>

template<typename Key, typename Value>
class ChainingHashTable/*: public Dictionary<Key, Value>*/{
private:
  int m, n;
  double loadFactorThreshold;
  std::vector<std::pair<Key, Value>>** table;
  std::hash<Key> h;

public:
  ChainingHashTable() = delete;
  ChainingHashTable(int m, std::hash<Key>, double loadFactorThreshold = 0.5);

  Value insert(const Key, Value&);
  Value del(const Key key);
  Value* search(const Key);
  //Value& operator[](const Key);
  inline int countValues(){
    return n;
  }
  inline double loadFactor(){
    return double(n)/m;
  }

  ~ChainingHashTable(){
    /*if(!table){
        return;
      }*/
    /*
    for(int i = 0; i<m; i++){
        if(!table[i]){
            delete table[i];
          }
      }
      */
    delete [] table;
  }
};

template<typename Key, typename Value>
ChainingHashTable<Key, Value>::ChainingHashTable(int m, std::hash<Key> h, double loadFactorThreshold):
  m{m}, loadFactorThreshold{loadFactorThreshold}, h{h}{
  if(m <= 0 || loadFactorThreshold <= 0 || loadFactorThreshold > 1){
      throw std::logic_error("m should be greater than 0 and the load factor should be in (0,1].");
    }
  table = new std::vector<std::pair<Key, Value>>*[m];
  for(int i = 0; i<m; i++){
      table[i] = nullptr;
    }
  //table = {0};
}

template<typename Key, typename Value>
Value ChainingHashTable<Key, Value>::insert(const Key key, Value &value){
  int i = h(key)%m;
  std::pair<Key,Value>* pair = new std::pair<Key, Value>(key, value);
  if(table[i] == nullptr){
      table[i] = new std::vector<std::pair<Key, Value>>();
      table[i]->push_back(*pair);
      n++;
      return NULL;
    }
  for(size_t j = 0; j != table[i]->size(); j++) {
      if(table[i]->at(j).first == key){
          Value oldValue = table[i]->at(j).second;
          table[i]->at(j) = *pair;
          return oldValue;
        }
    }
  table[i]->push_back(*pair);
  n++;
  return NULL;
}

template<typename Key, typename Value>
Value ChainingHashTable<Key, Value>::del(const Key key){
  auto hashValue = h(key);
  int i = hashValue%m;
  if(table[i] == nullptr){
      return NULL;
    }
  for(size_t j = 0; j != table[i]->size(); j++) {
      if(table[i]->at(j).first == key){
          Value oldValue = table[i]->at(j).second;
          table[i]->erase(table[i]->begin()+j);
          n--;
          return oldValue;
        }
    }
  return NULL;
}

template<typename Key, typename Value>
Value* ChainingHashTable<Key, Value>::search(const Key key){
  auto hashValue = h(key);
  int i = hashValue%m;
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
