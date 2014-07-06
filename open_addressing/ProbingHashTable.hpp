#ifndef CHAININGHASHTABLE_HPP
#define CHAININGHASHTABLE_HPP

//#include "Dictionary.hpp"
#include "Probing.hpp"

#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <memory>

template<typename Key, typename Value, typename ProbingMethod = DoubleHashing>
class ChainingHashTable{

public:
  typedef std::hash<Key> Hash;
  typedef std::pair<Key, Value> Pair;
  typedef Pair** Table;

  ChainingHashTable() = delete;
  ChainingHashTable(Hash, double loadFactorThreshold = 0.5, long int m = 17);

  bool insert(const Key, const Value&, Value** = nullptr);
  bool del(const Key, Value** = nullptr);
//  iterator_value search(const Key);
  Value& search(const Key);

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

  ~ChainingHashTable(){
    updateVersion();
    _dealloc(from_table, &from_m, &from_n);
    _dealloc(to_table, &to_m, &to_n);
  }

private:
  long int from_m, to_m, from_n, to_n;
  long int min_m;
  double upperLF, lowerLF;
  Hash h;
  HashingMethod hm;
  Table *from_table, *to_table;

  std::shared_ptr<long int> version;

  inline void updateVersion(){
    *version = *version + 1l;
  }

  bool _insert(Table* table,
               long int* m, long int* n, const Key key, const Value& value, Value** output = nullptr){
    throw std::logic_error("Not Implemented.");
  }

  bool _del(Table* table,
            long int* m, long int* n, const Key key, Value** output){
    throw std::logic_error("Not Implemented.");
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
    throw std::logic_error("Not Implemented.");
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

template<typename Key, typename Value, typename HashingMethod>
ChainingHashTable<Key, Value, HashingMethod>::ChainingHashTable(Hash h, double loadFactorThreshold, long int m):
  from_m{m}, to_m{0}, from_n{0}, to_n{0}, min_m{m},
  upperLF{loadFactorThreshold}, lowerLF{upperLF*0.30}, h{h},
  from_table{nullptr}, to_table{nullptr} {
  if(m <= 0 || loadFactorThreshold <= 0 || loadFactorThreshold > 1){
      throw std::logic_error("m should be greater than 0 and the load factor should be in (0,1].");
    }
    throw std::logic_error("Not Implemented.");
}

template<typename Key, typename Value, typename HashingMethod>
bool ChainingHashTable<Key, Value, HashingMethod>::insert(const Key key, const Value &value, Value** output){
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

template<typename Key, typename Value, typename HashingMethod>
bool ChainingHashTable<Key, Value, HashingMethod>::del(const Key key, Value** output){
  if(loadFactor() < lowerLF && !to_table){
      _shrinkTable();
    }
  if(to_table){
      _rehash(rehashThreshold());
    }
  return _del(from_table, &from_m, &from_n, key, output) ||
          (to_table && _del(to_table, &to_m, &to_n, key, output));
}

template<typename Key, typename Value, typename HashingMethod>
//typename ChainingHashTable<Key, Value, HashingMethod>::iterator_value
Value&
ChainingHashTable<Key, Value, HashingMethod>::search(const Key key){
  throw std::logic_error("Not Implemented.");
}

#endif // CHAININGHASHTABLE_HPP
