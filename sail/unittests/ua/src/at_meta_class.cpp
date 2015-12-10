/*
 * at_meta_class.cpp
 *
 *  Created on: Aug 10, 2015
 *      Author: chushen
 */


#include <ua/metas.h>
#include "gtest/gtest.h"
#include <string>
#include <stdexcept>
namespace {
using namespace sail;

    enum class UA_ENUM(Enum,
        v1, v2, v3
    );

    struct TestObject{
        bool b;

        unsigned char uc;
        uint8_t ui8;
        char c;
        int8_t i8;

        unsigned short us;
        uint16_t ui16;
        short s;
        int16_t i16;

        unsigned int ui;
        uint32_t ui32;
        int i;
        int32_t i32;

        unsigned long long ull;
        uint64_t ui64;
        long long ll;
        int64_t i64;

        float f;
        double d;

        std::string str1;
        std::string str2;
        std::wstring wstr1;
        std::wstring wstr2;
        Enum e1;
        Enum e2;

        UA_OBJECT(TestObject,
            +b,
            +uc,
            -i8 = 18,
            +us,
            -ui16 = 116,
            +s,
            -i16= -116,
            +ui,
            -ui32 = 132,
            +i,
            -i32 = -132,
            +ull,
            -ui64 = 164ll,
            +ll,
            -i64 = -164,
            +f,
            -d = 3.14,
            +str1,
            -str2="str",
            +wstr1,
            -wstr2=L"wstr",
            +e1,
            -e2 = Enum::v3
        );
    };
#define ABC_FIELDS +a,+b,+c
    struct Abc{
        int a;
        int b;
        int c;
        UA_OBJECT(Abc, ABC_FIELDS);
    };
#undef ABC_FIELDS

TEST(meta_class_tests, FieldVisitor){
    TestObject* t = nullptr;
    std::vector<meta_class::field> fields;
    detail::native_object_visitor visitor(fields);
    visitor.setObject(t);
    _ua_visit_native_object(t,&visitor);
    EXPECT_EQ(23u, fields.size());
    EXPECT_EQ("b",fields[0].name);
    EXPECT_EQ(&static_meta_value::obtain<bool>(),fields[0].meta);
    EXPECT_FALSE(fields[0].is_optional);
    EXPECT_EQ(offsetof(TestObject, b), fields[0].offset);

    EXPECT_EQ("uc",fields[1].name);
    EXPECT_EQ(&static_meta_value::obtain<uint8_t>(),fields[1].meta);
    EXPECT_FALSE(fields[1].is_optional);
    EXPECT_EQ(offsetof(TestObject, uc), fields[1].offset);

    EXPECT_EQ("i8",fields[2].name);
    EXPECT_EQ(&static_meta_value::obtain<int8_t>(),fields[2].meta);
    EXPECT_TRUE(fields[2].is_optional);
    EXPECT_EQ(offsetof(TestObject, i8), fields[2].offset);

    EXPECT_EQ("us",fields[3].name);
    EXPECT_EQ(&static_meta_value::obtain<uint16_t>(),fields[3].meta);
    EXPECT_FALSE(fields[3].is_optional);
    EXPECT_EQ(offsetof(TestObject, us), fields[3].offset);

    EXPECT_EQ("ui16",fields[4].name);
    EXPECT_EQ(&static_meta_value::obtain<uint16_t>(),fields[4].meta);
    EXPECT_TRUE(fields[4].is_optional);
    EXPECT_EQ(offsetof(TestObject, ui16), fields[4].offset);

    EXPECT_EQ("s",fields[5].name);
    EXPECT_EQ(&static_meta_value::obtain<int16_t>(),fields[5].meta);
    EXPECT_FALSE(fields[5].is_optional);
    EXPECT_EQ(offsetof(TestObject, s), fields[5].offset);

    EXPECT_EQ("i16",fields[6].name);
    EXPECT_EQ(&static_meta_value::obtain<int16_t>(),fields[6].meta);
    EXPECT_TRUE(fields[6].is_optional);
    EXPECT_EQ(offsetof(TestObject, i16), fields[6].offset);

    EXPECT_EQ("ui",fields[7].name);
    EXPECT_EQ(&static_meta_value::obtain<uint32_t>(),fields[7].meta);
    EXPECT_FALSE(fields[7].is_optional);

    EXPECT_EQ("ui32",fields[8].name);
    EXPECT_EQ(&static_meta_value::obtain<uint32_t>(),fields[8].meta);
    EXPECT_TRUE(fields[8].is_optional);
    EXPECT_EQ(offsetof(TestObject, ui32), fields[8].offset);

    EXPECT_EQ("i",fields[9].name);
    EXPECT_EQ(&static_meta_value::obtain<int32_t>(),fields[9].meta);
    EXPECT_FALSE(fields[9].is_optional);
    EXPECT_EQ(offsetof(TestObject, i), fields[9].offset);

    EXPECT_EQ("i32",fields[10].name);
    EXPECT_EQ(&static_meta_value::obtain<int32_t>(),fields[10].meta);
    EXPECT_TRUE(fields[10].is_optional);
    EXPECT_EQ(offsetof(TestObject, i32), fields[10].offset);

    EXPECT_EQ("ull",fields[11].name);
    EXPECT_EQ(&static_meta_value::obtain<uint64_t>(),fields[11].meta);
    EXPECT_FALSE(fields[11].is_optional);
    EXPECT_EQ(offsetof(TestObject, ull), fields[11].offset);

    EXPECT_EQ("ui64",fields[12].name);
    EXPECT_EQ(&static_meta_value::obtain<uint64_t>(),fields[12].meta);
    EXPECT_TRUE(fields[12].is_optional);
    EXPECT_EQ(offsetof(TestObject, ui64), fields[12].offset);

    EXPECT_EQ("ll",fields[13].name);
    EXPECT_EQ(&static_meta_value::obtain<int64_t>(),fields[14].meta);
    EXPECT_FALSE(fields[13].is_optional);
    EXPECT_EQ(offsetof(TestObject, ll), fields[13].offset);

    EXPECT_EQ("i64",fields[14].name);
    EXPECT_EQ(&static_meta_value::obtain<int64_t>(),fields[14].meta);
    EXPECT_TRUE(fields[14].is_optional);
    EXPECT_EQ(offsetof(TestObject, i64), fields[14].offset);

    EXPECT_EQ("f",fields[15].name);
    EXPECT_EQ(&static_meta_value::obtain<float>(),fields[15].meta);
    EXPECT_FALSE(fields[15].is_optional);
    EXPECT_EQ(offsetof(TestObject, f), fields[15].offset);

    EXPECT_EQ("d",fields[16].name);
    EXPECT_EQ(&static_meta_value::obtain<double>(),fields[16].meta);
    EXPECT_TRUE(fields[16].is_optional);
    EXPECT_EQ(offsetof(TestObject, d), fields[16].offset);

    EXPECT_EQ("str1",fields[17].name);
    EXPECT_EQ(&static_meta_value::obtain<std::string>(),fields[17].meta);
    EXPECT_FALSE(fields[17].is_optional);
    EXPECT_EQ(offsetof(TestObject, str1), fields[17].offset);

    EXPECT_EQ("str2",fields[18].name);
    EXPECT_EQ(&static_meta_value::obtain<std::string>(),fields[18].meta);
    EXPECT_TRUE(fields[18].is_optional);
    EXPECT_EQ(offsetof(TestObject, str2), fields[18].offset);

    EXPECT_EQ("wstr1",fields[19].name);
    EXPECT_EQ(&static_meta_value::obtain<std::wstring>(),fields[19].meta);
    EXPECT_FALSE(fields[19].is_optional);
    EXPECT_EQ(offsetof(TestObject, wstr1), fields[19].offset);

    EXPECT_EQ("wstr2",fields[20].name);
    EXPECT_EQ(&static_meta_value::obtain<std::wstring>(),fields[20].meta);
    EXPECT_TRUE(fields[20].is_optional);
    EXPECT_EQ(offsetof(TestObject, wstr2), fields[20].offset);

    EXPECT_EQ("e1",fields[21].name);
    EXPECT_EQ(&static_meta_value::obtain<Enum>(),fields[21].meta);
    EXPECT_FALSE(fields[21].is_optional);
    EXPECT_EQ(offsetof(TestObject, e1), fields[21].offset);

    EXPECT_EQ("e2",fields[22].name);
    EXPECT_EQ(&static_meta_value::obtain<Enum>(),fields[22].meta);
    EXPECT_TRUE(fields[22].is_optional);
    EXPECT_EQ(offsetof(TestObject, e2), fields[22].offset);
}

TEST(meta_class_tests, size){
    const meta_class& meta = metas::get_meta_class<TestObject>();
    EXPECT_EQ(23u, meta.size());
}
TEST(meta_class_tests, isHomogenious){
    const meta_class& meta = metas::get_meta_class<TestObject>();
    EXPECT_FALSE(meta.is_homogenious());
    const meta_class& meta2 = metas::get_meta_class<Abc>();
    EXPECT_TRUE(meta2.is_homogenious());
}
TEST(meta_class_tests, exists){
    const meta_class& meta = metas::get_meta_class<TestObject>();
    EXPECT_TRUE(meta.exists("b"));
    EXPECT_TRUE(meta.exists("uc"));
    EXPECT_TRUE(meta.exists("i8"));
    EXPECT_TRUE(meta.exists("us"));
    EXPECT_TRUE(meta.exists("ui16"));
    EXPECT_TRUE(meta.exists("ui"));
    EXPECT_TRUE(meta.exists("ui32"));
    EXPECT_TRUE(meta.exists("i"));
    EXPECT_TRUE(meta.exists("i32"));
    EXPECT_TRUE(meta.exists("ull"));
    EXPECT_TRUE(meta.exists("ui64"));
    EXPECT_TRUE(meta.exists("ll"));
    EXPECT_TRUE(meta.exists("i64"));
    EXPECT_TRUE(meta.exists("f"));
    EXPECT_TRUE(meta.exists("d"));
    EXPECT_TRUE(meta.exists("str1"));
    EXPECT_TRUE(meta.exists("str2"));
    EXPECT_TRUE(meta.exists("wstr1"));
    EXPECT_TRUE(meta.exists("wstr2"));
    EXPECT_TRUE(meta.exists("e1"));
    EXPECT_TRUE(meta.exists("e2"));
    EXPECT_FALSE(meta.exists("xx"));
}
TEST(meta_class_tests, at){
    const meta_class& meta = metas::get_meta_class<TestObject>();
    EXPECT_EQ(meta.get_field("b").offset, offsetof(TestObject,b));
    EXPECT_EQ(meta.get_field("uc").offset, offsetof(TestObject,uc));
    EXPECT_EQ(meta.get_field("i8").offset, offsetof(TestObject,i8));
    EXPECT_EQ(meta.get_field("us").offset, offsetof(TestObject,us));
    EXPECT_EQ(meta.get_field("ui16").offset, offsetof(TestObject,ui16));
    EXPECT_EQ(meta.get_field("s").offset, offsetof(TestObject,s));
    EXPECT_EQ(meta.get_field("i16").offset, offsetof(TestObject,i16));
    EXPECT_EQ(meta.get_field("ui").offset, offsetof(TestObject,ui));
    EXPECT_EQ(meta.get_field("ui32").offset, offsetof(TestObject,ui32));
    EXPECT_EQ(meta.get_field("ull").offset, offsetof(TestObject,ull));
    EXPECT_EQ(meta.get_field("ui64").offset, offsetof(TestObject,ui64));
    EXPECT_EQ(meta.get_field("ll").offset, offsetof(TestObject,ll));
    EXPECT_EQ(meta.get_field("i64").offset, offsetof(TestObject,i64));
    EXPECT_EQ(meta.get_field("f").offset, offsetof(TestObject,f));
    EXPECT_EQ(meta.get_field("d").offset, offsetof(TestObject,d));
    EXPECT_EQ(meta.get_field("str1").offset, offsetof(TestObject,str1));
    EXPECT_EQ(meta.get_field("str2").offset, offsetof(TestObject,str2));
    EXPECT_EQ(meta.get_field("wstr1").offset, offsetof(TestObject,wstr1));
    EXPECT_EQ(meta.get_field("wstr2").offset, offsetof(TestObject,wstr2));
    EXPECT_EQ(meta.get_field("e1").offset, offsetof(TestObject,e1));
    EXPECT_EQ(meta.get_field("e2").offset, offsetof(TestObject,e2));

    EXPECT_THROW(meta.get_field("xx"), std::out_of_range);
}

}
