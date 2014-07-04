#include "gtest/gtest.h"
#include "ChainingHashTable.hpp"
#include "ChainingHashTableIterator.hpp"

#include <stdexcept>

class GTest_Iterator_Key : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(GTest_Iterator_Key, Iterator_over_empty_table) {
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);

  for(auto it = table->begin_key(); it != table->end_key(); it++){
        ASSERT_TRUE(false);
    }
  ASSERT_TRUE(true);
}

TEST_F(GTest_Iterator_Key, Iterate_over_table_Forward_order) {
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);

  int result = 0;
  for(auto it = table->begin_key(); it != table->end_key(); it++){
      result += *it;
    }
  ASSERT_EQ(6, result);
}

TEST_F(GTest_Iterator_Key, Iterate_over_table_Forward_order_Chain) {
  std::hash<int> h;
  int m = 17;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(2,a); table->insert(3,a);
  table->insert(m+1,a);

  int result = 0;
  for(auto it = table->begin_key(); it != table->end_key(); it++){
    result += *it;
    }
  ASSERT_EQ(6+(m+1), result);
}

TEST_F(GTest_Iterator_Key, Iterate_over_table_Iterator_explicit_position) {
  std::hash<int> h;
  int m = 17;
  auto table = new ChainingHashTable<int, char, DivisionMethod>(h, 0.5, m);
  char a = 'a';
  table->insert(1,a); table->insert(m+1,a); table->insert(16,a);
  auto it_1 = new CHTBidirectionalIterator_Key<int, char, DivisionMethod>(table, 1, 0);
  auto it_2 = new CHTBidirectionalIterator_Key<int, char, DivisionMethod>(table, 1, 1);
  auto it_3 = new CHTBidirectionalIterator_Key<int, char, DivisionMethod>(table, 16, 0);
  auto it_err = new CHTBidirectionalIterator_Key<int, char, DivisionMethod>(table, 2, 0);

  int value = 1;
  ASSERT_EQ(value, **it_1);

  value = m+1;
  ASSERT_EQ(value, **it_2);

  value = 16;
  ASSERT_EQ(value, **it_3);

  ASSERT_EQ(*it_err, table->end_key());
}

TEST_F(GTest_Iterator_Key, Iterate_over_table_Operator_elision) {
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
