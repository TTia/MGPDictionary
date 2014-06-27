#ifndef CHAININGHASHTABLEITERATOR_HPP
#define CHAININGHASHTABLEITERATOR_HPP

#include <iterator>
#include <boost/iterator/iterator_facade.hpp>

#include "ChainingHashTable.hpp"

template <typename Key, typename Value>
class SpikeForwardIterator:
  public boost::iterator_facade<SpikeForwardIterator<Key, Value>, std::pair<Key, Value>, std::bidirectional_iterator_tag> {
  friend class ChainingHashTable<Key, Value>;
private:
  node_base* m_node;
public:
  node_iterator()
  : m_node(0)
  {}
  explicit node_iterator(node_base* p)
  : m_node(p)
  {}
};

#endif // CHAININGHASHTABLEITERATOR_HPP

/*
template <typename Key, typename Value>
class ChainingHashTableBidirectionalIterator_keys:
    public std::iterator<std::bidirectional_iterator_tag, Key>{
  friend class ChainingHashTable;
private:
public:
};

template <typename Key, typename Value>
class ChainingHashTableBidirectionalIterator_values:
    public std::iterator<std::bidirectional_iterator_tag, Value>{
  friend class ChainingHashTable;
private:
public:
};
*/
