#include "gtest/gtest.h"
#include "open_addressing/OpenAddressing.hpp"

#include <stdexcept>

class GTest_OpenAddressingDictionary : public ::testing::Test {};

TEST_F(GTest_OpenAddressingDictionary, Constructor) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  EXPECT_EQ(pht.countValues(), 0);
  EXPECT_EQ(pht.loadFactor(), 0);
}

TEST_F(GTest_OpenAddressingDictionary, Copy_Constructor) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht{h};
  pht.insert(1, 'a');

  OpenAddressingDictionary<int, char> pht2(pht);
  pht2.insert(2, 'b');
  ASSERT_EQ(1, pht.countValues());
  ASSERT_EQ(2, pht2.countValues());

  pht[1] = 'd';
  ASSERT_EQ((*pht.search(1)).second, 'd');
  ASSERT_EQ((*pht2.search(1)).second, 'a');
  pht2.insert(3, 'c');
  pht2.search(3);
  pht2.del(3);
}

TEST_F(GTest_OpenAddressingDictionary, Move_Constructor) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht{h};
  pht.insert(1, 'a');

  OpenAddressingDictionary<int, char> pht2{std::move(pht)};
  pht2.insert(2, 'b');
  ASSERT_EQ(2, pht2.countValues());
}

TEST_F(GTest_OpenAddressingDictionary, Copy_Operator) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht{h};
  OpenAddressingDictionary<int, char, QuadraticProbing> pht2{h};
  pht.insert(1, 'a');

  pht2 = pht;
  pht2.insert(2, 'b');
  ASSERT_EQ(1, pht.countValues());
  ASSERT_EQ(2, pht2.countValues());
}

TEST_F(GTest_OpenAddressingDictionary, Copy_Move_Operator) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht{h};
  OpenAddressingDictionary<int, char> pht2{h};
  pht.insert(1, 'a');

  pht2 = std::move(pht);
  pht2.insert(2, 'b');
  ASSERT_EQ(2, pht2.countValues());
}

TEST_F(GTest_OpenAddressingDictionary, Constructor_illegal_parameters) {
  std::hash<int> h;
  try{
    OpenAddressingDictionary<int, char> pht(h, 1.2, -1);
    ASSERT_TRUE(false);
  }catch(std::logic_error){
    ASSERT_TRUE(true);
  }
}

TEST_F(GTest_OpenAddressingDictionary, Constructor_copy) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
}

TEST_F(GTest_OpenAddressingDictionary, Insert_single) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a';

  ASSERT_FALSE(pht.insert(1,a));
  ASSERT_EQ(pht.countValues(), 1);
}

TEST_F(GTest_OpenAddressingDictionary, Insert_with_duplication) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a', b = 'b', c = 'c';
  char output;

  ASSERT_FALSE(pht.insert(1,a));
  ASSERT_FALSE(pht.insert(2,c));

  ASSERT_TRUE(pht.insert(1,a, &output));
  ASSERT_EQ(output, 'a');

  ASSERT_TRUE(pht.insert(1,b));
  ASSERT_TRUE(pht.insert(1,a));
  ASSERT_EQ(pht.countValues(), 2);
}

TEST_F(GTest_OpenAddressingDictionary, Search) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a';
  std::pair<int, char> p{1, a};

  pht.insert(p.first, p.second);
  ASSERT_EQ(*pht.search(1), p);
}


TEST_F(GTest_OpenAddressingDictionary, Collisions) {
  std::hash<int> h;
  int m = 13;
  OpenAddressingDictionary<int, char> pht(h, .5, m);
  char a = 'a', b = 'b', c = 'c';
  std::pair<int, char> p1{0, a}, p2{m,b}, p3{2*m, c};
  pht.insert(p1.first, p1.second);
  pht.insert(p2.first, p2.second);
  pht.insert(p3.first, p3.second);
  ASSERT_EQ(*pht.search(p1.first), p1);
  ASSERT_EQ(*pht.search(p2.first), p2);
  ASSERT_EQ(*pht.search(p3.first), p3);
}

TEST_F(GTest_OpenAddressingDictionary, Search_element_doesnt_exist_on_empty_table) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  ASSERT_EQ(pht.search(1), pht.end());
}

TEST_F(GTest_OpenAddressingDictionary, Search_element_doesnt_exist) {
  std::hash<int> h;
  int m = 10;
  OpenAddressingDictionary<int, char> pht(h, .5, m);
  char a = 'a';
  std::pair<int, char> p1{2*m+1, a};
  pht.insert(p1.first, p1.second);
  ASSERT_EQ(*pht.search(p1.first), p1);
  ASSERT_EQ(pht.search(2*m), pht.end());
}

