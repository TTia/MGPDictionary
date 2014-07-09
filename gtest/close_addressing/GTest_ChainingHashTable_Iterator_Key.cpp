#include "gtest/gtest.h"
#include "close_addressing/ChainingHashTable.hpp"
#include "close_addressing/ChainingHashTableIterator.hpp"

#include <stdexcept>

class GTest_Iterator_Key : public ::testing::Test {};

TEST_F(GTest_Iterator_Key, Iterator_over_empty_table) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h, .5};

  for(auto it = cht.begin_key(); it != cht.end_key(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_Iterator_Key, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h, .5};
  char a = 'a';
  cht.insert(1,a); cht.insert(2,a); cht.insert(3,a);

  int result = 0;
  for(auto it = cht.begin_key(); it != cht.end_key(); it++){
      result += *it;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_Iterator_Key, Iterate_over_table_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  ChainingHashTable<int, char> cht{h, .5, m};
  char a = 'a';
  cht.insert(1,a); cht.insert(2,a); cht.insert(3,a);
  cht.insert(m+1,a);

  int result = 0;
  for(auto it = cht.begin_key(); it != cht.end_key(); it++){
    result += *it;
    }
  ASSERT_EQ(6+(m+1), result);
}

TEST_F(GTest_Iterator_Key, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  ChainingHashTable<int, char> cht{h, .5, m};
  char a = 'a';
  cht.insert(1,a); cht.insert(m+1,a); cht.insert(16,a);

  CHTBidirectionalIterator_Key<int, char> it_1{&cht, 1, 0};
  CHTBidirectionalIterator_Key<int, char> it_2{&cht, 1, 1};
  CHTBidirectionalIterator_Key<int, char> it_3(&cht, 16, 0);
  CHTBidirectionalIterator_Key<int, char> it_err(&cht, 2, 0);

  int value = 1;
  ASSERT_EQ(value, *it_1);

  value = m+1;
  ASSERT_EQ(value, *it_2);

  value = 16;
  ASSERT_EQ(value, *it_3);

  ASSERT_EQ(it_err, cht.end_key());
}

TEST_F(GTest_Iterator_Key, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h, .5};
  char a = 'a';
  cht.insert(1,a); cht.insert(2,a); cht.insert(3,a);
  auto it = cht.begin();

  it++; it--;
  ASSERT_EQ(it, cht.begin());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, cht.begin());
}
