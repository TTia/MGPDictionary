#include "gtest/gtest.h"
#include "open_addressing/Probing.hpp"

class GTest_Probing : public ::testing::Test {
};

TEST_F(GTest_Probing, DISABLED_Deterministic_Linear_Probing) {
  LinearProbing linear_probing;
  for(unsigned long long m = 1; m < 128; m++)
    for(int k = 0; k < 2048; k++){
        for(int p = 0; p < 1024; p++){
          unsigned long long i = linear_probing(p,m,k);
          ASSERT_EQ(i, linear_probing(p,m,k));
          ASSERT_TRUE(i >= 0 && i < m);
        }
    }
}

TEST_F(GTest_Probing, DISABLED_Deterministic_Quadratic_Probing) {
  QuadraticProbing quadratic_probing;
  for(unsigned long long m = 1; m < 128; m++)
    for(int k = 0; k < 2048; k++){
        for(int p = 0; p < 1024; p++){
          unsigned long long i = quadratic_probing(p,m,k);
          ASSERT_EQ(i, quadratic_probing(p,m,k));
          ASSERT_TRUE(i >= 0 && i < m);
        }
    }
}

TEST_F(GTest_Probing, DISABLED_Deterministic_Double_Hashing) {
  DoubleHashing double_hashing;
  for(unsigned long long m = 1; m < 128; m++)
    for(int k = 0; k < 2048; k++){
        for(int p = 0; p < 1024; p++){
          unsigned long long i = double_hashing(p,m,k);
          ASSERT_EQ(i, double_hashing(p,m,k));
          ASSERT_TRUE(i >= 0 && i < m);
        }
    }
}
