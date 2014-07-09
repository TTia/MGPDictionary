#include "gtest/gtest.h"
#include "close_addressing/ChainingHashTable.hpp"
#include "close_addressing/ChainingHashTableIterator.hpp"

#include <stdio.h>
#include <string.h>
#include <stdexcept>

class GTest_Iterator_Values : public ::testing::Test {};

TEST_F(GTest_Iterator_Values, Iterator_over_empty_cht) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h};

  for(auto it = cht.begin_value(); it != cht.end_value(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_Iterator_Values, Iterate_over_cht_Forward_order) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h};
  char a = 'a', b = 'b', c = 'c', result[4] = "";
  int i = 0;
  cht.insert(1,a); cht.insert(2,b); cht.insert(3,c);

  for(auto it = cht.begin_value(); it != cht.end_value(); it++, i++){
    result[i] = *it;
    }
  result[i] = '\0';
  ASSERT_EQ(strcmp("abc", result), 0);
}

TEST_F(GTest_Iterator_Values, Iterate_over_cht_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  ChainingHashTable<int, char> cht{h, .5, m};
  char a = 'a', b = 'b', c = 'c', d = 'd', result[5] = "";
  int i = 0;
  cht.insert(1,a); cht.insert(2,b); cht.insert(3,c);
  cht.insert(m+1,d);

  for(auto it = cht.begin_value(); it != cht.end_value(); it++, i++){
    result[i] = *it;
  }
  result[i] = '\0';
  ASSERT_EQ(strcmp("adbc", result), 0);
}

TEST_F(GTest_Iterator_Values, Iterate_over_cht_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  ChainingHashTable<int, char> cht{h};
  char a = 'a', b = 'b', c = 'c';
  cht.insert(1,a); cht.insert(m+1,b); cht.insert(16,c);
  CHTBidirectionalIterator_Value<int, char> it_1(&cht, 1, 0);
  CHTBidirectionalIterator_Value<int, char> it_2(&cht, 1, 1);
  CHTBidirectionalIterator_Value<int, char> it_3(&cht, 16, 0);
  CHTBidirectionalIterator_Value<int, char> it_err(&cht, 2, 0);

  char value = a;
  ASSERT_EQ(value, *it_1);

  value = b;
  ASSERT_EQ(value, *it_2);

  value = c;
  ASSERT_EQ(value, *it_3);

  ASSERT_EQ(it_err, cht.end_value());
}

TEST_F(GTest_Iterator_Values, Iterate_over_cht_Operator_elision) {
  std::hash<int> h;
  ChainingHashTable<int, char> cht{h};
  char a = 'a';
  cht.insert(1,a); cht.insert(2,a); cht.insert(3,a);
  auto it = cht.begin_value();

  it++; it--;
  ASSERT_EQ(it, cht.begin_value());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, cht.begin_value());
}
