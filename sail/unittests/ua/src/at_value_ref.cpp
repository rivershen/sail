/*
 * at_any_ref.cpp
 *
 *  Created on: Aug 13, 2015
 *      Author: chushen
 */

#include <ua/ua_ref.h>
#include <ua/values.h>
#include "gtest/gtest.h"
#include "common_types.h"
#include "ua/metas.h"
#include "jsoncpp_support.h"
#include <cmath>
namespace{
using namespace sail;
TEST(value_ref_test, default_constructor){
    value_ref ref;
    EXPECT_TRUE(ref.is_none());
    EXPECT_NE(nullptr, ref.get_address());
    EXPECT_NO_THROW(ref.get_typed_reference<none_type>());
    EXPECT_THROW(ref.get_typed_reference<int>(), type_mismatch_exception);
    EXPECT_EQ(nullptr, ref.peek_typed_address<int>());
}
struct other_type{};
typedef ::testing::Types<
        bool,
        char, unsigned char,
        short, unsigned short,
        int, unsigned int,
        long, unsigned long,
        long long, unsigned long long,
        int8_t, uint8_t, int16_t, uint16_t,
        int32_t, uint32_t, int64_t, uint64_t,
        float,double,
        std::string, std::wstring,
        Rgb,Abc,Record,
        Json::Value, other_type
        > all_types;
template<typename T>
struct any_ref_obtain_test:public testing::Test{
};
TYPED_TEST_CASE(any_ref_obtain_test, all_types);

TYPED_TEST(any_ref_obtain_test, test){
    TypeParam a = TypeParam();
    value_ref ref = values::make_ref(a);
    EXPECT_EQ(&metas::get_meta_value<TypeParam>(), &ref.get_meta());
    EXPECT_EQ(&a, ref.get_address());
    EXPECT_TRUE(ref.is<TypeParam>());
    EXPECT_THROW(ref.get_typed_reference<none_type>(), type_mismatch_exception);
    EXPECT_EQ(nullptr, ref.peek_typed_address<none_type>());
    EXPECT_EQ(&a, ref.peek_typed_address<TypeParam>());
    EXPECT_NO_THROW(ref.as<TypeParam>());
}

TEST(value_ref_test,  as){
    int v = -1;
    value_ref ref = values::make_ref(v);
    EXPECT_EQ((unsigned int)-1, ref.as<unsigned int>());
    EXPECT_EQ((short)-1, ref.as<short>());
    EXPECT_TRUE(ref.as<bool>());
    EXPECT_THROW(ref.as<std::string>(), type_mismatch_exception);
    EXPECT_EQ("", ref.safe_as(std::string("")));

    std::string s;
    ref = values::make_ref(s);
    EXPECT_THROW(ref.as<int>(), type_mismatch_exception);
    EXPECT_NO_THROW(ref.as<std::wstring>());

    Json::Value v2= -1;
    ref = values::make_ref(v2);
    EXPECT_EQ((unsigned int)-1, ref.as<unsigned int>());
    EXPECT_EQ((short)-1, ref.as<short>());
    EXPECT_TRUE(ref.as<bool>());
    EXPECT_THROW(ref.as<std::string>(), type_mismatch_exception);
    EXPECT_EQ("", ref.safe_as(std::string("")));

    v2 = "abc";
    EXPECT_THROW(ref.as<int>(), type_mismatch_exception);
    EXPECT_EQ(std::wstring(L"abc"),ref.as<std::wstring>());
    EXPECT_EQ(std::wstring(L"abc"),ref.safe_as<std::wstring>(L""));
}

TEST(value_ref_test, to_json){
    std::stringstream ss;
    ss<<value_ref();
    EXPECT_EQ("null", ss.str());

    ss.str("");
    bool b = true;
    ss<< values::make_ref(b);
    EXPECT_EQ("true", ss.str());

    ss.str("");
    b = false;
    ss<< values::make_ref(b);
    EXPECT_EQ("true", ss.str());

    ss.str("");
    char c = 'a';
    ss<< values::make_ref(c);
    EXPECT_EQ("97", ss.str());

    ss.str("");
    int8_t i8 = -123;
    ss << values::make_ref(i8);
    EXPECT_EQ("-123", ss.str());

    ss.str("");
    uint8_t ui8 = 124;
    ss << values::make_ref(ui8);
    EXPECT_EQ("124", ss.str());

    ss.str("");
    int i = 100;
    ss << values::make_ref(i);
    EXPECT_EQ("100", ss.str());

    ss.str("");
    float f = 3.14f;
    ss << values::make_ref(f);
    EXPECT_EQ("3.14", ss.str());

    ss.str("");

    f = std::numeric_limits<float>::quiet_NaN();
    ss << values::make_ref(f);
    EXPECT_EQ("\"NaN\"",ss.str());

    ss.str("");
    f = std::numeric_limits<float>::infinity();
    ss << values::make_ref(f);
    EXPECT_EQ("\"Infinity\"",ss.str());

    ss.str("");
    std::string s ="abc\a\n\t\b\f\r\\\"";
    ss << values::make_ref(s);
    EXPECT_EQ("\"abc\\u0007\\n\\t\\b\\f\\r\\\\\\\"\"", ss.str());

    ss.str("");
    std::wstring ws =L"abc\a\n\t\b\f\r\\\"";
    ss << values::make_ref(ws);
    EXPECT_EQ("\"abc\\u0007\\n\\t\\b\\f\\r\\\\\\\"\"", ss.str());


}

}
