#include "gtest/gtest.h"
#include "open_addressing/ProbingHashTableIterator.hpp"
#include "open_addressing/OpenAddressing.hpp"

#include <stdio.h>
#include <string.h>
#include <stdexcept>

class GTest_OpenAddressingDictionary_Iterator_Values : public ::testing::Test {};

TEST_F(GTest_OpenAddressingDictionary_Iterator_Values, Iterator_over_empty_table) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);

  for(auto it = pht.begin_value(); it != pht.end_value(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}


TEST_F(GTest_OpenAddressingDictionary_Iterator_Values, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a', b = 'b', c = 'c', result[4] = "";
  int i = 0;
  pht.insert(1,a); pht.insert(2,b); pht.insert(3,c);

  for(auto it = pht.begin_value(); it != pht.end_value(); it++, i++){
    result[i] = *it;
    }
  result[i] = '\0';
  ASSERT_EQ(strcmp("abc", result), 0);
}

TEST_F(GTest_OpenAddressingDictionary_Iterator_Values, Iterate_over_table_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a', b = 'b', c = 'c', d = 'd', result[5] = "";
  int i = 0;
  pht.insert(1,a); pht.insert(2,b); pht.insert(3,c);
  pht.insert(m+1,d);

  for(auto it = pht.begin_value(); it != pht.end_value(); it++, i++){
    result[i] = *it;
  }
  result[i] = '\0';
  ASSERT_EQ(strcmp("abcd", result), 0);
}

TEST_F(GTest_OpenAddressingDictionary_Iterator_Values, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  OpenAddressingDictionary<int, char> pht(h, .5, m);
  char a = 'a', b = 'b', c = 'c';
  pht.insert(1,a); pht.insert(m+1,b); pht.insert(16,c);
  PHTBidirectionalIterator_Value<int, char> it_1(&pht, 1);
  PHTBidirectionalIterator_Value<int, char> it_2(&pht, 2);
  PHTBidirectionalIterator_Value<int, char> it_3(&pht, 16);
  PHTBidirectionalIterator_Value<int, char> it_err(&pht, 0);

  char value = a;
  ASSERT_EQ(value, *it_1);

  value = b;
  ASSERT_EQ(value, *it_2);

  value = c;
  ASSERT_EQ(value, *it_3);

  ASSERT_EQ(it_err, pht.end_value());
}

TEST_F(GTest_OpenAddressingDictionary_Iterator_Values, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a';
  pht.insert(1,a); pht.insert(2,a); pht.insert(3,a);
  auto it = pht.begin_value();

  it++; it--;
  ASSERT_EQ(it, pht.begin_value());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, pht.begin_value());
}
