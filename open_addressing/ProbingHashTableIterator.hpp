#ifndef CHAININGHASHTABLEITERATOR_HPP
#define CHAININGHASHTABLEITERATOR_HPP

#include "Probing.hpp"
//#include "ProbingHashTable.hpp"
#include <memory>
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>
#include <stdexcept>

template<typename Key, typename Value, typename Method>
class ProbingHashTable;
template<typename Key, typename Value, typename Method>
class PHTBidirectionalIterator_Key;
template<typename Key, typename Value, typename Method>
class PHTBidirectionalIterator_Value;

template <typename Key, typename Value, typename Method = LinearProbing>
class PHTBidirectionalIterator:
    public boost::iterator_facade<PHTBidirectionalIterator<Key, Value, Method>,
                                  typename ProbingHashTable<Key, Value, Method>::Pair&,
                                  std::bidirectional_iterator_tag> {
public:
  friend class boost::iterator_core_access;
  friend class PHTBidirectionalIterator_Key<Key, Value, Method>;
  friend class PHTBidirectionalIterator_Value<Key, Value, Method>;

  PHTBidirectionalIterator() = delete;
  PHTBidirectionalIterator(ProbingHashTable<Key, Value, Method>* htable, long int i):
    table{htable->from_table}, deleted{htable->deleted}, m{htable->from_m},
    i{i}, version{htable->version}, originalVersion{*htable->version}
  {
    if(!isValid() || !checkBoundaries() || !table[i] || table[i] == deleted){
        endIteration();
      }
  }

  PHTBidirectionalIterator(ProbingHashTable<Key, Value, Method>* htable):
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
  const typename ProbingHashTable<Key, Value, Method>::Table* table;
  const typename ProbingHashTable<Key, Value, Method>::Pair* deleted;

  long int m;
  long int i;
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
        //Switch table
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

  typename ProbingHashTable<Key, Value, Method>::Pair& dereference() const {
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

template <typename Key, typename Value, typename Method = LinearProbing>
class PHTBidirectionalIterator_Key:
    public boost::iterator_facade<PHTBidirectionalIterator_Key<Key, Value, Method>,
                                  Key&,
                                  std::bidirectional_iterator_tag> {
private:
  PHTBidirectionalIterator<Key, Value, Method> it;

  void increment() {
    this->it.increment();
  }

  void decrement() {
    this->itdecrement();
  }

  bool equal(PHTBidirectionalIterator_Key const& other) const
  {
    return this->it.equal(other.it);
  }

  Key& dereference() const {
    return this->it.dereference().first;
  }
public:
  friend class boost::iterator_core_access;

  PHTBidirectionalIterator_Key(ProbingHashTable<Key, Value, Method>* htable, long int i):
  it{htable, i}{}

  PHTBidirectionalIterator_Key(ProbingHashTable<Key, Value, Method>* htable):
  it{htable}{}
};

template <typename Key, typename Value, typename Method = LinearProbing>
class PHTBidirectionalIterator_Value:
    public boost::iterator_facade<PHTBidirectionalIterator_Value<Key, Value, Method>,
                                  Value&,
                                  std::bidirectional_iterator_tag> {
private:
  PHTBidirectionalIterator<Key, Value, Method> it;

  void increment() {
    this->it.increment();
  }

  void decrement() {
    this->it.decrement();
  }

  bool equal(PHTBidirectionalIterator_Value const& other) const
  {
    return this->it.equal(other.it);
  }

  Value& dereference() const {
    return this->it.dereference().second;
  }

public:
  friend class boost::iterator_core_access;

  PHTBidirectionalIterator_Value(ProbingHashTable<Key, Value, Method>* htable, long int i):
  it{htable, i}{}

  PHTBidirectionalIterator_Value(ProbingHashTable<Key, Value, Method>* htable):
  it{htable}{}
};

#endif
