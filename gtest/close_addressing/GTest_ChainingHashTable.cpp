#include "gtest/gtest.h"
#include "close_addressing/CloseAddressing.hpp"
#include "close_addressing/ChainingHashTableIterator.hpp"
#include "Hashing.hpp"

#include <stdexcept>

class GTest_CloseAddressingDictionary : public ::testing::Test {};

TEST_F(GTest_CloseAddressingDictionary, Constructor) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  EXPECT_EQ(cht.countValues(), 0);
  EXPECT_EQ(cht.loadFactor(), 0);
}

TEST_F(GTest_CloseAddressingDictionary, Copy_Constructor_Same_Method) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  cht.insert(1, 'a');

  CloseAddressingDictionary<int, char> cht2{cht};
  cht2.insert(2, 'b');
  ASSERT_EQ(1, cht.countValues());
  ASSERT_EQ(2, cht2.countValues());

  cht[1] = 'd';
  ASSERT_EQ((*cht.search(1)).second, 'd');
  ASSERT_EQ((*cht2.search(1)).second, 'a');
  cht2.insert(3, 'c');
  cht2.search(3);
  cht2.del(3);
}

TEST_F(GTest_CloseAddressingDictionary, Copy_Constructor_Different_Method) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  cht.insert(1, 'a');

  CloseAddressingDictionary<int, char, UniversalMethod> cht2{cht};
  cht2.insert(2, 'b');
  ASSERT_EQ(1, cht.countValues());
  ASSERT_EQ(2, cht2.countValues());

  cht[1] = 'd';
  ASSERT_EQ((*cht.search(1)).second, 'd');
  ASSERT_EQ((*cht2.search(1)).second, 'a');
  cht2.insert(3, 'c');
  cht2.search(3);
  cht2.del(3);
}

TEST_F(GTest_CloseAddressingDictionary, Move_Constructor) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  cht.insert(1, 'a');

  CloseAddressingDictionary<int, char> cht2{std::move(cht)};
  cht2.insert(2, 'b');
  ASSERT_EQ(2, cht2.countValues());
}

TEST_F(GTest_CloseAddressingDictionary, Copy_Operator) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  CloseAddressingDictionary<int, char, KnuthDivisionMethod> cht2{h};
  cht.insert(1, 'a');

  cht2 = cht;
  cht2.insert(2, 'b');
  ASSERT_EQ(1, cht.countValues());
  ASSERT_EQ(2, cht2.countValues());

}

TEST_F(GTest_CloseAddressingDictionary, Copy_Move_Operator) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  CloseAddressingDictionary<int, char> cht2{h};
  cht.insert(1, 'a');

  cht2 = std::move(cht);
  cht2.insert(2, 'b');
  ASSERT_EQ(2, cht2.countValues());
}

TEST_F(GTest_CloseAddressingDictionary, Constructor_illegal_parameters) {
  std::hash<int> h;
  try{
    CloseAddressingDictionary<int, char> cht{h, 1.2, -1};
    ASSERT_TRUE(false);
  }catch(std::logic_error){
    ASSERT_TRUE(true);
  }
}

TEST_F(GTest_CloseAddressingDictionary, Insert_single) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};

  ASSERT_FALSE(cht.insert(1,'a'));
  ASSERT_EQ(cht.countValues(), 1);
}

TEST_F(GTest_CloseAddressingDictionary, Insert_with_duplication) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  char a = 'a', b = 'b', c = 'c';
  char output;

  ASSERT_FALSE(cht.insert(1,a));
  ASSERT_FALSE(cht.insert(2,c));

  ASSERT_TRUE(cht.insert(1,a, &output));
  ASSERT_EQ(output, a);

  ASSERT_TRUE(cht.insert(1,b));
  ASSERT_TRUE(cht.insert(1,a));
  ASSERT_EQ(cht.countValues(), 2);
}

TEST_F(GTest_CloseAddressingDictionary, Search) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  char a = 'a';
  std::pair<int, char> p1{1,a};
  cht.insert(p1.first, p1.second);
  ASSERT_EQ(*cht.search(1), p1);
}

TEST_F(GTest_CloseAddressingDictionary, Search_over_chain) {
  std::hash<int> h;
  int m = 13;
  CloseAddressingDictionary<int, char> cht{h, .5, m};
  char a = 'a', b = 'b', c = 'c';
  std::pair<int, char> p1{1,a}, p2{m+1, b}, p3{2*m+1, c};
  cht.insert(p1.first, p1.second);
  cht.insert(p2.first, p2.second);
  cht.insert(p3.first, p3.second);
  ASSERT_EQ(*cht.search(p1.first), p1);
  ASSERT_EQ(*cht.search(p2.first), p2);
  ASSERT_EQ(*cht.search(p3.first), p3);
}

