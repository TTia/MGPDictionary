#ifndef CHAININGHASHTABLE_HPP
#define CHAININGHASHTABLE_HPP

#include "ProbingHashTableIterator.hpp"
#include "Probing.hpp"

#include <iostream>
#include <stdexcept>
#include <memory>

template<typename Key, typename Value, typename Method = LinearProbing>
class ProbingHashTable{
  friend class PHTBidirectionalIterator<Key, Value, Method>;

public:
  typedef std::hash<Key> Hash;
  typedef std::pair<Key, Value> Pair;
  typedef Pair* Table;
  typedef PHTBidirectionalIterator<Key, Value, Method> iterator;
  typedef PHTBidirectionalIterator_Key<Key, Value, Method> iterator_key;
  typedef PHTBidirectionalIterator_Value<Key, Value, Method> iterator_value;

  ProbingHashTable() = delete;

  ProbingHashTable(Hash, double loadFactorThreshold = 0.5, long int m = 17);

  bool insert(const Key, const Value&, Value * = nullptr);

  bool del(const Key, Value* = nullptr);

  iterator search(const Key);

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

  Value& operator[](const Key);

  iterator begin();

  iterator end();

  iterator_key begin_key();

  iterator_key end_key();

  iterator_value begin_value();

  iterator_value end_value();

  ~ProbingHashTable();

private:
  long int from_m, to_m, from_n, to_n;
  long int min_m;
  double upperLF, lowerLF;
  Hash h;
  Method pm;
  Table *from_table, *to_table;
  Pair *deleted;

  std::shared_ptr<long int> version;

  inline void updateVersion(){
    *version = *version + 1l;
  }

