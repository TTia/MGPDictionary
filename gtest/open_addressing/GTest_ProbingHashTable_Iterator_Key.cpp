#include "gtest/gtest.h"
#include "open_addressing/ProbingHashTable.hpp"
#include "open_addressing/ProbingHashTableIterator.hpp"
#include <stdexcept>

class GTest_ProbingHashTable_Iterator_Key : public ::testing::Test {
protected:
  ProbingHashTable<int, char> *table;
  virtual void SetUp() {
    table = nullptr;
  }

  virtual void TearDown() {
    if(table)
      delete table;
  }
};

TEST_F(GTest_ProbingHashTable_Iterator_Key, Iterator_over_empty_table) {
  std::hash<int> h;
  table = new ProbingHashTable<int, char>(h);

  for(auto it = table->begin_key(); it != table->end_key(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_ProbingHashTable_Iterator_Key, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  table = new ProbingHashTable<int, char>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);

  int result = 0;
  for(auto it = table->begin_key(); it != table->end_key(); it++){
      result += *it;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_ProbingHashTable_Iterator_Key, Iterate_over_table_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  table = new ProbingHashTable<int, char>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  table->insert(m+1,a);

  int result = 0;
  for(auto it = table->begin_key(); it != table->end_key(); it++){
    result += *it;
    }
  ASSERT_EQ(6+(m+1), result);
}

TEST_F(GTest_ProbingHashTable_Iterator_Key, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  table = new ProbingHashTable<int, char>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(m+1,a); table->insert(16,a);
  auto it_1 = new PHTBidirectionalIterator_Key<int, char>(table, 1);
  auto it_2 = new PHTBidirectionalIterator_Key<int, char>(table, 2);
  auto it_3 = new PHTBidirectionalIterator_Key<int, char>(table, 16);
  auto it_err = new PHTBidirectionalIterator_Key<int, char>(table, 0);

  int value = 1;
  ASSERT_EQ(value, **it_1);

  value = m+1;
  ASSERT_EQ(value, **it_2);

  value = 16;
  ASSERT_EQ(value, **it_3);

  ASSERT_EQ(*it_err, table->end_key());
  delete it_1;
  delete it_2;
  delete it_3;
  delete it_err;
}

TEST_F(GTest_ProbingHashTable_Iterator_Key, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  table = new ProbingHashTable<int, char>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  auto it = table->begin();

  it++; it--;
  ASSERT_EQ(it, table->begin());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, table->begin());
}
