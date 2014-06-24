#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

template <typename Key, typename Value>
class Dictionary{
public:
  virtual Value insert(const Key, Value&);
  virtual Value&& del(const Key);
  virtual Value& search(const Key);
  virtual int countValues();
  virtual double loadFactor();
};

#endif // DICTIONARY_HPP
