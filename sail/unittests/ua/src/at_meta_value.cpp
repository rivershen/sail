/*
 * at_meta_value.cpp
 *
 *  Created on: Jul 24, 2015
 *      Author: chushen
 */


#include "ua/ua.h"
#include "common_types.h"
//#include "ua/detail/key_converter.h"
#include "gtest/gtest.h"
#include "jsoncpp_support.h"

namespace{
using namespace sail;


typedef ::testing::Types<
        bool,
        int,
        float,
        std::string,
        std::wstring,
        Rgb,
        Json::Value
        > ScalarTypes;
template<typename T>
struct scalar_meta_tests:public testing::Test{
};

TYPED_TEST_CASE(scalar_meta_tests, ScalarTypes);
TYPED_TEST(scalar_meta_tests, obtain){
    TypeParam v;
    EXPECT_EQ(NULL, metas::get_meta_value<TypeParam>().to_map(&v));
    EXPECT_EQ(NULL, metas::get_meta_value<TypeParam>().to_list(&v));
}
TYPED_TEST(scalar_meta_tests, load_from_same_type){
    TypeParam from = TypeParam();
    TypeParam to;
    const meta_value& meta = metas::get_meta_value<TypeParam>();
    EXPECT_EQ(bind_status::SUCCESS, meta.load_from(values::make_ref(from),&to));
    EXPECT_EQ(from, to);
}

TEST(scalar_meta_tests, different_type){
    EXPECT_NE(&metas::get_meta_value<int>(), &metas::get_meta_value<std::string>());
    EXPECT_NE(&metas::get_meta_value<int>(), &metas::get_meta_value<unsigned int>());
}

TEST(scalar_meta_tests, same_type){
    EXPECT_EQ(&metas::get_meta_value<int>(), &metas::get_meta_value<int>());

    EXPECT_EQ(&metas::get_meta_value<int8_t>(), &metas::get_meta_value<char>());
    EXPECT_EQ(&metas::get_meta_value<int16_t>(), &metas::get_meta_value<short>());
    EXPECT_EQ(&metas::get_meta_value<int32_t>(), &metas::get_meta_value<int>());
    EXPECT_EQ(&metas::get_meta_value<int64_t>(), &metas::get_meta_value<long long>());
}
}



