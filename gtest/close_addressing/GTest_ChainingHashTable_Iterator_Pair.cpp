#include "gtest/gtest.h"
#include "close_addressing/ChainingHashTable.hpp"
#include "close_addressing/ChainingHashTableIterator.hpp"

#include <stdexcept>

class GTest_Iterator_Pair : public ::testing::Test {
protected:
  ChainingHashTable<int, char> *table;
  virtual void SetUp() {
    table = nullptr;
  }

  virtual void TearDown() {
    if(table)
      delete table;
  }
};

TEST_F(GTest_Iterator_Pair, Iterator_over_empty_table) {
  std::hash<int> h;
  table = new ChainingHashTable<int, char, DivisionMethod>(h);

  for(auto it = table->begin(); it != table->end(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_Iterator_Pair, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  table = new ChainingHashTable<int, char, DivisionMethod>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);

  int result = 0;
  for(auto it = table->begin(); it != table->end(); it++){
    const ChainingHashTable<int, char, DivisionMethod>::Pair& current = *it;
    result += current.first;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_Iterator_Pair, Iterate_over_table_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  table = new ChainingHashTable<int, char, DivisionMethod>(h, 0.5, m);
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

TEST_F(GTest_Iterator_Pair, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  table = new ChainingHashTable<int, char, DivisionMethod>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(m+1,a); table->insert(16,a);
  auto it_1 = new CHTBidirectionalIterator<int, char, DivisionMethod>(table, 1, 0);
  auto it_2 = new CHTBidirectionalIterator<int, char, DivisionMethod>(table, 1, 1);
  auto it_3 = new CHTBidirectionalIterator<int, char, DivisionMethod>(table, 16, 0);
  auto it_err = new CHTBidirectionalIterator<int, char, DivisionMethod>(table, 2, 0);

  ChainingHashTable<int, char, DivisionMethod>::Pair& pair = **it_1;
  auto value = new std::pair<int, char>(1, a);
  ASSERT_EQ(*value, pair);

  value->first = m+1;
  pair = **it_2;
  ASSERT_EQ(*value, pair);

  value->first = 16;
  pair = **it_3;
  ASSERT_EQ(*value, pair);

  ASSERT_EQ(*it_err, table->end());
  delete it_1;
  delete it_2;
  delete it_3;
  delete it_err;
}

TEST_F(GTest_Iterator_Pair, Iterate_over_table_Operator_elision) {
  std::hash<int> h;
  table = new ChainingHashTable<int, char, DivisionMethod>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  auto it = table->begin();

  it++; it--;
  ASSERT_EQ(it, table->begin());
  it++; it++; it--; it++; it--; it--;
  ASSERT_EQ(it, table->begin());
}

TEST_F(GTest_Iterator_Pair, Inferior_and_Superior_Boundaries_EQ_End) {
  std::hash<int> h;
  table = new ChainingHashTable<int, char, DivisionMethod>(h);
  table->insert(1, 'a');
  auto it = table->begin(), it_2 = table->begin();

  ASSERT_NE(it, table->end());
  ASSERT_NE(it_2, table->end());

  it--;
  ASSERT_EQ(it, table->end());

  it_2++; it_2++;
  ASSERT_EQ(it_2, table->end());
}
