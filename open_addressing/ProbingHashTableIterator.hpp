#ifndef CHAININGHASHTABLEITERATOR_HPP
#define CHAININGHASHTABLEITERATOR_HPP

#include "../Hashing.hpp"
#include "Probing.hpp"
#include "ProbingHashTable.hpp"
#include <memory>
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>
#include <stdexcept>

template<typename Key, typename Value, typename ProbingMethod>
class ProbingHashTable;
//template<typename Key, typename Value, typename HashingMethod>
//class CHTBidirectionalIterator_Key;
//template<typename Key, typename Value, typename HashingMethod>
//class CHTBidirectionalIterator_Value;

template <typename Key, typename Value, typename ProbingMethod = LinearProbing>
class PHTBidirectionalIterator:
    public boost::iterator_facade<PHTBidirectionalIterator<Key, Value, ProbingMethod>,
                                  typename ProbingHashTable<Key, Value, ProbingMethod>::Pair&,
                                  std::bidirectional_iterator_tag> {
public:
  friend class boost::iterator_core_access;
//  friend class CHTBidirectionalIterator_Key<Key, Value, HashingMethod>;
//  friend class CHTBidirectionalIterator_Value<Key, Value, HashingMethod>;

  PHTBidirectionalIterator() = delete;
  PHTBidirectionalIterator(ProbingHashTable<Key, Value, ProbingMethod>* htable, int i):
    table{htable->from_table}, deleted{htable->deleted}, m{htable->from_m}, i{i}, version{htable->version}, originalVersion{*htable->version}
  {
    if(!isValid() || !checkBoundaries() || !table[i] || table[i] == deleted){
        endIteration();
      }
  }

  PHTBidirectionalIterator(ProbingHashTable<Key, Value, ProbingMethod>* htable):
    table{htable->from_table}, deleted{htable->deleted}, m{htable->from_m}, i{0}, version{htable->version}, originalVersion{*htable->version}
  {
    if(!table[i] || table[i] == deleted){
        increment();
      }else{
        if(!isValid()){
            endIteration();
        }
      }
  }
private:
  typename ProbingHashTable<Key, Value, ProbingMethod>::Table* table;
  const typename ProbingHashTable<Key, Value, ProbingMethod>::Pair* deleted;

  long int m, i;
  std::shared_ptr<long int> version;
  long int originalVersion;

  inline bool isEnd() const{
    return i == -1;
  }
  inline void endIteration(){
    i = -1;
  }
  inline bool isValid() const{
    return *version == originalVersion;
  }

  inline bool checkBoundaries() const{
    return i >= 0 && i < m;
  }

  void increment() {
    if(isEnd()){
        return;
      }
    if(!isValid()){
        endIteration();
        return;
      }
    do{
      i++;
    }while(checkBoundaries() && (!table[i] || table[i] == deleted));
    if(!checkBoundaries()){
        endIteration();
      }
  }

  void decrement() {
    if(isEnd()){
        return;
      }
    if(!isValid()){
        endIteration();
        return;
      }
    do{
      i--;
    }while(checkBoundaries() && (!table[i] || table[i] == deleted));
    if(!checkBoundaries()){
        endIteration();
      }
  }

  bool equal(PHTBidirectionalIterator const& other) const
  {
    if((!isValid() || isEnd()) && (!other.isValid() || other.isEnd())){
        return table == other.table;
    }
    return table == other.table && i == other.i;
  }

  typename ProbingHashTable<Key, Value, ProbingMethod>::Pair& dereference() const {
    if(!isValid()){
        throw std::logic_error("Invalid iterator.");
      }else if(isEnd()){
        throw std::out_of_range("This iterator has reach the end.");
      }else if(!table[i]){
        throw std::out_of_range("Seg-fault. Sorry!");
      }
    return *table[i];
  }
};

#endif
