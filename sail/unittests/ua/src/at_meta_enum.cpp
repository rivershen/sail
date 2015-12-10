/*
 * at_meta_enum.cpp
 *
 *  Created on: Aug 10, 2015
 *      Author: chushen
 */
#include "ua/macros.h"
#include "ua/enums.h"
#include "ua/metas.h"
#include "gtest/gtest.h"
#include <string>
using namespace sail;
namespace internal2{
    enum  UA_ENUM(Alphabet,
        a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,
        u,v,w,x,y,z
    );
    // don't change the white space in this macro parameters,
    // they are specially arranged to test the enum name
    // parsing routine.
    enum  UA_ENUM( Color,
        White = (0xFFFFFF),
        Black= 0x0,
        Red = 0xFF0000,
        Green = 0xFF00,
        Blue = 0xff ,
        Invalid = 0xFFFFFFFF
    );
    enum class Abc{
          a, b, c
     };
    UA_MAKE_ENUM(Abc, a, b, c);
}

using namespace internal2;
using namespace sail;

TEST(mete_enum_tests, Alphabet){
    EXPECT_EQ(26, metas::get_meta_enum<Alphabet>().size());
    EXPECT_EQ("a", enums::to_string(Alphabet::a));
    EXPECT_EQ("b", enums::to_string(Alphabet::b));
    EXPECT_EQ("c", enums::to_string(Alphabet::c));
    EXPECT_EQ("d", enums::to_string(Alphabet::d));
    EXPECT_EQ("e", enums::to_string(Alphabet::e));
    EXPECT_EQ("f", enums::to_string(Alphabet::f));
    EXPECT_EQ("g", enums::to_string(Alphabet::g));
    EXPECT_EQ("h", enums::to_string(Alphabet::h));
    EXPECT_EQ("i", enums::to_string(Alphabet::i));
    EXPECT_EQ("j", enums::to_string(Alphabet::j));
    EXPECT_EQ("k", enums::to_string(Alphabet::k));
    EXPECT_EQ("l", enums::to_string(Alphabet::l));
    EXPECT_EQ("m", enums::to_string(Alphabet::m));
    EXPECT_EQ("n", enums::to_string(Alphabet::n));
    EXPECT_EQ("o", enums::to_string(Alphabet::o));
    EXPECT_EQ("p", enums::to_string(Alphabet::p));
    EXPECT_EQ("q", enums::to_string(Alphabet::q));
    EXPECT_EQ("r", enums::to_string(Alphabet::r));
    EXPECT_EQ("s", enums::to_string(Alphabet::s));
    EXPECT_EQ("t", enums::to_string(Alphabet::t));
    EXPECT_EQ("u", enums::to_string(Alphabet::u));
    EXPECT_EQ("v", enums::to_string(Alphabet::v));
    EXPECT_EQ("w", enums::to_string(Alphabet::w));
    EXPECT_EQ("x", enums::to_string(Alphabet::x));
    EXPECT_EQ("y", enums::to_string(Alphabet::y));
    EXPECT_EQ("z", enums::to_string(Alphabet::z));

    EXPECT_EQ(Alphabet::a, enums::from_string<Alphabet>("a"));
    EXPECT_EQ(Alphabet::b, enums::from_string<Alphabet>("b"));
    EXPECT_EQ(Alphabet::c, enums::from_string<Alphabet>("c"));
    EXPECT_EQ(Alphabet::d, enums::from_string<Alphabet>("d"));
    EXPECT_EQ(Alphabet::e, enums::from_string<Alphabet>("e"));
    EXPECT_EQ(Alphabet::f, enums::from_string<Alphabet>("f"));
    EXPECT_EQ(Alphabet::g, enums::from_string<Alphabet>("g"));
    EXPECT_EQ(Alphabet::h, enums::from_string<Alphabet>("h"));
    EXPECT_EQ(Alphabet::i, enums::from_string<Alphabet>("i"));
    EXPECT_EQ(Alphabet::j, enums::from_string<Alphabet>("j"));
    EXPECT_EQ(Alphabet::k, enums::from_string<Alphabet>("k"));
    EXPECT_EQ(Alphabet::l, enums::from_string<Alphabet>("l"));
    EXPECT_EQ(Alphabet::m, enums::from_string<Alphabet>("m"));
    EXPECT_EQ(Alphabet::n, enums::from_string<Alphabet>("n"));
    EXPECT_EQ(Alphabet::o, enums::from_string<Alphabet>("o"));
    EXPECT_EQ(Alphabet::p, enums::from_string<Alphabet>("p"));
    EXPECT_EQ(Alphabet::q, enums::from_string<Alphabet>("q"));
    EXPECT_EQ(Alphabet::r, enums::from_string<Alphabet>("r"));
    EXPECT_EQ(Alphabet::s, enums::from_string<Alphabet>("s"));
    EXPECT_EQ(Alphabet::t, enums::from_string<Alphabet>("t"));
    EXPECT_EQ(Alphabet::u, enums::from_string<Alphabet>("u"));
    EXPECT_EQ(Alphabet::v, enums::from_string<Alphabet>("v"));
    EXPECT_EQ(Alphabet::w, enums::from_string<Alphabet>("w"));
    EXPECT_EQ(Alphabet::x, enums::from_string<Alphabet>("x"));
    EXPECT_EQ(Alphabet::y, enums::from_string<Alphabet>("y"));
    EXPECT_EQ(Alphabet::z, enums::from_string<Alphabet>("z"));

    int count = 0;
    for(auto& item: metas::get_meta_enum<Alphabet>()){
        (void) item;
        ++ count;
    }

    EXPECT_EQ(26, count);
}

TEST(mete_enum_testsmete_enum_tests, Colors){
    EXPECT_EQ(6, metas::get_meta_enum<Color>().size());
    EXPECT_EQ("White",  enums::to_string(Color::White));
    EXPECT_EQ("Black",  enums::to_string(Color::Black));
    EXPECT_EQ("Red",  enums::to_string(Color::Red));
    EXPECT_EQ("Green",  enums::to_string(Color::Green));
    EXPECT_EQ("Blue",  enums::to_string(Color::Blue));
    EXPECT_EQ("Invalid",  enums::to_string(Color::Invalid));

    EXPECT_EQ(Color::White, enums::from_string<Color>("White"));
    EXPECT_EQ(Color::Black, enums::from_string<Color>("Black"));
    EXPECT_EQ(Color::Red, enums::from_string<Color>("Red"));
    EXPECT_EQ(Color::Green, enums::from_string<Color>("Green"));
    EXPECT_EQ(Color::Blue, enums::from_string<Color>("Blue"));
    EXPECT_EQ(Color::Invalid, enums::from_string<Color>("Invalid"));
}



TEST(mete_enum_tests, Abcs){
    EXPECT_EQ(3, metas::get_meta_enum<Abc>().size());
    EXPECT_EQ("a",  enums::to_string(Abc::a));
    EXPECT_EQ("b",  enums::to_string(Abc::b));
    EXPECT_EQ("c",  enums::to_string(Abc::c));
    EXPECT_EQ(Abc::a, enums::from_string<Abc>("a"));
    EXPECT_EQ(Abc::b, enums::from_string<Abc>("b"));
    EXPECT_EQ(Abc::c, enums::from_string<Abc>("c"));
}











