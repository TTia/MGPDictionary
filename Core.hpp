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
//  typedef typename Dictionary::Key Key;
//  typedef typename Dictionary::Value Value;
//  typedef typename Dictionary::Method Method;

  using Dictionary::Dictionary;

//  bool insert(const Key, const Value&, Value* = nullptr);

  bool del(const Key, Value* = nullptr);

//  iterator search(const Key);

  iterator_key begin_key();

  iterator_value begin_value();

  iterator_key end_key();

  iterator_value end_value();

  Value& operator[](const Key);

//  inline int countValues() const{
//    return this->to_n+this->from_n;
//  }

//  inline double loadFactor(){
//    return !(this->to_table? this->to_m: this->from_m) ?
//          0 : double(countValues())/ (this->to_table? this->to_m: this->from_m);
//  }

  inline int rehashThreshold(){
    return (this->to_table? this->to_m: this->from_m) * 0.10;
  }

private:

//  void _shrinkTable();

//  void _enlargeTable();

};

/*
 * Public Methods
 */
//template<typename Dictionary, typename Key, typename Value, typename Method>
//bool Core<Dictionary, Key, Value, Method>::insert(const Key key, const Value &value, Value* output){
//  if(loadFactor() > this->upperLF && !this->to_table){
//      _enlargeTable();
//    }
//  if(this->to_table){
//      this->_rehash(rehashThreshold());
//    }
//  this->updateVersion();
//  return this->to_table ?
//        this->_insert(this->to_table, &this->to_m, &this->to_n, key, value, output) :
//        this->_insert(this->from_table, &this->from_m, &this->from_n, key, value, output);
//}

template<typename Dictionary, typename Key, typename Value, typename Method>
bool Core<Dictionary, Key, Value, Method>::del(const Key key, Value* output){
  if(this->loadFactor() < this->lowerLF && !this->to_table){
      this->_shrinkTable();
    }
  if(this->to_table){
      this->_rehash(rehashThreshold());
    }
  return this->_del(this->from_table, &this->from_m, &this->from_n, key, output) ||
          (this->to_table && this->_del(this->to_table, &this->to_m, &this->to_n, key, output));
}

//template<typename Dictionary, typename Key, typename Value, typename Method>
//typename Core<Dictionary, Key, Value, Method>::iterator
//Core<Dictionary, Key, Value, Method>::search(const Key key){
//  if(this->to_table){
//      _rehash(this->from_n);
//    }
//  long int i = hm(this->from_m, h(key));
//  if(!this->from_table[i]){
//      return this->end();
//    }
//  for(size_t j = 0; j != from_table[i]->size(); j++) {
//      if(from_table[i]->at(j).first == key){
//          iterator it(this, i, j);
//          return it;
//        }
//    }
//  return this->end();
//}

//template<typename Dictionary, typename Key, typename Value, typename Method>
//typename Core<Dictionary, Key, Value, Method>::iterator
//Core<Dictionary, Key, Value, Method>::begin(){
//  if(!countValues())
//    return this->end();
//  if(this->to_table)
//    _rehash(this->from_n);
//  iterator it(this);
//  return it;
//}

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

//template<typename Dictionary, typename Key, typename Value, typename Method>
//typename Core<Dictionary, Key, Value, Method>::iterator
//Core<Dictionary, Key, Value, Method>::end(){
//  iterator it(this, -1);
//  return it;
//}

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

//template<typename Dictionary, typename Key, typename Value, typename Method>
//void Core<Dictionary, Key, Value, Method>::_enlargeTable(){
//  this->to_m = this->from_m * 2;
//  this->to_n = 0;
//  this->_alloca(this->to_m);
//}

//template<typename Dictionary, typename Key, typename Value, typename Method>
//void Core<Dictionary, Key, Value, Method>::_shrinkTable(){
//  if(this->from_m <= this->min_m){
//      return;
//    }
//  this->to_m = this->from_m / 2;
//  this->to_n = 0;
//  this->_alloca(this->to_m);
//}

#endif // CORE_HPP
