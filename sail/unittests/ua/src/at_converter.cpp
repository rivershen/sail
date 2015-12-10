/*
 * at_converter.cpp
 *
 *  Created on: Jun 26, 2015
 *      Author: chushen
 */
#include "gtest/gtest.h"
#include "ua/detail/safe_implicit_value_converter.h"
#include <climits>
#include <limits>
#include <cstdint>
using namespace sail::detail;
using namespace std;
namespace {
typedef ::testing::Types<
        int8_t,
        int16_t,
        int32_t,
        uint8_t,
        uint16_t,
        uint32_t,
        int64_t,
        uint64_t,
        char,
        short,
        int,
        unsigned char,
        unsigned short,
        unsigned int,
        long long,
        unsigned long long
        > IntegralTypes;
}
template<typename K>
struct safe_implicit_value_converter_test:public testing::Test{
};

TYPED_TEST_CASE(safe_implicit_value_converter_test, IntegralTypes);

TYPED_TEST(safe_implicit_value_converter_test, integral_limits){
    TypeParam v;
    EXPECT_TRUE(safe_implicit_value_converter::convert(numeric_limits<TypeParam>::max(), &v));
    EXPECT_EQ(numeric_limits<TypeParam>::max(), v);
    EXPECT_TRUE(safe_implicit_value_converter::convert(numeric_limits<TypeParam>::min(), &v));
    EXPECT_EQ(numeric_limits<TypeParam>::min(), v);
  /*  if(!numeric_limits<TypeParam>::is_signed){
        EXPECT_FALSE(safe_implicit_value_converter::convert(-1, &v));
    }else if(sizeof(v) < 8){
        EXPECT_FALSE(safe_implicit_value_converter::convert(numeric_limits<TypeParam>::max()+1ull, &v));
    }*/
}
TYPED_TEST(safe_implicit_value_converter_test, convertible_to_bool){
    bool b;
    EXPECT_TRUE(safe_implicit_value_converter::convert(TypeParam(1), &b));
    EXPECT_TRUE(b);
    EXPECT_TRUE(safe_implicit_value_converter::convert(TypeParam(), &b));
    EXPECT_FALSE(b);

}
TYPED_TEST(safe_implicit_value_converter_test, convertible_from_bool){
    TypeParam v = 100;
    bool b;
    EXPECT_TRUE(safe_implicit_value_converter::convert(v, &b));

    EXPECT_TRUE(safe_implicit_value_converter::convert(true, &v));
    EXPECT_EQ(1, v);
    EXPECT_TRUE(safe_implicit_value_converter::convert(false, &v));
    EXPECT_EQ(0, v);


}
TYPED_TEST(safe_implicit_value_converter_test, not_convertible_to_string){
    TypeParam v = 100;
    EXPECT_FALSE(safe_implicit_value_converter::convert(std::string("123"), &v));
    EXPECT_FALSE(safe_implicit_value_converter::convert(std::wstring(L"123"), &v));
    std::string s;
    EXPECT_FALSE(safe_implicit_value_converter::convert(v,&s));
    std::wstring ws;
    EXPECT_FALSE(safe_implicit_value_converter::convert(v,&ws));
}
TYPED_TEST(safe_implicit_value_converter_test, convertible_to_float){
    TypeParam v = 100;
    float f;
    EXPECT_TRUE(safe_implicit_value_converter::convert(v, &f));
    EXPECT_FLOAT_EQ(100.0, f);
    double d;
    EXPECT_TRUE(safe_implicit_value_converter::convert(v, &d));
    EXPECT_DOUBLE_EQ(100.0l, d);

    EXPECT_TRUE(safe_implicit_value_converter::convert(3.14f, &v));
    EXPECT_EQ(3, v);
    EXPECT_TRUE(safe_implicit_value_converter::convert(1.23l, &v));
    EXPECT_EQ(1, v);
}
TEST(safe_implicit_value_converter_tests2, float){
    double d;
    float f;
    EXPECT_TRUE(safe_implicit_value_converter::convert(3.14f, &f));
    EXPECT_FLOAT_EQ(3.14f, f);
    EXPECT_TRUE(safe_implicit_value_converter::convert(3.14l, &f));
    EXPECT_FLOAT_EQ(3.14f, f);

    EXPECT_TRUE(safe_implicit_value_converter::convert(3.14f, &d));
    EXPECT_DOUBLE_EQ(3.14f, d);
    EXPECT_TRUE(safe_implicit_value_converter::convert(3.14l, &d));
    EXPECT_DOUBLE_EQ(3.14l, d);
}

