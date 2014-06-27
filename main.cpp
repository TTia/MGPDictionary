#include <iostream>
#include "Dictionary.hpp"
#include "ChainingHashTable.hpp"
#include "OpenAddressingHashTable.hpp"
#include "gtest/gtest.h"
#include "ChainingHashTableIterator.hpp"

using namespace std;

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

