#include "gtest/gtest.h"
#include "close_addressing/ChainingHashTable.hpp"

#include <stdexcept>

class GTest_ChainingHashTable_Rehashing : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(GTest_ChainingHashTable_Rehashing, Enlarging_1_Time) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5, 16);
  ASSERT_EQ(0, cht->loadFactor());
  for(int i = 0; i<7; i++){
      cht->insert(i, 'a');
    }
  ASSERT_EQ(7, cht->countValues());
  ASSERT_EQ(7.0/16, cht->loadFactor());

  for(int i = 7; i<14; i++){
      cht->insert(i+99, 'b');
    }
  ASSERT_EQ(14, cht->countValues());
  ASSERT_EQ(14.0/32, cht->loadFactor());
}

TEST_F(GTest_ChainingHashTable_Rehashing, Enlarging_3_Times) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5, 16);
  ASSERT_EQ(0, cht->loadFactor());
  for(int i = 0; i<65; i++){
      cht->insert(i, 'a');
    }

  ASSERT_EQ(65, cht->countValues());
  ASSERT_EQ(65.0/128, cht->loadFactor());
}

TEST_F(GTest_ChainingHashTable_Rehashing, Enlarging_3_Times_And_clear) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5, 16);
  ASSERT_EQ(0, cht->loadFactor());
  for(int i = 0; i<65; i++){
      cht->insert(i, 'a');
    }

  ASSERT_EQ(65, cht->countValues());
  ASSERT_EQ(65.0/128, cht->loadFactor());

  for(int i = 0; i<64; i++){
      cht->del(i);
    }
  ASSERT_EQ(1, cht->countValues());
  ASSERT_EQ(1.0/16, cht->loadFactor());
}

TEST_F(GTest_ChainingHashTable_Rehashing, Search_causes_complete_rehashing) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5, 16);
  for(int i = 0; i<10; i++){
      cht->insert(i, 'a');
    }
  ASSERT_EQ(10.0/32, cht->loadFactor());
  cht->search(-1);
  ASSERT_EQ(10.0/32, cht->loadFactor());
}