TEST_F(GTest_CloseAddressingDictionary, Search_element_doesnt_exist_on_empty_table) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  ASSERT_EQ(cht.search(1), cht.end());
}

TEST_F(GTest_CloseAddressingDictionary, Search_element_doesnt_exist) {
  std::hash<int> h;
  int m = 10;
  CloseAddressingDictionary<int, char> cht{h, .5, m};
  char a = 'a';
  std::pair<int, char> p1{2*m+1,a};
  cht.insert(p1.first, p1.second);
  ASSERT_EQ(*cht.search(p1.first), p1);
  ASSERT_EQ(cht.search(2*m), cht.end());
}

TEST_F(GTest_CloseAddressingDictionary, Search_element_doesnt_exist_Extended) {
  std::hash<int> h;
  int m = 1000;
  CloseAddressingDictionary<int, char> cht{h, .5, m};
  char a = 'a';
  for(int i = 0; i<100; i++){
      std::pair<int, char> p1{i,a};
      cht.insert(p1.first, p1.second);
      ASSERT_NE(cht.search(i), cht.end());
      ASSERT_EQ(p1, *cht.search(i));
    }
  ASSERT_EQ(cht.search(100), cht.end());
}

TEST_F(GTest_CloseAddressingDictionary, Delete_single) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  char a = 'a', output;
  cht.insert(0, a);
  ASSERT_EQ(cht.countValues(), 1);
  ASSERT_TRUE(cht.del(0, &output));
  ASSERT_EQ(output, 'a');
  ASSERT_EQ(cht.countValues(), 0);
}

TEST_F(GTest_CloseAddressingDictionary, Delete_over_chain) {
  std::hash<int> h;
  int m = 13;
  CloseAddressingDictionary<int, char> cht{h};
  char a = 'a', b = 'b', c = 'c';
  std::pair<int, char> p1{1,a}, p2{m+1, b}, p3{2*m+1, c};
  cht.insert(p1.first, p1.second);
  cht.insert(p2.first, p2.second);
  cht.insert(p3.first, p3.second);
  ASSERT_EQ(cht.countValues(), 3);
  cht.del(p3.first);
  ASSERT_EQ(cht.countValues(), 2);
  ASSERT_EQ(*cht.search(1), p1);
  ASSERT_EQ(*cht.search(m+1), p2);
  ASSERT_EQ(cht.search(2*m+1), cht.end());

  char output;
  ASSERT_TRUE(cht.del(p2.first, &output));
  ASSERT_EQ(output, b);
  ASSERT_EQ(cht.countValues(), 1);
}

TEST_F(GTest_CloseAddressingDictionary, Delete_non_existing_element) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  ASSERT_EQ(cht.countValues(), 0);
  ASSERT_FALSE(cht.del(17));
}

TEST_F(GTest_CloseAddressingDictionary, Operator_Square_Bracket) {
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  char a = 'a';
  cht.insert(1, a);

  ASSERT_TRUE(cht[1] == 'a');
  try{
    cht[2];
    ASSERT_TRUE(false);
  }catch(std::out_of_range){
    ASSERT_TRUE(true);
  }
}

TEST_F(GTest_CloseAddressingDictionary, Invalidate_iterator_Insert){
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  char a = 'a';
  cht.insert(1, a);
  auto it = cht.begin();
  ASSERT_NE(it, cht.end());

  cht.insert(2, a);
  ASSERT_EQ(it, cht.end());
}

TEST_F(GTest_CloseAddressingDictionary, Invalidate_iterator_Remove){
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  char a = 'a';
  cht.insert(1, a);
  cht.insert(2, a);

  auto it = cht.begin();
  ASSERT_NE(it, cht.end());
  cht.del(1);
  ASSERT_EQ(it, cht.end());
}

TEST_F(GTest_CloseAddressingDictionary, Invalidate_iterator_Fail_to_remove){
  std::hash<int> h;
  CloseAddressingDictionary<int, char> cht{h};
  char a = 'a';
  cht.insert(1, a);
  cht.insert(2, a);

  auto it = cht.begin();
  ASSERT_NE(it, cht.end());
  ASSERT_FALSE(cht.del(5));
  ASSERT_NE(it, cht.end());
}

TEST_F(GTest_CloseAddressingDictionary, Invalidate_iterator_Delete){
  std::hash<int> h;
  int m = 17;
  CloseAddressingDictionary<int, char> cht{h, .5, m};
  char a = 'a';
  cht.insert(1, a);

  auto it = cht.begin();
  ASSERT_NE(it, cht.end());
  auto value = std::pair<int, char>(1,a);
  ASSERT_EQ(*it, value);
  cht.del(value.first);

  try{
    value = *it;
    ASSERT_TRUE(false);
  }catch(std::logic_error){
    ASSERT_TRUE(true);
  }
}
