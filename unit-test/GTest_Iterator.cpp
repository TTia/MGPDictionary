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
  std::hash<int> h;
  auto table = new ChainingHashTable<int, char>(h);
  char a = 'a';

  ASSERT_TRUE(true);
}
