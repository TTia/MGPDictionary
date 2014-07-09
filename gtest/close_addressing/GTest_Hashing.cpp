#include "gtest/gtest.h"
#include "Hashing.hpp"

#include <stdexcept>

class GTest_Hashing : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

TEST_F(GTest_Hashing, DISABLED_Deterministic_Hashing_DivisionMethod) {
  DivisionMethod method;
  for(long int m = 1; m < 100; m++)
    for(long int k = 0; k < 8388608; k++){
        long int i = method(m,k);
        ASSERT_EQ(i, method(m,k));
        ASSERT_TRUE(i >= 0 && i < m);
    }
}

TEST_F(GTest_Hashing, DISABLED_Deterministic_Hashing_KnuthDivisionMethod) {
  KnuthDivisionMethod method;
  for(long int m = 1; m < 100; m++)
    for(long int k = 0; k < 8388608; k++){
      long int i = method(m,k);
      ASSERT_EQ(i, method(m,k));
      ASSERT_TRUE(i >= 0 && i < m);
  }
}

TEST_F(GTest_Hashing, DISABLED_Deterministic_Hashing_MultiplicationMethod) {
  MultiplicationMethod method;
  for(int m = 1; m < 10; m++)
    for(long int k = 0; k < 8388608; k++){
      ASSERT_EQ(method(m,k), method(m,k));
  }
}

TEST_F(GTest_Hashing, DISABLED_Deterministic_Hashing_UniversalMethod) {
  UniversalMethod method;
  for(long int m = 1; m < 100; m++)
    for(long int k = 0; k < 8388608; k++){
        long int i = method(m,k);
        ASSERT_EQ(i, method(m,k));
        ASSERT_TRUE(i >= 0 && i < m);
    }
}
