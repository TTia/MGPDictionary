#include "gtest/gtest.h"
#include "open_addressing/OpenAddressing.hpp"

class ConceptLess{
public:
  bool operator ==(ConceptLess&){
    return true;
  }
  bool operator !=(ConceptLess&){
    return false;
  }
//  bool operator ==(ConceptLess&) = delete;
//  bool operator !=(ConceptLess&) = delete;
};

namespace std {
  template <>
  struct hash<ConceptLess>{
    size_t operator()(const ConceptLess&) const{
      return 0;
    }
  };
}

class GTest_Concept : public ::testing::Test {
};

TEST_F(GTest_Concept, Equality) {
  std::hash<ConceptLess> h;
  OpenAddressingDictionary<ConceptLess, char, LinearProbing> pht(h);
}
