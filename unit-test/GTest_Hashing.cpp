#include "gtest/gtest.h"
#include "Hashing.hpp"

#include <stdexcept>

class DISABLED_GTest_Hashing : public ::testing::Test {
//class GTest_Hashing : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

TEST_F(DISABLED_GTest_Hashing, Deterministic_Hashing_DivisionMethod) {
//TEST_F(GTest_Hashing, Deterministic_Hashing_DivisionMethod) {
  DivisionMethod method;
  for(unsigned long long m = 1; m < 100; m++)
    for(int k = 0; k < 8388608; k++){
        unsigned long long i = method(m,k);
        ASSERT_EQ(method(m,k), method(m,k));
        ASSERT_TRUE(i >= 0 && i < m);
    }
}

TEST_F(DISABLED_GTest_Hashing, Deterministic_Hashing_KnuthDivisionMethod) {
//TEST_F(GTest_Hashing, Deterministic_Hashing_KnuthDivisionMethod) {
  KnuthDivisionMethod method;
  for(unsigned long long m = 1; m < 100; m++)
    for(int k = 0; k < 8388608; k++){
      unsigned long long i = method(m,k);
      ASSERT_EQ(i, method(m,k));
      ASSERT_TRUE(i >= 0 && i < m);
  }
}
/*
TEST_F(DISABLED_GTest_Hashing, Deterministic_Hashing_KnuthDivisionMethod) {
//TEST_F(GTest_Hashing, Deterministic_Hashing_MultiplicationMethod) {
  MultiplicationMethod method;
  for(int m = 1; m < 10; m++)
    for(int k = 0; k < 8388608; k++){
      ASSERT_EQ(method(m,k), method(m,k));
  }
}
*/

TEST_F(DISABLED_GTest_Hashing, Deterministic_Hashing_UniversalMethod) {
//TEST_F(GTest_Hashing, Deterministic_Hashing_UniversalMethod) {
  UniversalMethod method;
  for(unsigned long long m = 1; m < 100; m++)
    for(int k = 0; k < 8388608; k++){
        unsigned long long i = method(m,k);
        ASSERT_EQ(method(m,k), method(m,k));
        ASSERT_TRUE(i >= 0 && i < m);
    }
}
