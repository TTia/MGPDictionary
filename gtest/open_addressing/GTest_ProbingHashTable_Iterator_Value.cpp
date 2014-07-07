#include "gtest/gtest.h"
#include "open_addressing/ProbingHashTableIterator.hpp"
#include "open_addressing/ProbingHashTable.hpp"

#include <stdio.h>
#include <string.h>
#include <stdexcept>

class GTest_ProbingHashTable_Iterator_Values : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};
TEST_F(GTest_ProbingHashTable_Iterator_Values, Iterator_over_empty_table) {
  std::hash<int> h;
  auto table = new ProbingHashTable<int, char>(h);

  for(auto it = table->begin_value(); it != table->end_value(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}


TEST_F(GTest_ProbingHashTable_Iterator_Values, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  auto table = new ProbingHashTable<int, char>(h);
  char a = 'a', b = 'b', c = 'c', result[4] = "";
  int i = 0;
  table->insert(1,a); table->insert(2,b); table->insert(3,c);

  for(auto it = table->begin_value(); it != table->end_value(); it++, i++){
    result[i] = *it;
    }
  result[i] = '\0';
  ASSERT_EQ(strcmp("abc", result), 0);
}

TEST_F(GTest_ProbingHashTable_Iterator_Values, Iterate_over_table_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  auto table = new ProbingHashTable<int, char>(h, 0.5, m);
  char a = 'a', b = 'b', c = 'c', d = 'd', result[5] = "";
  int i = 0;
  table->insert(1,a); table->insert(2,b); table->insert(3,c);
  table->insert(m+1,d);

  for(auto it = table->begin_value(); it != table->end_value(); it++, i++){
    result[i] = *it;
  }
  result[i] = '\0';
  ASSERT_EQ(strcmp("abcd", result), 0);
}

TEST_F(GTest_ProbingHashTable_Iterator_Values, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  auto table = new ProbingHashTable<int, char>(h, 0.5, m);
  char a = 'a', b = 'b', c = 'c';
  table->insert(1,a); table->insert(m+1,b); table->insert(16,c);
  auto it_1 = new PHTBidirectionalIterator_Value<int, char>(table, 1);
  auto it_2 = new PHTBidirectionalIterator_Value<int, char>(table, 2);
  auto it_3 = new PHTBidirectionalIterator_Value<int, char>(table, 16);
  auto it_err = new PHTBidirectionalIterator_Value<int, char>(table, 3);

  char value = a;
  ASSERT_EQ(value, **it_1);

  value = b;
  ASSERT_EQ(value, **it_2);

  value = c;
  ASSERT_EQ(value, **it_3);

  ASSERT_EQ(*it_err, table->end_value());
}

TEST_F(GTest_ProbingHashTable_Iterator_Values, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  auto table = new ProbingHashTable<int, char>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  auto it = table->begin_value();

  it++; it--;
  ASSERT_EQ(it, table->begin_value());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, table->begin_value());
}
