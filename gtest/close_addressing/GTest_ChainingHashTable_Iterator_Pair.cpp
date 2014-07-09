#include "gtest/gtest.h"
#include "close_addressing/ChainingHashTable.hpp"
#include "close_addressing/ChainingHashTableIterator.hpp"

#include <stdexcept>

class GTest_Iterator_Pair : public ::testing::Test {};

TEST_F(GTest_Iterator_Pair, Iterator_over_empty_cht) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h, .5};

  for(auto it = cht.begin(); it != cht.end(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_Iterator_Pair, Iterate_over_cht_Forward_order) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h, .5};
  char a = 'a';
  cht.insert(1,a); cht.insert(2,a); cht.insert(3,a);

  int result = 0;
  for(auto it = cht.begin(); it != cht.end(); it++){
    const ChainingHashTable<int, char>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_Iterator_Pair, Iterate_over_cht_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  ChainingHashTable<int, char> cht{h, .5};
  char a = 'a';
  cht.insert(1,a); cht.insert(2,a); cht.insert(3,a);
  cht.insert(m+1,a);

  int result = 0;
  for(auto it = cht.begin(); it != cht.end(); it++){
    ChainingHashTable<int, char>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(6+(m+1), result);
}

TEST_F(GTest_Iterator_Pair, Iterate_over_cht_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  ChainingHashTable<int, char> cht{h, .5};
  char a = 'a';
  cht.insert(1,a); cht.insert(m+1,a); cht.insert(16,a);
  CHTBidirectionalIterator<int, char> it_1(&cht, 1, 0);
  CHTBidirectionalIterator<int, char> it_2(&cht, 1, 1);
  CHTBidirectionalIterator<int, char> it_3(&cht, 16, 0);
  CHTBidirectionalIterator<int, char> it_err(&cht, 2, 0);

  ChainingHashTable<int, char>::Pair& pair = *it_1;
  std::pair<int, char> value(1, a);
  ASSERT_EQ(value, pair);

  value.first = m+1;
  pair = *it_2;
  ASSERT_EQ(value, pair);

  value.first = 16;
  pair = *it_3;
  ASSERT_EQ(value, pair);

  ASSERT_EQ(it_err, cht.end());
}

TEST_F(GTest_Iterator_Pair, Iterate_over_cht_Operator_elision) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h};
  char a = 'a';
  cht.insert(1,a); cht.insert(2,a); cht.insert(3,a);
  auto it = cht.begin();

  it++; it--;
  ASSERT_EQ(it, cht.begin());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, cht.begin());
}

TEST_F(GTest_Iterator_Pair, Inferior_and_Superior_Boundaries_EQ_End) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h};
  cht.insert(1, 'a');
  auto it = cht.begin(), it_2 = cht.begin();

  ASSERT_NE(it, cht.end());
  ASSERT_NE(it_2, cht.end());

  it--;
  ASSERT_EQ(it, cht.end());

  it_2++; it_2++;
  ASSERT_EQ(it_2, cht.end());
}
