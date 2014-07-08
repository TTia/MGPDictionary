#include "gtest/gtest.h"
#include "open_addressing/ProbingHashTable.hpp"

#include <stdexcept>

class GTest_ProbingHashTable : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(GTest_ProbingHashTable, Constructor) {
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h, .5);
  EXPECT_EQ(pht->countValues(), 0);
  EXPECT_EQ(pht->loadFactor(), 0);
}

TEST_F(GTest_ProbingHashTable, Constructor_illegal_parameters) {
  std::hash<int> h;
  try{
    new ProbingHashTable<int, char>(h, 1.2, -1);
    ASSERT_TRUE(false);
  }catch(std::logic_error){
    ASSERT_TRUE(true);
  }
}

TEST_F(GTest_ProbingHashTable, Insert_single) {
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h, .5);
  char a = 'a';

  ASSERT_FALSE(pht->insert(1,a));
  ASSERT_EQ(pht->countValues(), 1);
}

TEST_F(GTest_ProbingHashTable, Insert_with_duplication) {
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h, .5);
  char a = 'a', b = 'b', c = 'c';
  char *output;

  ASSERT_FALSE(pht->insert(1,a));
  ASSERT_FALSE(pht->insert(2,c));

  ASSERT_TRUE(pht->insert(1,a, &output));
  ASSERT_EQ(*output, a);

  ASSERT_TRUE(pht->insert(1,b));
  ASSERT_TRUE(pht->insert(1,a));
  ASSERT_EQ(pht->countValues(), 2);
  delete pht;
}

TEST_F(GTest_ProbingHashTable, Search) {
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h, .5);
  char a = 'a';
  pht->insert(1, a);
  ASSERT_EQ(*pht->search(1), a);
}


TEST_F(GTest_ProbingHashTable, Collisions) {
  std::hash<int> h;
  int m = 13;
  auto pht = new ProbingHashTable<int, char>(h, .5, m);
  char a = 'a', b = 'b', c = 'c';
  pht->insert(1, a);
  pht->insert(m+1, b);
  pht->insert(2*m+1, c);
  ASSERT_EQ(*pht->search(1), a);
  ASSERT_EQ(*pht->search(m+1), b);
}

TEST_F(GTest_ProbingHashTable, Search_element_doesnt_exist_on_empty_table) {
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h, .5);
  ASSERT_EQ(pht->search(1), pht->end_value());
//  ASSERT_EQ(pht->search(1), nullptr);
}

TEST_F(GTest_ProbingHashTable, Search_element_doesnt_exist) {
  std::hash<int> h;
  int m = 10;
  auto pht = new ProbingHashTable<int, char>(h, .5, m);
  char a = 'a';
  pht->insert(2*m+1, a);
  ASSERT_EQ(*pht->search(2*m+1), a);
  ASSERT_EQ(pht->search(2*m), pht->end_value());
//  ASSERT_EQ(pht->search(2*m), nullptr);
}

TEST_F(GTest_ProbingHashTable, Search_element_doesnt_exist_Extended) {
  std::hash<int> h;
  int m = 1024;
  auto pht = new ProbingHashTable<int, char>(h, .5, m);
  char a = 'a';
  for(int i = 0; i<100; i++){
      pht->insert(i, a);
      ASSERT_NE(pht->search(i), pht->end_value());
//      ASSERT_NE(pht->search(i), nullptr);
      ASSERT_EQ(a, *pht->search(i));
    }
  ASSERT_EQ(pht->search(100), pht->end_value());
//  ASSERT_EQ(pht->search(100), nullptr);
}

TEST_F(GTest_ProbingHashTable, Delete_single) {
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h, .5);
  char a = 'a', *output = nullptr;
  pht->insert(0, a);
  ASSERT_EQ(pht->countValues(), 1);
  ASSERT_TRUE(pht->del(0, &output));
  ASSERT_TRUE(output != nullptr);
  ASSERT_EQ(*output, 'a');
  ASSERT_EQ(pht->countValues(), 0);
}

TEST_F(GTest_ProbingHashTable, Delete_with_collisions) {
  std::hash<int> h;
  int m = 13;
  auto pht = new ProbingHashTable<int, char>(h, .5, m);
  char a = 'a', b = 'b', c = 'c';
  pht->insert(1, a);
  pht->insert(m+1, b);
  pht->insert(2*m+1, c);
  ASSERT_EQ(pht->countValues(), 3);
  ASSERT_FALSE(pht->del(2*m+1));
  ASSERT_EQ(pht->countValues(), 2);
  ASSERT_EQ(*pht->search(1), a);
  ASSERT_EQ(*pht->search(m+1), b);
  ASSERT_EQ(pht->search(2*m+1), pht->end_value());
//  ASSERT_EQ(pht->search(2*m+1), nullptr);

  char* output = nullptr;
  ASSERT_TRUE(pht->del(m+1, &output));
  ASSERT_EQ(*output, b);
  ASSERT_EQ(pht->countValues(), 1);
}

TEST_F(GTest_ProbingHashTable, Delete_non_existing_element) {
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h);
  char *output;
  ASSERT_EQ(pht->countValues(), 0);
  ASSERT_FALSE(pht->del(17, &output));
  ASSERT_EQ(pht->countValues(), 0);
}

TEST_F(GTest_ProbingHashTable, Invalidate_iterator_Insert){
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h);
  char a = 'a';
  pht->insert(1, a);
  auto it = pht->begin();
  ASSERT_NE(it, pht->end());

  pht->insert(2, a);
  ASSERT_EQ(it, pht->end());
}

TEST_F(GTest_ProbingHashTable, Invalidate_iterator_Remove){
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h);
  char a = 'a';
  pht->insert(1, a);
  pht->insert(2, a);

  auto it = pht->begin();
  ASSERT_NE(it, pht->end());
  pht->del(1);
  ASSERT_EQ(it, pht->end());
}

TEST_F(GTest_ProbingHashTable, Invalidate_iterator_Fail_to_remove){
  std::hash<int> h;
  auto pht = new ProbingHashTable<int, char>(h);
  char a = 'a';
  pht->insert(1, a);
  pht->insert(2, a);

  auto it = pht->begin();
  ASSERT_NE(it, pht->end());
  ASSERT_FALSE(pht->del(5));
  ASSERT_NE(it, pht->end());
}

TEST_F(GTest_ProbingHashTable, Invalidate_iterator_Delete){
  std::hash<int> h;
  int m = 17;
  auto pht = new ProbingHashTable<int, char>(h, 0.5, m);
  char a = 'a';
  pht->insert(1, a);

  auto it = pht->begin();
  ASSERT_NE(it, pht->end());
  auto value = std::pair<int, char>(1,a);
  ASSERT_EQ(*it, value);

  delete pht;
  try{
    value = *it;
    ASSERT_TRUE(false);
  }catch(std::logic_error){
    ASSERT_TRUE(true);
  }
}
