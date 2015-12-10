/*
 * at_map_ref.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: chushen
 */

#include <ua/maps.h>
#include "gtest/gtest.h"
#include "common_types.h"
#include "ua/ua_ref.h"
#include <map>
#include <sstream>
using namespace sail;
template<typename T>
struct map_ref_tests:public testing::Test{
};
TYPED_TEST_CASE(map_ref_tests, IntMapTestDataList);


TYPED_TEST(map_ref_tests, for_each_string){
    struct Validator{
       void operator()(const std::string& name, const value_ref& value){
           EXPECT_EQ(expected.at(name),value.as<int>());
       }
       std::map<std::string, int> expected;
       Validator() :expected({ { "R", 0 }, { "G", 1 }, { "B", 2 } }){
       }
   };
   typedef typename TypeParam::Type Type;
   const Type& m = TypeParam::getPopulated();
   std::map<std::string, int> expected{ { "R", 0 }, { "G", 1 }, { "B", 2 } };
   values::make_ref(m).to_map().for_each([&](const std::string&name, const value_ref&value){
       EXPECT_EQ(expected.at(name),value.as<int>());
   });
}
TYPED_TEST(map_ref_tests, get_with_string){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    const map_ref& ref = values::make_ref(m).to_map();
    EXPECT_EQ(0, ref[std::string("R")].template as<int>());
    EXPECT_EQ(1, ref[std::string("G")].template as<int>());
    EXPECT_EQ(2, ref[std::string("B")].template as<int>());
    EXPECT_THROW(ref[std::string("X")], std::out_of_range);
}
TYPED_TEST(map_ref_tests, get_with_cstr){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    const map_ref& ref = values::make_ref(m).to_map();
    EXPECT_EQ(0, ref["R"].template as<int>());
    EXPECT_EQ(1, ref["G"].template as<int>());
    EXPECT_EQ(2, ref["B"].template as<int>());
    EXPECT_THROW(ref["X"], std::out_of_range);
}
TYPED_TEST(map_ref_tests, to_json){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    const map_ref& ref = values::make_ref(m).to_map();
    std::stringstream ss;
    ss << ref;
    Json::Value target;
    ss >> target;
    EXPECT_TRUE(target.isMember("R"));
    EXPECT_EQ(Json::intValue, target["R"].type());
    EXPECT_EQ(0, target["R"].asInt());
    EXPECT_TRUE(target.isMember("G"));
    EXPECT_EQ(Json::intValue, target["G"].type());
    EXPECT_EQ(1, target["G"].asInt());
    EXPECT_TRUE(target.isMember("B"));
    EXPECT_EQ(Json::intValue, target["B"].type());
    EXPECT_EQ(2, target["B"].asInt());
}

TEST(map_ref_test2, heterogenous_fields){
    // maps with heterogeneous fields
    Record m5 ={100, "abc"};
    values::make_ref(m5).to_map().for_each([](const std::string& name, const value_ref& value){
      if(name == "id"){
          EXPECT_EQ(100, value.as<int>());
      }else if(name == "name"){
          EXPECT_EQ("abc", value.get_typed_reference<std::string>());
      }
    });
}

TEST(map_ref_tests2, empty_and_size){
    std::map<std::string, int> m1;
    map_ref ref = maps::make_ref(m1);
    EXPECT_TRUE(ref.empty());
    EXPECT_EQ(0, ref.size());
    m1.emplace("abc", 100);
    EXPECT_FALSE(ref.empty());
    EXPECT_EQ(1, ref.size());

    Record m2;
    ref = maps::make_ref(m2);
    EXPECT_FALSE(ref.empty());
    EXPECT_EQ(2, ref.size());
}




