#include "gtest/gtest.h"
#include "open_addressing/ProbingHashTable.hpp"
#include "open_addressing/Probing.hpp"
#include <stdexcept>

class GTest_ProbingHashTable_Iterator_Pair : public ::testing::Test {};

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterator_over_empty_table) {
  std::hash<int> h;
  ProbingHashTable<int, char> pht(h);

  for(auto it = pht.begin(); it != pht.end(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  ProbingHashTable<int, char> pht(h);
  char a = 'a';
  pht.insert(1,a); pht.insert(2,a); pht.insert(3,a);
  int result = 0;
  for(auto it = pht.begin(); it != pht.end(); it++){
    const ProbingHashTable<int, char>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(6, result);
}
TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_cht_ForEach) {
  std::hash<int> h;
  ProbingHashTable<int, char> pht(h);
  char a = 'a';
  pht.insert(1,a); pht.insert(2,a); pht.insert(3,a);
  int result = 0;
  for(auto current: pht){
    result += current.first;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_table_Forward_order_With_collisions) {
  std::hash<int> h;
  int m = 17;
  ProbingHashTable<int, char> pht(h, 0.5, m);
  pht.insert(0,'a'); pht.insert(2,'b'); pht.insert(3,'c');
  pht.insert(m+1,'d');

  int result = 0;
  for(auto it = pht.begin(); it != pht.end(); it++){
    ProbingHashTable<int, char, DivisionMethod>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(5+(m+1), result);
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  ProbingHashTable<int, char> pht(h, 0.5, m);
  char a = 'a';
  pht.insert(1,a); pht.insert(m+1,a); pht.insert(16,a);
  PHTBidirectionalIterator<int, char> it_1(&pht, 1);
  PHTBidirectionalIterator<int, char> it_2(&pht, 2);
  PHTBidirectionalIterator<int, char> it_3(&pht, 16);
  PHTBidirectionalIterator<int, char> it_err(&pht, 3);

  std::pair<int, char> value(1, a);

  ASSERT_EQ(value, *it_1);

  value.first = m+1;
  ASSERT_EQ(value, *it_2);

  value.first = 16;
  ASSERT_EQ(value, *it_3);

  ASSERT_EQ(it_err, pht.end());
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  ProbingHashTable<int, char> pht(h);
  char a = 'a';
  pht.insert(1,a); pht.insert(2,a); pht.insert(3,a);
  auto it = pht.begin();

  it++; it--;
  ASSERT_EQ(it, pht.begin());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, pht.begin());
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Inferior_and_Superior_Boundaries_EQ_End) {
  std::hash<int> h;
  ProbingHashTable<int, char> pht(h);
  pht.insert(1, 'a');
  auto it = pht.begin(),
      it_2 = pht.begin(),
      it_3 = pht.begin();

  ASSERT_NE(it, pht.end());
  ASSERT_NE(it_2, pht.end());
  ASSERT_NE(it_3, pht.end());

  it--;
  ASSERT_EQ(it, pht.end());

  it_2++;
  it_2++;
  ASSERT_EQ(it_2, pht.end());

  it_3++; it_3--;
  ASSERT_EQ(it_3, pht.end());
}
