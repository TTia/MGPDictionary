#ifndef CHAININGHASHTABLEITERATOR_HPP
#define CHAININGHASHTABLEITERATOR_HPP

#include <iterator>
#include <boost/iterator/iterator_facade.hpp>

template<typename Key, typename Value, typename HashingMethod>
class ChainingHashTable;

template <typename Key, typename Value, typename HashingMethod = KnuthDivisionMethod>
class ChainingHashTableBidirectionalIterator:
    public boost::iterator_facade<ChainingHashTableBidirectionalIterator<Key, Value, HashingMethod>,
                                  std::pair<Key, Value>,
                                  std::bidirectional_iterator_tag> {
public:
  ChainingHashTableBidirectionalIterator() = delete;
  ChainingHashTableBidirectionalIterator(ChainingHashTable<Key, Value, HashingMethod>* table, int i, int j){}

  friend class boost::iterator_core_access;
  friend class ChainingHashTable<Key, Value, HashingMethod>;
private:
  ChainingHashTable<Key, Value, HashingMethod> *table;

  long int i, j;

  void increment() {
  }

  bool equal(ChainingHashTableBidirectionalIterator const& other) const
  {
    std::cout << "equal" << std::endl;
    return this->data[i]->at(j) == other.data[i]->at(j);
  }

  ChainingHashTable::Pair& dereference() const {
    std::cout << "* deref" << std::endl;
    return this->data[i]->at(j);
  }
};

#endif