  bool _insert(Table* table, long int* m, long int* n,
               const Key key, const Value& value,
               Value* output);

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
ProbingHashTable<Key, Value, Method>::ProbingHashTable(Hash h, double loadFactorThreshold, long int m):
  from_m{m}, to_m{0}, from_n{0}, to_n{0}, min_m{m},
  upperLF{loadFactorThreshold}, lowerLF{upperLF*0.30}, h{h},
  from_table{nullptr}, to_table{nullptr}{
  if(m <= 0 || loadFactorThreshold <= 0 || loadFactorThreshold > 1){
      throw std::logic_error("m should be greater than 0 and the load factor should be in (0,1].");
    }

  from_table = new Table[from_m];
  for(long int i = 0; i<from_m; i++){
      from_table[i] = nullptr;
    }
  version = std::make_shared<long int>(0);
  deleted = new Pair();
}

template<typename Key, typename Value, typename Method>
bool ProbingHashTable<Key, Value, Method>::insert(const Key key, const Value &value, Value* output){
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
bool ProbingHashTable<Key, Value, Method>::del(const Key key, Value *output){
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
typename ProbingHashTable<Key, Value, Method>::iterator
ProbingHashTable<Key, Value, Method>::search(const Key key){
  if(to_table){
      _rehash(from_n);
    }
  for(long int _i = 0; _i < from_m; _i++){
      long int index = pm(_i, from_m, h(key));
      if(!from_table[index]){
          return end();
        }
      if(from_table[index] == deleted){
          continue;
        }
      if(from_table[index]->first == key){
          iterator it(this, index);
          return it;
        }
    }
  return end();
}

template<typename Key, typename Value, typename Method>
typename ProbingHashTable<Key, Value, Method>::iterator
ProbingHashTable<Key, Value, Method>::begin(){
  if(!countValues()){
      return this->end();
    }
  if(to_table){
      _rehash(from_n);
    }
  iterator it(this);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ProbingHashTable<Key, Value, Method>::iterator
ProbingHashTable<Key, Value, Method>::end(){
  iterator it(this, -1);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ProbingHashTable<Key, Value, Method>::iterator_key
ProbingHashTable<Key, Value, Method>::begin_key(){
  if(!countValues()){
      return this->end_key();
    }
  if(to_table){
      _rehash(from_n);
    }
  iterator_key it(this);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ProbingHashTable<Key, Value, Method>::iterator_key
ProbingHashTable<Key, Value, Method>::end_key(){
  iterator_key it(this, -1);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ProbingHashTable<Key, Value, Method>::iterator_value
ProbingHashTable<Key, Value, Method>::begin_value(){
  if(!countValues()){
      return this->end_value();
    }
  if(to_table){
      _rehash(from_n);
    }
  iterator_value it(this);
  return it;
}

template<typename Key, typename Value, typename Method>
typename ProbingHashTable<Key, Value, Method>::iterator_value
ProbingHashTable<Key, Value, Method>::end_value(){
  iterator_value it(this, -1);
  return it;
}
template<typename Key, typename Value, typename Method>
ProbingHashTable<Key, Value, Method>::~ProbingHashTable(){
  updateVersion();
  _dealloc(from_table, &from_m);
  _dealloc(to_table, &to_m);
  delete deleted;
}

template<typename Key, typename Value, typename Method>
Value& ProbingHashTable<Key, Value, Method>::operator[](const Key key){
  iterator it = this->search(key);
  if(it == this->end()){
      throw std::out_of_range("No such key.");
    }
  return (*it).second;
}

/*
 * Private Methods
 */

template<typename Key, typename Value, typename Method>
bool ProbingHashTable<Key, Value, Method>::_insert(Table* table, long int* m, long int* n,
             const Key key, const Value& value,
             Value* output){
  for(long int _i = 0; _i < *m; _i++){
      auto index = pm(_i, *m, h(key));
      if(table[index] == nullptr || table[index] == deleted){
          table[index] = new Pair(key, value);
          (*n)++;
          updateVersion();
          return false;
        }else if(table[index]->first == key){
          if(output != nullptr){
              *output = table[index]->second;
            }
          table[index]->second = value;
          updateVersion();
          return true;
        }
    }
  throw std::logic_error("Rehashing should have occurred.");
}

template<typename Key, typename Value, typename Method>
bool ProbingHashTable<Key, Value, Method>::_del(Table* table,
          long int* m, long int* n, const Key key, Value* output){
  for(long int _i = 0; _i<*m; _i++){
      int index = pm(_i, *m, h(key));
      if(table[index] && table[index] != deleted && table[index]->first == key){
          if(output != nullptr){
              *output = std::move(table[index]->second);
            }
          delete table[index];
          table[index] = deleted;
          (*n)--;
          updateVersion();
          return output;
        }
    }
  return false;
}

template<typename Key, typename Value, typename Method>
void ProbingHashTable<Key, Value, Method>::_shrinkTable(){
  if(from_m <= min_m){
      return;
    }
  to_m = from_m / 2;
  to_n = 0;
  _alloca(to_m);
}

template<typename Key, typename Value, typename Method>
void ProbingHashTable<Key, Value, Method>::_enlargeTable(){
  to_m = from_m * 2;
  to_n = 0;
  _alloca(to_m);
}

template<typename Key, typename Value, typename Method>
void ProbingHashTable<Key, Value, Method>::_rehash(int k){
  int r = 0;
  for(long int i = 0; i < from_m && r < k && from_n > 0; i++){
      if(!from_table[i]){
          continue;
        }
      if(from_table[i] == deleted){
          from_table[i] = nullptr;
          continue;
        }
      _insert(to_table, &to_m, &to_n, std::move(from_table[i]->first),
              std::move(from_table[i]->second), nullptr);
      delete from_table[i];
      updateVersion();
      from_table[i] = nullptr;
      from_n--;
      r++;
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
void ProbingHashTable<Key, Value, Method>::_alloca(int m){
  to_table = new Table[m];
  for(int i = 0; i<m; i++){
      to_table[i] = nullptr;
    }
  to_m = m;
}

template<typename Key, typename Value, typename Method>
void ProbingHashTable<Key, Value, Method>::_dealloc(Table* table, long int* m){
  for(long int i = 0; table && i<*m; i++){
      if(table[i] != nullptr && table[i] != deleted){
          delete table[i];
        }
    }
  delete [] table;
  table = nullptr;
}

#endif // CHAININGHASHTABLE_HPP
