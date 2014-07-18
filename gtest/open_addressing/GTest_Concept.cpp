#include "gtest/gtest.h"
#include "open_addressing/OpenAddressing.hpp"

class ConceptLess{
public:
//  bool operator ==(ConceptLess&){
//    return true;
//  }
//  bool operator !=(ConceptLess&){
//    return false;
//  }
  bool operator ==(ConceptLess&) = delete;
  bool operator !=(ConceptLess&) = delete;
};

template <>
struct std::hash<ConceptLess>{
  int operator ()(){
    return 0;
  }
};

class GTest_Concept : public ::testing::Test {
};

TEST_F(GTest_Concept, Equality) {
  std::hash<ConceptLess> h;
  std::hash<int> hi;
  OpenAddressingDictionary<int, char, LinearProbing> pht(hi);
}
