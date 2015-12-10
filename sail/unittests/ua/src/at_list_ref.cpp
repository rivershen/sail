/*
 * at_list_ref.cpp
 *
 *  Created on: Aug 25, 2015
 *      Author: chushen
 */
#include "gtest/gtest.h"
#include "common_types.h"
#include "ua/lists.h"
#include "ua/ua_ref.h"
#include <list>
#include <forward_list>
namespace{
using namespace sail;
template<typename T>
struct list_ref_tests:public testing::Test{
    typedef typename T::Type Type;
    Type value;
    value_ref ref;
    void SetUp() override{
        value = T::getPopulated();
        ref = values::make_ref(value);
    }
};
TYPED_TEST_CASE(list_ref_tests, IntListTestDataList);

TYPED_TEST(list_ref_tests, for_each){
    int index = 0;
    this->ref.to_list().for_each([&](const value_ref& item){
        ++ index;
        EXPECT_EQ(index, item.as<int>()+1);
    });
}
TYPED_TEST(list_ref_tests, size){
    EXPECT_EQ(6u, this->ref.to_list().size());
    EXPECT_FALSE(this->ref.to_list().empty());
}

TYPED_TEST(list_ref_tests, subscription){
    list_ref list = this->ref.to_list();
    EXPECT_EQ(0, list[0].as<int>());
    EXPECT_EQ(1, list[1].as<int>());
    EXPECT_EQ(2, list[2].as<int>());
    EXPECT_EQ(3, list[3].as<int>());
    EXPECT_EQ(4, list[4].as<int>());
    EXPECT_EQ(5, list[5].as<int>());
    EXPECT_THROW(list[6], std::out_of_range);
}
TYPED_TEST(list_ref_tests, to_json){
    list_ref list = this->ref.to_list();
    std::stringstream ss;
    ss << list;
    EXPECT_EQ("[0,1,2,3,4,5]", ss.str());
}
TEST(list_ref_tests, empty){
    std::list<int> v1;
    list_ref ref = lists::make_ref(v1);
    EXPECT_TRUE(ref.empty());
     //forward_list do not have size() method defined.
    std::forward_list<int> v2;
    ref = lists::make_ref(v2);
    EXPECT_TRUE(ref.empty());
}

}
