#include "gtest/gtest.h"
#include "open_addressing/OpenAddressing.hpp"

class GTest_OpenAddressingDictionary_Rehashing : public ::testing::Test {};

TEST_F(GTest_OpenAddressingDictionary_Rehashing, Enlarging_1_Time) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h, .5, 16);
  ASSERT_EQ(0, pht.loadFactor());
  for(int i = 0; i<7; i++){
      pht.insert(i, 'a');
    }
  ASSERT_EQ(7, pht.countValues());
  ASSERT_EQ(7.0/16, pht.loadFactor());

  for(int i = 7; i<14; i++){
      pht.insert(i+99, 'b');
    }
  ASSERT_EQ(14, pht.countValues());
  ASSERT_EQ(14.0/32, pht.loadFactor());
}

TEST_F(GTest_OpenAddressingDictionary_Rehashing, Enlarging_3_Times) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h, .5, 16);
  ASSERT_EQ(0, pht.loadFactor());
  for(int i = 0; i<65; i++){
      pht.insert(i, 'a');
    }

  ASSERT_EQ(65, pht.countValues());
  ASSERT_EQ(65.0/128, pht.loadFactor());
}

TEST_F(GTest_OpenAddressingDictionary_Rehashing, Enlarging_3_Times_And_clear) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h, .5, 16);
  ASSERT_EQ(0, pht.loadFactor());
  for(int i = 0; i<65; i++){
      pht.insert(i, 'a');
    }

  ASSERT_EQ(65, pht.countValues());
  ASSERT_EQ(65.0/128, pht.loadFactor());

  for(int i = 0; i<64; i++){
      pht.del(i);
    }
  ASSERT_EQ(1, pht.countValues());
  ASSERT_EQ(1.0/16, pht.loadFactor());
}

TEST_F(GTest_OpenAddressingDictionary_Rehashing, Search_causes_complete_rehashing) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h, .5, 16);
  for(int i = 0; i<10; i++){
      pht.insert(i, 'a');
    }
  ASSERT_EQ(10.0/32, pht.loadFactor());
  pht.search(-1);
  ASSERT_EQ(10.0/32, pht.loadFactor());
}
