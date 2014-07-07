#ifndef CHAININGHASHTABLEITERATOR_HPP
#define CHAININGHASHTABLEITERATOR_HPP

#include "../Hashing.hpp"
#include "ChainingHashTable.hpp"
#include <memory>
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>
#include <stdexcept>

template<typename Key, typename Value, typename HashingMethod>
class ChainingHashTable;

template<typename Key, typename Value, typename ProbingMethod>
class CHTBidirectionalIterator_Key;
template<typename Key, typename Value, typename ProbingMethod>
class CHTBidirectionalIterator_Value;

template <typename Key, typename Value, typename HashingMethod = DivisionMethod>
class CHTBidirectionalIterator:
    public boost::iterator_facade<CHTBidirectionalIterator<Key, Value, HashingMethod>,
                                  typename ChainingHashTable<Key, Value, HashingMethod>::Pair&,
                                  std::bidirectional_iterator_tag> {
public:
  friend class boost::iterator_core_access;
  friend class CHTBidirectionalIterator_Key<Key, Value, HashingMethod>;
  friend class CHTBidirectionalIterator_Value<Key, Value, HashingMethod>;

  CHTBidirectionalIterator() = delete;
  CHTBidirectionalIterator(ChainingHashTable<Key, Value, HashingMethod>* htable, int i, size_t j = 0):
    table{htable->from_table}, m{htable->from_m}, i{i}, j(j), version{htable->version}, originalVersion{*htable->version}
  {
    validate();
    if(i<0 || i>=m || !table[i] || j > table[i]->size()){
        this->i = -1;
        this->j = 0;
      }
  }

  CHTBidirectionalIterator(ChainingHashTable<Key, Value, HashingMethod>* htable):
    table{htable->from_table}, m{htable->from_m}, i{0}, j(0), version{htable->version}, originalVersion{*htable->version}
  {
    validate();
    this->_searchNextChain();
  }
private:
  typename ChainingHashTable<Key, Value, HashingMethod>::Table* table;
  long int m, i;
  size_t j;
  std::shared_ptr<long int> version;
  long int originalVersion;

  inline bool isEnd() const{
    return i == -1 && j == 0;
  }
  inline void endIteration(){
    i = -1;
    j = 0;
  }
  inline bool isValid() const{
    return *version == originalVersion;
  }
  inline bool checkBoundaries() const{
    return i >= 0 && i < m;
  }
  void validate(){
    if(!isValid()){
        endIteration();
      }
  }

  void increment() {
    validate();

    if(!checkBoundaries()){
        endIteration();
        return;
      }
    j++;
    if(j >= 0 && j < table[i]->size()){
      return;
    }
    i++;
    this->_searchNextChain();
  }
  void _searchNextChain(){
    j = 0;
    while(checkBoundaries() && !table[i]){
        i++;
      }
    if(!checkBoundaries()){
        endIteration();
        return;
      }
  }

  void decrement() {
    validate();

    if(!checkBoundaries()){
        endIteration();
        return;
      }
    j--;
    if(j > 0 && j < table[i]->size()){
      return;
    }
    i--;
    this->_searchPreviousChain();
  }

  void _searchPreviousChain(){
    while(checkBoundaries() && !table[i]){
        i--;
      }
    if(!checkBoundaries()){
        endIteration();
        return;
      }
    j = table[i]->size() - 1;
  }

  bool equal(CHTBidirectionalIterator const& other) const
  {
    if((!isValid() || isEnd()) && (!other.isValid() || other.isEnd())){
        return table == other.table;
    }
    return table == other.table && i == other.i && j == other.j;
  }

  typename ChainingHashTable<Key, Value, HashingMethod>::Pair& dereference() const {
    if(!isValid()){
        throw std::logic_error("Invalid iterator.");
      }else if(isEnd()){
        throw std::out_of_range("This iterator has reach the end.");
      }else if(!table[i]){
        throw std::out_of_range("Seg-fault. Sorry!");
      }
    return table[i]->at(j);
  }
};

template <typename Key, typename Value, typename HashingMethod = DivisionMethod>
class CHTBidirectionalIterator_Key:
    public boost::iterator_facade<CHTBidirectionalIterator_Key<Key, Value, HashingMethod>,
                                  Key&,
                                  std::bidirectional_iterator_tag> {
private:
  CHTBidirectionalIterator<Key, Value, HashingMethod>* it;

  void increment() {
    this->it->increment();
  }

  void decrement() {
    this->it->decrement();
  }

  bool equal(CHTBidirectionalIterator_Key const& other) const
  {
    return this->it->equal(*other.it);
  }

  Key& dereference() const {
    return this->it->dereference().first;
  }
public:
  typedef CHTBidirectionalIterator<Key, Value, HashingMethod> super;
  friend class boost::iterator_core_access;
  friend class ChainingHashTable<Key, Value, HashingMethod>;

  CHTBidirectionalIterator_Key(ChainingHashTable<Key, Value, HashingMethod>* htable, int i, size_t j = 0):
  it{new CHTBidirectionalIterator<Key, Value, HashingMethod>(htable, i, j)}{}

  CHTBidirectionalIterator_Key(ChainingHashTable<Key, Value, HashingMethod>* htable):
  it{new CHTBidirectionalIterator<Key, Value, HashingMethod>(htable)}{}
};

template <typename Key, typename Value, typename HashingMethod = DivisionMethod>
class CHTBidirectionalIterator_Value:
    public boost::iterator_facade<CHTBidirectionalIterator_Value<Key, Value, HashingMethod>,
                                  Value&,
                                  std::bidirectional_iterator_tag> {
private:
  CHTBidirectionalIterator<Key, Value, HashingMethod>* it;

  void increment() {
    //Verifica stato valore
    this->it->increment();
  }

  void decrement() {
    //Verifica stato valore
    this->it->decrement();
  }

  bool equal(CHTBidirectionalIterator_Value const& other) const
  {
    return this->it->equal(*other.it);
  }

  Value& dereference() const {
    //Aggiungo (?!)l'impronta(?!) del valore al set.
    return this->it->dereference().second;
  }

public:
  typedef CHTBidirectionalIterator<Key, Value, HashingMethod> super;
  friend class boost::iterator_core_access;
  friend class ChainingHashTable<Key, Value, HashingMethod>;

  CHTBidirectionalIterator_Value(ChainingHashTable<Key, Value, HashingMethod>* htable, int i, size_t j = 0):
  it{new CHTBidirectionalIterator<Key, Value, HashingMethod>(htable, i, j)}{}

  CHTBidirectionalIterator_Value(ChainingHashTable<Key, Value, HashingMethod>* htable):
  it{new CHTBidirectionalIterator<Key, Value, HashingMethod>(htable)}{}
};

#endif
