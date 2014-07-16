#ifndef CORE_HPP
#define CORE_HPP

template <typename Dictionary, typename Key, typename Value, typename Method>
class Core: public Dictionary{
public:
  typedef typename Dictionary::Hash Hash;
  typedef typename Dictionary::Pair Pair;
  typedef typename Dictionary::Table Table;
  typedef typename Dictionary::iterator iterator;
  typedef typename Dictionary::iterator_key iterator_key;
  typedef typename Dictionary::iterator_value iterator_value;

  using Dictionary::Dictionary;

  bool del(const Key, Value* = nullptr);

  iterator_key begin_key();

  iterator_value begin_value();

  iterator_key end_key();

  iterator_value end_value();

  Value& operator[](const Key);

private:

};

/*
 * Public Methods
 */

template<typename Dictionary, typename Key, typename Value, typename Method>
bool Core<Dictionary, Key, Value, Method>::del(const Key key, Value* output){
  if(this->loadFactor() < this->lowerLF && !this->to_table){
      this->_shrinkTable();
    }
  if(this->to_table){
      this->_rehash(this->rehashThreshold());
    }
  return this->_del(this->from_table, &this->from_m, &this->from_n, key, output) ||
          (this->to_table && this->_del(this->to_table, &this->to_m, &this->to_n, key, output));
}

template<typename Dictionary, typename Key, typename Value, typename Method>
typename Core<Dictionary, Key, Value, Method>::iterator_key
Core<Dictionary, Key, Value, Method>::begin_key(){
  if(!this->countValues())
    return this->end_key();
  if(this->to_table)
    this->_rehash(this->from_n);
  iterator_key it(this);
  return it;
}

template<typename Dictionary, typename Key, typename Value, typename Method>
typename Core<Dictionary, Key, Value, Method>::iterator_value
Core<Dictionary, Key, Value, Method>::begin_value(){
  if(!this->countValues())
    return this->end_value();
  if(this->to_table)
    this->_rehash(this->from_n);
  iterator_value it(this);
  return it;
}

template<typename Dictionary, typename Key, typename Value, typename Method>
typename Core<Dictionary, Key, Value, Method>::iterator_key
Core<Dictionary, Key, Value, Method>::end_key(){
  iterator_key it(this, -1);
  return it;
}

template<typename Dictionary, typename Key, typename Value, typename Method>
typename Core<Dictionary, Key, Value, Method>::iterator_value
Core<Dictionary, Key, Value, Method>::end_value(){
  iterator_value it(this, -1);
  return it;
}

template<typename Dictionary, typename Key, typename Value, typename Method>
Value& Core<Dictionary, Key, Value, Method>::operator[](const Key key){
  iterator it = this->search(key);
  if(it == this->end()){
      throw std::out_of_range("No such key.");
    }
  return (*it).second;
}

/*
 * Private Methods
 */

#endif // CORE_HPP