TEST_F(GTest_OpenAddressingDictionary, Search_element_doesnt_exist_Extended) {
  std::hash<int> h;
  int m = 1024;
  OpenAddressingDictionary<int, char> pht(h, .5, m);
  char a = 'a';
  for(int i = 0; i<100; i++){
      std::pair<int, char> p1{i, a};
      pht.insert(p1.first, p1.second);
      ASSERT_NE(pht.search(i), pht.end());
      ASSERT_EQ(p1, *pht.search(i));
    }
  ASSERT_EQ(pht.search(100), pht.end());
}

TEST_F(GTest_OpenAddressingDictionary, Operator_Square_Bracket) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht{h};
  char a = 'a';
  pht.insert(1, a);

  ASSERT_TRUE(pht[1] == 'a');
  try{
    pht[2];
    ASSERT_TRUE(false);
  }catch(std::out_of_range){
    ASSERT_TRUE(true);
  }
}

TEST_F(GTest_OpenAddressingDictionary, Delete_single) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a', output;
  pht.insert(0, a);
  ASSERT_EQ(pht.countValues(), 1);
  ASSERT_TRUE(pht.del(0, &output));
  ASSERT_EQ(output, 'a');
  ASSERT_EQ(pht.countValues(), 0);
}

TEST_F(GTest_OpenAddressingDictionary, Delete_with_collisions) {
  std::hash<int> h;
  int m = 13;
  OpenAddressingDictionary<int, char> pht(h, .5, m);
  char a = 'a', b = 'b', c = 'c';
  std::pair<int, char> p1{1, a}, p2{m+1,b}, p3{2*m+1, c};
  pht.insert(p1.first, p1.second);
  pht.insert(p2.first, p2.second);
  pht.insert(p3.first, p3.second);
  ASSERT_EQ(pht.countValues(), 3);
  ASSERT_FALSE(pht.del(p3.first));

  ASSERT_EQ(pht.countValues(), 2);
  ASSERT_EQ(*pht.search(1), p1);
  ASSERT_EQ(*pht.search(m+1), p2);
  ASSERT_EQ(pht.search(2*m+1), pht.end());

  char output;
  ASSERT_TRUE(pht.del(m+1, &output));
  ASSERT_EQ(output, b);
  ASSERT_EQ(pht.countValues(), 1);
}

TEST_F(GTest_OpenAddressingDictionary, Delete_non_existing_element) {
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char output;
  ASSERT_EQ(pht.countValues(), 0);
  ASSERT_FALSE(pht.del(17, &output));
  ASSERT_EQ(pht.countValues(), 0);
}

TEST_F(GTest_OpenAddressingDictionary, Invalidate_iterator_Insert){
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a';
  pht.insert(1, a);
  auto it = pht.begin();
  ASSERT_NE(it, pht.end());

  pht.insert(2, a);
  ASSERT_EQ(it, pht.end());
}

TEST_F(GTest_OpenAddressingDictionary, Invalidate_iterator_Remove){
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a';
  pht.insert(1, a);
  pht.insert(2, a);

  auto it = pht.begin();
  ASSERT_NE(it, pht.end());
  pht.del(1);
  ASSERT_EQ(it, pht.end());
}

TEST_F(GTest_OpenAddressingDictionary, Invalidate_iterator_Fail_to_remove){
  std::hash<int> h;
  OpenAddressingDictionary<int, char> pht(h);
  char a = 'a';
  pht.insert(1, a);
  pht.insert(2, a);

  auto it = pht.begin();
  ASSERT_NE(it, pht.end());
  ASSERT_FALSE(pht.del(5));
  ASSERT_NE(it, pht.end());
}

TEST_F(GTest_OpenAddressingDictionary, Invalidate_iterator_Delete){
  std::hash<int> h;
  int m = 17;
  OpenAddressingDictionary<int, char> pht(h, .5, m);
  char a = 'a';
  pht.insert(1, a);

  auto it = pht.begin();
  ASSERT_NE(it, pht.end());
  auto value = std::pair<int, char>(1,a);
  ASSERT_EQ(*it, value);
  pht.del(value.first);

  try{
    value = *it;
    ASSERT_TRUE(false);
  }catch(std::logic_error){
    ASSERT_TRUE(true);
  }
}
