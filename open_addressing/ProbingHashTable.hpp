#ifndef CHAININGHASHTABLE_HPP
#define CHAININGHASHTABLE_HPP

#include "ProbingHashTableIterator.hpp"

#include <iostream>
#include <stdexcept>
#include <memory>

template <typename Key, typename Value, typename Method>
class PHTBidirectionalIterator;
template<typename Key, typename Value, typename Method>
class PHTBidirectionalIterator_Key;
template<typename Key, typename Value, typename Method>
class PHTBidirectionalIterator_Value;

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

  bool insert(const Key, const Value&, Value** = nullptr);
  bool del(const Key, Value** = nullptr);
  iterator_value search(const Key);
//  Value* search(const Key);

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

  inline iterator begin(){
    if(!countValues()){
        return this->end();
      }
    if(to_table){
        _rehash(from_n);
      }
    return *new iterator(this);
  }

  inline iterator end(){
    return *new iterator(this, -1);
  }

  inline iterator_key begin_key(){
    if(!countValues()){
        return this->end_key();
      }
    if(to_table){
        _rehash(from_n);
      }
    return *new iterator_key(this);
  }

  inline iterator_key end_key(){
    return *new iterator_key(this, -1);
  }

  inline iterator_value begin_value(){
    if(!countValues()){
        return this->end_value();
      }
    if(to_table){
        _rehash(from_n);
      }
    return *new iterator_value(this);
  }

  inline iterator_value end_value(){
    return *new iterator_value(this, -1);
  }

  ~ProbingHashTable(){
    updateVersion();
    _dealloc(from_table, &from_m, &from_n);
    _dealloc(to_table, &to_m, &to_n);
  }

private:
  long int from_m, to_m, from_n, to_n;
  long int min_m;
  double upperLF, lowerLF;
  Hash h;
  Method pm;
  Table *from_table, *to_table;
  Pair* deleted;

  std::shared_ptr<long int> version;

  inline void updateVersion(){
    *version = *version + 1l;
  }

  bool _insert(Table* table, long int* m, long int* n,
               const Key key, const Value& value,
               Value** output = nullptr){
    for(long int _i = 0; _i < *m; _i++){
        int index = pm(_i, *m, h(key));
        if(!table[index] || table[index] == deleted){
          auto pair = new Pair(key, value);
          table[index] = pair;
          (*n)++;
          updateVersion();
          return false;
        }else if(table[index]->first == key){
          if(output)
            *output = new Value(std::move(table[index]->second));
          auto pair = new Pair(key, value);
          table[index] = pair;
          updateVersion();
          return true;
        }
      }
    throw std::logic_error("Rehashing should have occurred.");
  }

  bool _del(Table* table,
            long int* m, long int* n, const Key key, Value** output){
    for(int _i = 0; _i<*m; _i++){
        int index = pm(_i, *m, h(key));
        if(table[index] && table[index] != deleted && table[index]->first == key){
            if(output)
              *output = new Value(std::move(table[index]->second));
            table[index] = deleted;
            (*n)--;
            updateVersion();
            return output;
          }
      }
    return false;
  }

  void _shrinkTable(){
    if(from_m <= min_m){
        return;
      }
    to_m = from_m / 2;
    to_n = 0;
    _alloca(to_m);
  }
  void _enlargeTable(){
    to_m = from_m * 2;
    to_n = 0;
    _alloca(to_m);
  }
  void _rehash(int k){
    int r = 0;
    for(int i = 0; i < from_m && r < k && from_n > 0; i++){
        if(!from_table[i]){
            continue;
          }
        if(from_table[i] == deleted){
            from_table[i] = nullptr;
            continue;
          }
        _insert(to_table, &to_m, &to_n, std::move(from_table[i]->first),
                std::move(from_table[i]->second));
        updateVersion();
        from_table[i] = nullptr;
        from_n--;
        r++;
      }
    if(!from_n){
        from_m = to_m;
        from_n = to_n;
        _dealloc(from_table, &to_m, &to_n);
        from_table = to_table;
        to_table = nullptr;
      }
  }
  void _alloca(int m){
    to_table = new Table[m];
    for(int i = 0; i<m; i++){
        to_table[i] = nullptr;
      }
    to_m = m;
  }
  void _dealloc(Table* table, long int* m, long int* n){
    for(int i = 0; table && i<*m; i++){
        if(!table[i]){
            delete table[i];
          }
      }
    delete [] table;
    table = nullptr;
    *m = 0;
    *n = 0;
  }

};

template<typename Key, typename Value, typename Method>
ProbingHashTable<Key, Value, Method>::ProbingHashTable(Hash h, double loadFactorThreshold, long int m):
  from_m{m}, to_m{0}, from_n{0}, to_n{0}, min_m{m},
  upperLF{loadFactorThreshold}, lowerLF{upperLF*0.30}, h{h},
  from_table{nullptr}, to_table{nullptr} {
  if(m <= 0 || loadFactorThreshold <= 0 || loadFactorThreshold > 1){
      throw std::logic_error("m should be greater than 0 and the load factor should be in (0,1].");
    }

  deleted = new Pair();
  from_table = new Table[from_m];
  for(int i = 0; i<from_m; i++){
      from_table[i] = nullptr;
    }
  version = std::make_shared<long int>(0);
}

template<typename Key, typename Value, typename Method>
bool ProbingHashTable<Key, Value, Method>::insert(const Key key, const Value &value, Value** output){
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
bool ProbingHashTable<Key, Value, Method>::del(const Key key, Value** output){
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
typename ProbingHashTable<Key, Value, Method>::iterator_value
//Value*
ProbingHashTable<Key, Value, Method>::search(const Key key){
  if(to_table){
      _rehash(from_n);
    }
  for(int _i = 0; _i < from_m; _i++){
      int index = pm(_i, from_m, h(key));
      if(!from_table[index]){
//          return nullptr;
          return end_value();
        }
      if(from_table[index]->first == key){
//          return &from_table[index]->second;
          auto it = new iterator_value(this, index);
          return *it;
        }
    }
//  return nullptr;
    return end_value();
}

#endif // CHAININGHASHTABLE_HPP
