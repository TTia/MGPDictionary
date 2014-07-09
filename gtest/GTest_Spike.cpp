#include "gtest/gtest.h"
#include "close_addressing/ChainingHashTable.hpp"
#include "open_addressing/ProbingHashTable.hpp"

template <typename T>
class GTest_Spike : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

//typedef ::testing::Types<ChainingHashTable<int, char>, ProbingHashTable<int, char>> Types;
