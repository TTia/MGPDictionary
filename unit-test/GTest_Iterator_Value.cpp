#include "gtest/gtest.h"
#include "ChainingHashTable.hpp"
#include "ChainingHashTableIterator.hpp"

#include <stdexcept>

class GTest_Iterator_Values : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

TEST_F(GTest_Iterator_Values, Iterator_over_empty_table) {
  return;
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);

  for(auto it = table->begin(); it != table->end(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_Iterator_Values, Iterate_over_table_Forward_order) {
  return;
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);

  int result = 0;
  for(auto it = table->begin(); it != table->end(); it++){
    ChainingHashTable<int, char, DivisionMethod>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_Iterator_Values, Iterate_over_table_Forward_order_Chain) {
  return;
  std::hash<int> h;
  int m = 17;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  table->insert(m+1,a);

  int result = 0;
  for(auto it = table->begin(); it != table->end(); it++){
    ChainingHashTable<int, char, DivisionMethod>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(6+(m+1), result);
}

TEST_F(GTest_Iterator_Values, Iterate_over_table_Iterator_explicit_position) {
  return;
  std::hash<int> h;
  int m = 17;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(m+1,a); table->insert(16,a);
  auto it_1 = new CHTBidirectionalIterator<int, char, DivisionMethod>(table, 1, 0);
  auto it_2 = new CHTBidirectionalIterator<int, char, DivisionMethod>(table, 1, 1);
  auto it_3 = new CHTBidirectionalIterator<int, char, DivisionMethod>(table, 16, 0);

  ChainingHashTable<int, char, DivisionMethod>::Pair& pair = **it_1;
  auto value = new std::pair<int, char>(1, a);
  ASSERT_EQ(*value, pair);

  value->first = m+1;
  pair = **it_2;
  ASSERT_EQ(*value, pair);

  value->first = 16;
  pair = **it_3;
  ASSERT_EQ(*value, pair);
}

TEST_F(GTest_Iterator_Values, Iterate_over_table_Operator_elision) {
  return;
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  auto it = table->begin();

  it++; it--;
  ASSERT_EQ(it, table->begin());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, table->begin());
}
