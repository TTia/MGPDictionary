#ifndef CHAININGHASHTABLEITERATOR_HPP
#define CHAININGHASHTABLEITERATOR_HPP

#include "Hashing.hpp"
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>

template<typename Key, typename Value, typename HashingMethod>
class ChainingHashTable;

template <typename Key, typename Value, typename HashingMethod = DivisionMethod>
class CHTBidirectionalIterator:
    public boost::iterator_facade<CHTBidirectionalIterator<Key, Value, HashingMethod>,
                                  std::pair<Key, Value>,
                                  std::bidirectional_iterator_tag> {
public:
  friend class boost::iterator_core_access;
  friend class ChainingHashTable<Key, Value, HashingMethod>;

  CHTBidirectionalIterator() = delete;
  CHTBidirectionalIterator(ChainingHashTable<Key, Value, HashingMethod>* htable, int i, size_t j = 0):
    table{htable->table}, m{htable->m}, i{i}, j(j)
  {
    this->_searchNextChain();
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

#endif
