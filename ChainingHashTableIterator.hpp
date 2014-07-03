#ifndef CHAININGHASHTABLEITERATOR_HPP
#define CHAININGHASHTABLEITERATOR_HPP

#include "Hashing.hpp"
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>

template<typename Key, typename Value, typename HashingMethod>
class ChainingHashTable;
template<typename Key, typename Value, typename HashingMethod>
class CHTBidirectionalIterator_Key;
template<typename Key, typename Value, typename HashingMethod>
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
//  friend class ChainingHashTable<Key, Value, HashingMethod>;

  CHTBidirectionalIterator() = delete;
  CHTBidirectionalIterator(ChainingHashTable<Key, Value, HashingMethod>* htable, int i, size_t j = 0):
    table{htable->table}, m{htable->m}, i{i}, j(j)
  {
    if(i<0 || i>=m || !table[i] || j > table[i]->size()){
        this->i = m;
        this->j = 0;
      }
//    this->_searchNextChain();
  }
  CHTBidirectionalIterator(ChainingHashTable<Key, Value, HashingMethod>* htable):
    table{htable->table}, m{htable->m}, i{0}, j(0)
  {
    this->_searchNextChain();
  }
private:
  typename ChainingHashTable<Key, Value, HashingMethod>::Table* table;
  long int m, i;
  size_t j;

  void increment() {
    j++;
    if(j >= 0 && j < table[i]->size()){
      return;
    }
    i++;
    this->_searchNextChain();
  }
  void _searchNextChain(){
    j = 0;
    while(i >= 0 && i < m && !table[i]){
        i++;
      }
    if(i < 0 || i >= m){
        i = m;
        return;
      }
  }

  void decrement() {
    j--;
    if(j > 0 && j < table[i]->size()){
      return;
    }
    i--;
    this->_searchPreviousChain();
  }

  void _searchPreviousChain(){
    while(i < m && i >= 0 && !table[i]){
        i--;
      }
    if(i < 0 || i >= m){
        i = m;
        j = 0;
        return;
      }
    j = table[i]->size() - 1;
  }

  bool equal(CHTBidirectionalIterator const& other) const
  {
    return table == other.table && i == other.i && j == other.j;
  }

  typename ChainingHashTable<Key, Value, HashingMethod>::Pair& dereference() const {
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
    this->it->increment();
  }

  void decrement() {
    this->it->decrement();
  }

  bool equal(CHTBidirectionalIterator_Value const& other) const
  {
    return this->it->equal(*other.it);
  }

  Value& dereference() const {
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
