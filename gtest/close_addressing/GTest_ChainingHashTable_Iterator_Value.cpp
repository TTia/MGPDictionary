#include "gtest/gtest.h"
#include "close_addressing/ChainingHashTable.hpp"
#include "close_addressing/ChainingHashTableIterator.hpp"

#include <stdio.h>
#include <string.h>
#include <stdexcept>

class GTest_Iterator_Values : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

TEST_F(GTest_Iterator_Values, Iterator_over_empty_table) {
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);

  for(auto it = table->begin_value(); it != table->end_value(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_Iterator_Values, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);
  char a = 'a', b = 'b', c = 'c', result[4] = "";
  int i = 0;
  table->insert(1,a); table->insert(2,b); table->insert(3,c);

  for(auto it = table->begin_value(); it != table->end_value(); it++, i++){
    result[i] = *it;
    }
  result[i] = '\0';
  ASSERT_EQ(strcmp("abc", result), 0);
}

TEST_F(GTest_Iterator_Values, Iterate_over_table_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h, 0.5, m);
  char a = 'a', b = 'b', c = 'c', d = 'd', result[5] = "";
  int i = 0;
  table->insert(1,a); table->insert(2,b); table->insert(3,c);
  table->insert(m+1,d);

  for(auto it = table->begin_value(); it != table->end_value(); it++, i++){
    result[i] = *it;
  }
  result[i] = '\0';
  ASSERT_EQ(strcmp("adbc", result), 0);
}

TEST_F(GTest_Iterator_Values, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h, 0.5, m);
  char a = 'a', b = 'b', c = 'c';
  table->insert(1,a); table->insert(m+1,b); table->insert(16,c);
  auto it_1 = new CHTBidirectionalIterator_Value<int, char, DivisionMethod>(table, 1, 0);
  auto it_2 = new CHTBidirectionalIterator_Value<int, char, DivisionMethod>(table, 1, 1);
  auto it_3 = new CHTBidirectionalIterator_Value<int, char, DivisionMethod>(table, 16, 0);
  auto it_err = new CHTBidirectionalIterator_Value<int, char, DivisionMethod>(table, 2, 0);

  char value = a;
  ASSERT_EQ(value, **it_1);

  value = b;
  ASSERT_EQ(value, **it_2);

  value = c;
  ASSERT_EQ(value, **it_3);

  ASSERT_EQ(*it_err, table->end_value());
}

TEST_F(GTest_Iterator_Values, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  auto it = table->begin_value();

  it++; it--;
  ASSERT_EQ(it, table->begin_value());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, table->begin_value());
}
