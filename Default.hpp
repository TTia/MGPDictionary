#ifndef DEFAULT_HPP
#define DEFAULT_HPP

#include <boost/concept_check.hpp>
#include <assert.h>

class DefaultValues{
public:
  static constexpr double DEFAULT_UPPERLF = 0.5;
  static constexpr double DEFAULT_LOWERLF = 0.15;
  static constexpr long int DEFAULT_M = 17;
  static constexpr double REHASH_THRESHOLD_DEFAULT = 0.10;
};

template<typename Key>
class DefaultConstraints{
public:
  static void checkEqualityComparable(){
    BOOST_CONCEPT_ASSERT((boost::EqualityComparable<Key>));
  }
};

#endif // DEFAULT_HPP
