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
  CHTBidirectionalIterator(ChainingHashTable<Key, Value, HashingMethod>* htable, int i/*, int j*/):
    table{htable->table}, m{htable->m}, i{i}/*, j{j}*/, it{nullptr}
  {
    this->_searchNextChain();
  }
  CHTBidirectionalIterator(ChainingHashTable<Key, Value, HashingMethod>* htable):
    table{htable->table}, m{htable->m}, i{0}/*, j{0}*/, it{nullptr}
  {
    this->_searchNextChain();
  }
private:
  typedef typename ChainingHashTable<Key, Value, HashingMethod>::__vector::iterator slave_iterator;
  typename ChainingHashTable<Key, Value, HashingMethod>::Table* table;
  long int m, i;
//  , j;
  slave_iterator it;

  void increment() {
    it++;
//    j++;
    if(table[i]->end() != it){
      return;
    }
    i++;
    this->_searchNextChain();
  }
  void _searchNextChain(){
    while(i < m && i >= 0 && !table[i]){
        i++;
      }
    if(i >= m){
        i = m;
//        j = 0;
        return;
      }
//    if(table[i]){
//        std::cout << table[i]->size() << "x" << std::endl;
        it = table[i]->begin();
        //j = 0;
//      }
  }

  void decrement() {
    it--;
    if(table[i]->end() != it){
      return;
    }
    i--;
    this->_searchPreviousChain();
  }
  void _searchPreviousChain(){
    while(i < m && i >= 0 && !table[i]){
        i--;
      }
    if(i >= m || i <= -1){
        i = m;
        return;
      }
//    it = table[i]->begin();
    it = table[i]->end();
    it--;
  }

  bool equal(CHTBidirectionalIterator const& other) const
  {
    return table == other.table && i == other.i && it == other.it/* && j == other.j*/;
  }

  typename ChainingHashTable<Key, Value, HashingMethod>::Pair& dereference() const {
//    return this->table[i]->at(j);
//    typename ChainingHashTable<Key, Value, HashingMethod>::Pair& pair = *it;
//    return pair;
    return *it;
  }
};

#endif
