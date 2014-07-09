#include <iostream>

#include "gtest/gtest.h"
#include "close_addressing/ChainingHashTable.hpp"
//#include "open_addressing/ProbingHashTable.hpp"

using namespace std;

int main(int argc, char** argv)
{
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
  std::hash<int> h;
//  ProbingHashTable<int, char> cht(h);
  ChainingHashTable<int, char> cht(h);
  cht.insert(1,'a');
}

