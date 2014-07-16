#include "gtest/gtest.h"
#include "open_addressing/OpenAddressing.hpp"
#include "open_addressing/ProbingHashTableIterator.hpp"
#include <stdexcept>

class GTest_OpenAddressingDictionary_Iterator_Key : public ::testing::Test {};

TEST_F(GTest_OpenAddressingDictionary_Iterator_Key, Iterator_over_empty_table) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);

  for(auto it = pht.begin_key(); it != pht.end_key(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_OpenAddressingDictionary_Iterator_Key, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a';
  pht.insert(1,a); pht.insert(2,a); pht.insert(3,a);

  int result = 0;
  for(auto it = pht.begin_key(); it != pht.end_key(); it++){
      result += *it;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_OpenAddressingDictionary_Iterator_Key, Iterate_over_table_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  OpenAddressingDictionary<int, char> pht(h, 0.5, m);
  char a = 'a';
  pht.insert(1,a); pht.insert(2,a); pht.insert(3,a);
  pht.insert(m+1,a);

  int result = 0;
  for(auto it = pht.begin_key(); it != pht.end_key(); it++){
    result += *it;
    }
  ASSERT_EQ(6+(m+1), result);
}

TEST_F(GTest_OpenAddressingDictionary_Iterator_Key, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  OpenAddressingDictionary<int, char> pht(h, 0.5, m);
  char a = 'a';
  pht.insert(1,a); pht.insert(m+1,a); pht.insert(16,a);
  PHTBidirectionalIterator_Key<int, char> it_1(&pht, 1);
  PHTBidirectionalIterator_Key<int, char> it_2(&pht, 2);
  PHTBidirectionalIterator_Key<int, char> it_3(&pht, 16);
  PHTBidirectionalIterator_Key<int, char> it_err(&pht, 0);

  int value = 1;
  ASSERT_EQ(value, *it_1);

  value = m+1;
  ASSERT_EQ(value, *it_2);

  value = 16;
  ASSERT_EQ(value, *it_3);

  ASSERT_EQ(it_err, pht.end_key());
}

TEST_F(GTest_OpenAddressingDictionary_Iterator_Key, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a';
  pht.insert(1,a); pht.insert(2,a); pht.insert(3,a);
  auto it = pht.begin();

  it++; it--;
  ASSERT_EQ(it, pht.begin());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, pht.begin());
}
