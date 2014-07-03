#include "gtest/gtest.h"
#include "ChainingHashTable.hpp"

#include <stdexcept>

class GTest_ChainingHashTable : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(GTest_ChainingHashTable, coutnstructor) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5);
  EXPECT_EQ(cht->countValues(), 0);
  EXPECT_EQ(cht->loadFactor(), 0);
  delete cht;
}

TEST_F(GTest_ChainingHashTable, coutnstructor_illegal_parameters) {
  std::hash<int> h;
  try{
    new ChainingHashTable<int, char>(h, 1.2, -1);
    ASSERT_TRUE(false);
  }catch(std::logic_error e){
    ASSERT_TRUE(true);
  }
  //ASSERT_THROW(stat,type);
}

TEST_F(GTest_ChainingHashTable, Insert_single) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5);
  char a = 'a';

  ASSERT_FALSE(cht->insert(1,a));
  ASSERT_EQ(cht->countValues(), 1);
  delete cht;

}

TEST_F(GTest_ChainingHashTable, Insert_with_duplication) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5);
  char a = 'a', b = 'b', c = 'c';
  char *output;

  ASSERT_FALSE(cht->insert(1,a));
  ASSERT_FALSE(cht->insert(2,c));

  ASSERT_TRUE(cht->insert(1,a, &output));
  ASSERT_EQ(*output, a);

  ASSERT_TRUE(cht->insert(1,b));
  ASSERT_TRUE(cht->insert(1,a));
  ASSERT_EQ(cht->countValues(), 2);
  delete cht;
}

TEST_F(GTest_ChainingHashTable, Search) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5);
  char a = 'a';
  cht->insert(1, a);
  ASSERT_EQ(*cht->search(1), a);
}

TEST_F(GTest_ChainingHashTable, Search_over_chain) {
  std::hash<int> h;
  int m = 13;
  auto cht = new ChainingHashTable<int, char>(h, .5, m);
  char a = 'a', b = 'b', c = 'c';
  cht->insert(1, a);
  cht->insert(m+1, b);
  cht->insert(2*m+1, c);
  ASSERT_EQ(*cht->search(1), a);
  ASSERT_EQ(*cht->search(m+1), b);
}

TEST_F(GTest_ChainingHashTable, Search_element_doesnt_exist_on_empty_table) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5);
  ASSERT_EQ(cht->search(1), cht->end_value());
}

TEST_F(GTest_ChainingHashTable, Search_element_doesnt_exist) {
  std::hash<int> h;
  int m = 10;
  auto cht = new ChainingHashTable<int, char>(h, .5, m);
  char a = 'a';
  cht->insert(2*m+1, a);
  ASSERT_EQ(*cht->search(2*m+1), a);
  ASSERT_EQ(cht->search(2*m), cht->end_value());
}
TEST_F(GTest_ChainingHashTable, Delete_single) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h, .5);
  char a = 'a', *output = nullptr;
  cht->insert(0, a);
  ASSERT_EQ(cht->countValues(), 1);
  ASSERT_TRUE(cht->del(0, &output));
  ASSERT_TRUE(output != nullptr);
  ASSERT_EQ(*output, 'a');
  ASSERT_EQ(cht->countValues(), 0);
}

TEST_F(GTest_ChainingHashTable, Delete_over_chain) {
  std::hash<int> h;
  int m = 13;
  auto cht = new ChainingHashTable<int, char>(h, .5, m);
  char a = 'a', b = 'b', c = 'c';
  cht->insert(1, a);
  cht->insert(m+1, b);
  cht->insert(2*m+1, c);
  ASSERT_EQ(cht->countValues(), 3);
  ASSERT_TRUE(cht->del(2*m+1));
  ASSERT_EQ(cht->countValues(), 2);
  ASSERT_EQ(*cht->search(1), a);
  ASSERT_EQ(*cht->search(m+1), b);
  ASSERT_EQ(cht->search(2*m+1), cht->end_value());

  char* output = nullptr;
  ASSERT_TRUE(cht->del(m+1, &output));
  ASSERT_EQ(*output, b);
  ASSERT_EQ(cht->countValues(), 1);
}
TEST_F(GTest_ChainingHashTable, Delete_non_existing_element) {
  std::hash<int> h;
  auto cht = new ChainingHashTable<int, char>(h);
  ASSERT_EQ(cht->countValues(), 0);
  ASSERT_FALSE(cht->del(17));
}

TEST_F(GTest_ChainingHashTable, Square_bracket_operator_Insert_Update){
  ASSERT_FALSE(true);
}

TEST_F(GTest_ChainingHashTable, Invalidate_iterator_Insert){
  ASSERT_FALSE(true);
}
TEST_F(GTest_ChainingHashTable, Invalidate_iterator_Remove){
  ASSERT_FALSE(true);
}
TEST_F(GTest_ChainingHashTable, Invalidate_iterator_Delete){
  ASSERT_FALSE(true);
}
