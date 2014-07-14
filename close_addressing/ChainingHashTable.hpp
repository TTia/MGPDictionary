#ifndef CHAININGHASHTABLE_HPP
#define CHAININGHASHTABLE_HPP

#include "ChainingHashTableIterator.hpp"
#include "Hashing.hpp"

#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <memory>

template<typename Key, typename Value, typename Method = DivisionMethod>
class ChainingHashTable{
  friend class CHTBidirectionalIterator<Key, Value, Method>;

public:
  typedef std::hash<Key> Hash;
  typedef std::pair<Key, Value> Pair;
  typedef std::vector<Pair>* Table;
  typedef CHTBidirectionalIterator<Key, Value, Method> iterator;
  typedef CHTBidirectionalIterator_Key<Key, Value, Method> iterator_key;
  typedef CHTBidirectionalIterator_Value<Key, Value, Method> iterator_value;

  ChainingHashTable() = delete;

  ChainingHashTable(Hash, double loadFactorThreshold = 0.5, long int m = 17);

  bool insert(const Key, const Value&, Value* = nullptr);

  bool del(const Key, Value* = nullptr);

  iterator_value search(const Key);

  iterator begin();

  iterator_key begin_key();

  iterator_value begin_value();

  iterator end();

  iterator_key end_key();

  iterator_value end_value();

  ~ChainingHashTable();

  inline int rehashThreshold(){
    return (to_table? to_m: from_m) * 0.10;
  }

  inline int countValues(){
    return to_n+from_n;
  }

  inline double loadFactor(){
    return !(to_table? to_m: from_m) ?
          0 : double(countValues())/ (to_table? to_m: from_m);
  }

private:
  typedef std::vector<std::pair<Key, Value>> __vector;

  long int from_m, to_m, from_n, to_n;
  long int min_m;
  double upperLF, lowerLF;
  Hash h;
  Method hm;
  Table *from_table, *to_table;
  std::shared_ptr<long int> version;

  inline void updateVersion(){
    *version = *version + 1l;
  }

  bool _insert(Table* table,
               long int* m, long int* n,
               const Key key, const Value& value, Value* output);

  bool _del(Table* table, long int* m, long int* n,
            const Key key, Value* output);

  void _shrinkTable();

  void _enlargeTable();

  void _rehash(int k);

  void _alloca(int m);

  void _dealloc(Table* table, long int* m);
};

/*
 * Public Methods
 */

template<typename Key, typename Value, typename Method>
ChainingHashTable<Key, Value, Method>::ChainingHashTable(Hash h, double loadFactorThreshold, long int m):
  from_m{m}, to_m{0}, from_n{0}, to_n{0}, min_m{m},
  upperLF{loadFactorThreshold}, lowerLF{upperLF*0.30}, h{h},
  from_table{nullptr}, to_table{nullptr} {
  if(m <= 0 || loadFactorThreshold <= 0 || loadFactorThreshold > 1){
      throw std::logic_error("m should be greater than 0 and the load factor should be in (0,1].");
    }

  from_table = new Table[m];
  for(long int i = 0; i<m; i++){
      from_table[i] = nullptr;
    }
  version = std::make_shared<long int>(0);
}

template<typename Key, typename Value, typename Method>
bool ChainingHashTable<Key, Value, Method>::insert(const Key key, const Value &value, Value* output){
  if(loadFactor() > upperLF && !to_table){
      _enlargeTable();
    }
  if(to_table){
      _rehash(rehashThreshold());
    }
  updateVersion();
  return to_table ? _insert(to_table, &to_m, &to_n, key, value, output) :
                    _insert(from_table, &from_m, &from_n, key, value, output);
}

template<typename Key, typename Value, typename Method>
bool ChainingHashTable<Key, Value, Method>::del(const Key key, Value* output){
  if(loadFactor() < lowerLF && !to_table){
      _shrinkTable();
    }
  if(to_table){
      _rehash(rehashThreshold());
    }
  return _del(from_table, &from_m, &from_n, key, output) ||
          (to_table && _del(to_table, &to_m, &to_n, key, output));
}

