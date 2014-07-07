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
template<typename Key, typename Value, typename ProbingMethod>
class PHTBidirectionalIterator_Key;
template<typename Key, typename Value, typename ProbingMethod>
class PHTBidirectionalIterator_Value;

template <typename Key, typename Value, typename ProbingMethod = LinearProbing>
class PHTBidirectionalIterator:
    public boost::iterator_facade<PHTBidirectionalIterator<Key, Value, ProbingMethod>,
                                  typename ProbingHashTable<Key, Value, ProbingMethod>::Pair&,
                                  std::bidirectional_iterator_tag> {
public:
  friend class boost::iterator_core_access;
  friend class PHTBidirectionalIterator_Key<Key, Value, ProbingMethod>;
  friend class PHTBidirectionalIterator_Value<Key, Value, ProbingMethod>;

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

template <typename Key, typename Value, typename HashingMethod = LinearProbing>
class PHTBidirectionalIterator_Key:
    public boost::iterator_facade<PHTBidirectionalIterator_Key<Key, Value, ProbingMethod>,
                                  Key&,
                                  std::bidirectional_iterator_tag> {
private:
  PHTBidirectionalIterator<Key, Value, HashingMethod>* it;

  void increment() {
    this->it->increment();
  }

  void decrement() {
    this->it->decrement();
  }

  bool equal(PHTBidirectionalIterator_Key const& other) const
  {
    return this->it->equal(*other.it);
  }

  Key& dereference() const {
    return this->it->dereference().first;
  }
public:
  friend class boost::iterator_core_access;

  PHTBidirectionalIterator_Key(ProbingHashTable<Key, Value, ProbingMethod>* htable, int i):
  it{new PHTBidirectionalIterator<Key, Value, ProbingMethod>(htable, i)}{}

  PHTBidirectionalIterator_Key(ProbingHashTable<Key, Value, ProbingMethod>* htable):
  it{new PHTBidirectionalIterator<Key, Value, ProbingMethod>(htable)}{}
};

template <typename Key, typename Value, typename ProbingMethod = LinearProbing>
class PHTBidirectionalIterator_Value:
    public boost::iterator_facade<PHTBidirectionalIterator_Value<Key, Value, LinearProbing>,
                                  Value&,
                                  std::bidirectional_iterator_tag> {
private:
  PHTBidirectionalIterator<Key, Value, ProbingMethod>* it;

  void increment() {
    this->it->increment();
  }

  void decrement() {
    this->it->decrement();
  }

  bool equal(PHTBidirectionalIterator_Value const& other) const
  {
    return this->it->equal(*other.it);
  }

  Value& dereference() const {
    return this->it->dereference().second;
  }

public:
  friend class boost::iterator_core_access;

  PHTBidirectionalIterator_Value(ProbingHashTable<Key, Value, ProbingMethod>* htable, int i):
  it{new PHTBidirectionalIterator<Key, Value, ProbingMethod>(htable, i)}{}

  PHTBidirectionalIterator_Value(ProbingHashTable<Key, Value, ProbingMethod>* htable):
  it{new PHTBidirectionalIterator<Key, Value, ProbingMethod>(htable)}{}
};

#endif
