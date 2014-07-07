#include "gtest/gtest.h"
#include "open_addressing/ProbingHashTable.hpp"
#include "open_addressing/Probing.hpp"
#include <stdexcept>

class GTest_ProbingHashTable_Iterator_Pair : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterator_over_empty_table) {
  std::hash<int> h;
  auto table = new ProbingHashTable<int, char>(h);

  for(auto it = table->begin(); it != table->end(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  auto table = new ProbingHashTable<int, char>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  int result = 0;
  for(auto it = table->begin(); it != table->end(); it++){
    const ProbingHashTable<int, char>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_table_Forward_order_With_collisions) {
  std::hash<int> h;
  int m = 17;
  auto table = new ProbingHashTable<int, char>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  table->insert(m+1,a);

  int result = 0;
  for(auto it = table->begin(); it != table->end(); it++){
    ProbingHashTable<int, char, DivisionMethod>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(6+(m+1), result);
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  auto table = new ProbingHashTable<int, char>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(m+1,a); table->insert(16,a);
  auto it_1 = new PHTBidirectionalIterator<int, char>(table, 1);
  auto it_2 = new PHTBidirectionalIterator<int, char>(table, 2);
  auto it_3 = new PHTBidirectionalIterator<int, char>(table, 16);
  auto it_err = new PHTBidirectionalIterator<int, char>(table, 3);

  auto value = new std::pair<int, char>(1, a);

  ASSERT_EQ(*value, **it_1);

  value->first = m+1;
  ASSERT_EQ(*value, **it_2);

  value->first = 16;
  ASSERT_EQ(*value, **it_3);

  ASSERT_EQ(*it_err, table->end());
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  auto table = new ProbingHashTable<int, char>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  auto it = table->begin();

  it++; it--;
  ASSERT_EQ(it, table->begin());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, table->begin());
}

TEST_F(GTest_ProbingHashTable_Iterator_Pair, Inferior_and_Superior_Boundaries_EQ_End) {
  std::hash<int> h;
  auto table = new ProbingHashTable<int, char>(h);
  table->insert(1, 'a');
  auto it = table->begin(),
      it_2 = table->begin(),
      it_3 = table->begin();

  ASSERT_NE(it, table->end());
  ASSERT_NE(it_2, table->end());
  ASSERT_NE(it_3, table->end());

  it--;
  ASSERT_EQ(it, table->end());

  it_2++;
  it_2++;
  ASSERT_EQ(it_2, table->end());

  it_3++; it_3--;
  ASSERT_EQ(it_3, table->end());
}