template<typename Key, typename Value, typename Method>
typename ChainingHashTable<Key, Value, Method>::iterator_value
ChainingHashTable<Key, Value, Method>::search(const Key key){
  if(to_table){
      _rehash(from_n);
    }
  long int i = hm(from_m, h(key));
  if(!from_table[i]){
      return this->end_value();
    }
  for(size_t j = 0; j != from_table[i]->size(); j++) {
      if(from_table[i]->at(j).first == key){
          //auto it = new iterator_value(this, i, j);
          iterator_value it(this, i, j);
          return it;
        }
    }
  return this->end_value();
}
template<typename Key, typename Value, typename Method>
typename ChainingHashTable<Key, Value, Method>::iterator
ChainingHashTable<Key, Value, Method>::begin(){
  if(!countValues())
    return this->end();
  if(to_table)
    _rehash(from_n);
  iterator it(this);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ChainingHashTable<Key, Value, Method>::iterator_key
ChainingHashTable<Key, Value, Method>::begin_key(){
  if(!countValues())
    return this->end_key();
  if(to_table)
    _rehash(from_n);
  iterator_key it(this);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ChainingHashTable<Key, Value, Method>::iterator_value
ChainingHashTable<Key, Value, Method>::begin_value(){
  if(!countValues())
    return this->end_value();
  if(to_table)
    _rehash(from_n);
  iterator_value it(this);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ChainingHashTable<Key, Value, Method>::iterator
ChainingHashTable<Key, Value, Method>::end(){
  iterator it(this, -1);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ChainingHashTable<Key, Value, Method>::iterator_key
ChainingHashTable<Key, Value, Method>::end_key(){
  iterator_key it(this, -1);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ChainingHashTable<Key, Value, Method>::iterator_value
ChainingHashTable<Key, Value, Method>::end_value(){
  iterator_value it(this, -1);
  return it;
}

template<typename Key, typename Value, typename Method>
ChainingHashTable<Key, Value, Method>::~ChainingHashTable(){
  updateVersion();
  _dealloc(from_table, &from_m);
  _dealloc(to_table, &to_m);
}

/*
 * Private Methods
 */
template<typename Key, typename Value, typename Method>
void ChainingHashTable<Key, Value, Method>::_dealloc(Table* table, long int* m){
  for(long int i = 0; table && i<*m; i++){
      if(table[i]){
          table[i]->clear();
          delete table[i];
        }
    }
  delete [] table;
  table = nullptr;
}

template<typename Key, typename Value, typename Method>
void ChainingHashTable<Key, Value, Method>::_alloca(int m){
  to_table = new Table[m];
  for(int i = 0; i<m; i++){
      to_table[i] = nullptr;
    }
  to_m = m;
}

template<typename Key, typename Value, typename Method>
void ChainingHashTable<Key, Value, Method>::_rehash(int k){
  int r = 0;
  for(long int i = 0; i < from_m && r < k && from_n > 0; i++){
      while(from_table[i] && r < k && from_table[i]->size() > 0){
          auto pair = from_table[i]->at(0);
          _insert(to_table, &to_m, &to_n, pair.first, pair.second, nullptr);
          from_table[i]->erase(from_table[i]->begin());
          from_n--;
          r++;
        }
      updateVersion();
    }
  if(!from_n){
      _dealloc(from_table, &from_m);
      from_m = to_m;
      from_n = to_n;
      to_m = 0;
      to_n = 0;
      from_table = to_table;
      to_table = nullptr;
    }
}

template<typename Key, typename Value, typename Method>
void ChainingHashTable<Key, Value, Method>::_enlargeTable(){
  to_m = from_m * 2;
  to_n = 0;
  _alloca(to_m);
}

template<typename Key, typename Value, typename Method>
void ChainingHashTable<Key, Value, Method>::_shrinkTable(){
  if(from_m <= min_m){
      return;
    }
  to_m = from_m / 2;
  to_n = 0;
  _alloca(to_m);
}

template<typename Key, typename Value, typename Method>
bool ChainingHashTable<Key, Value, Method>::_del(Table* table,
          long int* m, long int* n, const Key key, Value* output){
  int i = hm(*m, h(key));
  if(!table[i] || table[i]->size() == 0){
      return false;
    }
  for(size_t j = 0; j != table[i]->size(); j++) {
      if(table[i]->at(j).first == key){
          if(output){
            *output = std::move(table[i]->at(j).second);
          }
          table[i]->erase(table[i]->begin()+j);
          (*n)--;
          updateVersion();
          return output;
        }
    }
  return false;
}

template<typename Key, typename Value, typename Method>
bool ChainingHashTable<Key, Value, Method>::_insert(Table* table,
             long int* m, long int* n, const Key key, const Value& value, Value* output){
  int i = hm(*m, h(key));
  Pair pair(key, value);

  if(!table[i]){
      table[i] = new __vector();
      table[i]->push_back(pair);
      (*n)++;
      return false;
    }
  for(size_t j = 0; j != table[i]->size(); j++) {
      if(table[i]->at(j).first == key){
          if(output){
            *output = std::move(table[i]->at(j).second);
          }
          table[i]->at(j) = pair;
          return true;
        }
    }
  table[i]->push_back(pair);
  (*n)++;
  return false;
}

#endif // CHAININGHASHTABLE_HPP
