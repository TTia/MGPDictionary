#include "gtest/gtest.h"
#include "ChainingHashTable.hpp"
#include "ChainingHashTableIterator.hpp"

#include <stdexcept>

class GTest_Iterator : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

TEST_F(GTest_Iterator, Iterator_over_empty_table) {
  return;
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);

  for(auto it = table->begin(); it != table->end(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_Iterator, Iterate_over_table_Forward_order) {
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

TEST_F(GTest_Iterator, Iterate_over_table_Iterator_i_and_j) {
  return;
}

TEST_F(GTest_Iterator, Iterate_over_table_Operator_elision) {
  return;
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  auto it = table->begin();

  it++; it--;
  ASSERT_EQ(it, table->begin());
  it++; it++; it--; it++; it--; it--; it--;
  ASSERT_EQ(it, table->begin());
}
