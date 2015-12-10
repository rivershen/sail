/*
 * at_key_converter.cpp
 *
 *  Created on: Aug 11, 2015
 *      Author: chushen
 */
/*
 * at_universal_key_accessor.cpp
 *
 *  Created on: May 22, 2015
 *      Author: chushen
 */
#include "ua/metas.h"
#include "gtest/gtest.h"
using namespace sail;
namespace{
    enum class UA_ENUM(Color,
        Red,
        Green,
        Blue
    );
}
struct key_converter_tests:public testing::Test{
    const char* cs =
            //one byte per code point
            "a"
            //two bytes per code point
            "\xce\xa6"
            //three bytes per code point
            "\xe3\x87\x97"
            //four bytes per code point
            "\xf0\x9f\xa1\x84";

    const wchar_t* wcs =
            L"a\U000003a6\U000031d7\U0001f844";
    std::string str = std::string(cs);
    std::wstring wstr = std::wstring(wcs);
};

TEST_F(key_converter_tests, std_string){
    key_converter<std::string> converter;
    converter.set("");
    EXPECT_EQ("", converter.get());
    converter.set(L"");
    EXPECT_EQ("", converter.get());

    converter.set(cs);
    EXPECT_EQ(str, converter.get());
    converter.set(str);
    EXPECT_EQ(str, converter.get());

    converter.set(wcs);
    EXPECT_EQ(str, converter.get());
    converter.set(wstr);
    EXPECT_EQ(str, converter.get());

    converter.set(Color::Red);
    EXPECT_EQ("Red", converter.get());
    converter.set(Color::Green);
    EXPECT_EQ("Green", converter.get());
    converter.set(Color::Blue);
    EXPECT_EQ("Blue", converter.get());
}
TEST_F(key_converter_tests, c_str){
    key_converter<const char*> converter;
    const char* enmpty_str = "";
    converter.set(enmpty_str);
    EXPECT_EQ(enmpty_str, converter.get());
    converter.set(L"");
    EXPECT_EQ(0, ::strlen(converter.get()));

    converter.set(cs);
    EXPECT_EQ(str, converter.get());
    converter.set(str);
    EXPECT_EQ(str, converter.get());

    converter.set(wcs);
    EXPECT_EQ(str, converter.get());
    converter.set(wstr);
    EXPECT_EQ(str, converter.get());

    converter.set(Color::Red);
    EXPECT_EQ(std::string("Red"), converter.get());
    converter.set(Color::Green);
    EXPECT_EQ(std::string("Green"), converter.get());
    converter.set(Color::Blue);
    EXPECT_EQ(std::string("Blue"), converter.get());
}

TEST_F(key_converter_tests, std_wstring){
    key_converter<std::wstring> converter;
    converter.set("");
    EXPECT_TRUE(converter.get().empty());
    converter.set(L"");
    EXPECT_EQ(L"", converter.get());

    converter.set(cs);
    EXPECT_EQ(wstr, converter.get());
    converter.set(str);
    EXPECT_EQ(wstr, converter.get());

    converter.set(wcs);
    EXPECT_EQ(wstr, converter.get());
    converter.set(wstr);
    EXPECT_EQ(wstr, converter.get());

    converter.set(Color::Red);
    EXPECT_EQ(std::wstring(L"Red"), converter.get());
    converter.set(Color::Green);
    EXPECT_EQ(std::wstring(L"Green"), converter.get());
    converter.set(Color::Blue);
    EXPECT_EQ(std::wstring(L"Blue"), converter.get());
}

TEST_F(key_converter_tests, c_wstr){
    key_converter<const wchar_t*> converter;
    const wchar_t* enmpty_str = L"";
    converter.set("");
    EXPECT_EQ(0, ::wcslen(converter.get()));
    converter.set(enmpty_str);
    EXPECT_EQ(enmpty_str, converter.get());

    converter.set(cs);
    EXPECT_EQ(wstr, converter.get());
    converter.set(str);
    EXPECT_EQ(wstr, converter.get());

    converter.set(wcs);
    EXPECT_EQ(wstr, converter.get());
    converter.set(wstr);
    EXPECT_EQ(wstr, converter.get());

    converter.set(Color::Red);
    EXPECT_EQ(std::wstring(L"Red"), converter.get());
    converter.set(Color::Green);
    EXPECT_EQ(std::wstring(L"Green"), converter.get());
    converter.set(Color::Blue);
    EXPECT_EQ(std::wstring(L"Blue"), converter.get());
}

TEST_F(key_converter_tests, enum_color){
    key_converter<Color> converter;
    converter.set(Color::Red);
    EXPECT_EQ(Color::Red, converter.get());
    converter.set(Color::Green);
    EXPECT_EQ(Color::Green, converter.get());
    converter.set(Color::Blue);
    EXPECT_EQ(Color::Blue, converter.get());

    converter.set("Red");
    EXPECT_EQ(Color::Red, converter.get());
    converter.set(L"Green");
    EXPECT_EQ(Color::Green, converter.get());
    std::string s = "Blue";
    converter.set(s);
    EXPECT_EQ(Color::Blue, converter.get());

    std::wstring ws = L"XXXX";
    converter.set(ws);
    EXPECT_EQ(bind_status::INVALID_ENUM_NAME, converter.get_last_error());

}





