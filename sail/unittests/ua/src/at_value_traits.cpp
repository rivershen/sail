/*
 * at_sfinae_type_manifest.cpp
 *
 *  Created on: May 6, 2015
 *      Author: chushen
 */

#include <ua/metas.h>
#include "gtest/gtest.h"
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

using namespace sail;
template<typename T>
using array = std::array<T,1>;

template<typename T>
using map = std::map<int, T>;

template<typename T>
using multimap = std::multimap<int, T>;

template<typename T>
using unordered_map = std::unordered_map<int, T>;

template<typename T>
using unordered_multimap = std::unordered_multimap<int, T>;

struct dummy{};
TEST(value_traits_test, is_container){
	// none container or none supported container types
	EXPECT_FALSE(is_container<int>::value);
	EXPECT_FALSE(is_container<bool>::value);
	EXPECT_FALSE(is_container<dummy>::value);
	EXPECT_FALSE(is_container<std::stack<int>>::value);
	EXPECT_FALSE(is_container<std::queue<int>>::value);
	EXPECT_FALSE(is_container<std::priority_queue<int>>::value);

	//supported container types
	EXPECT_TRUE(is_container<array<int>>::value);
	EXPECT_TRUE(is_container<std::vector<int>>::value);
	EXPECT_TRUE(is_container<std::deque<int>>::value);
	EXPECT_TRUE(is_container<std::list<int>>::value);
	EXPECT_TRUE(is_container<std::forward_list<int>>::value);
	EXPECT_TRUE(is_container<map<int>>::value);
	EXPECT_TRUE(is_container<multimap<int>>::value);
	EXPECT_TRUE(is_container<unordered_map<int>>::value);
	EXPECT_TRUE(is_container<unordered_multimap<int>>::value);
	EXPECT_TRUE(is_container<std::set<int>>::value);
	EXPECT_TRUE(is_container<std::multiset<int>>::value);
	EXPECT_TRUE(is_container<std::unordered_set<int>>::value);
	EXPECT_TRUE(is_container<std::unordered_multiset<int>>::value);

}

TEST(value_traits_test, is_map){
	//none map
	EXPECT_FALSE(is_map<array<int>>::value);
	EXPECT_FALSE(is_map<std::vector<int>>::value);
	EXPECT_FALSE(is_map<std::deque<int>>::value);
	EXPECT_FALSE(is_map<std::list<int>>::value);
	EXPECT_FALSE(is_map<std::forward_list<int>>::value);
	EXPECT_FALSE(is_map<std::set<int>>::value);
	EXPECT_FALSE(is_map<std::multiset<int>>::value);
	EXPECT_FALSE(is_map<std::unordered_set<int>>::value);
	EXPECT_FALSE(is_map<std::unordered_multiset<int>>::value);

	//map
	EXPECT_TRUE(is_map<map<int>>::value);
	EXPECT_TRUE(is_map<multimap<int>>::value);
	EXPECT_TRUE(is_map<unordered_map<int>>::value);
	EXPECT_TRUE(is_map<unordered_multimap<int>>::value);
}

TEST(value_traits_test, is_set){
	//none set
	EXPECT_FALSE(is_set<array<int>>::value);
	EXPECT_FALSE(is_set<std::vector<int>>::value);
	EXPECT_FALSE(is_set<std::deque<int>>::value);
	EXPECT_FALSE(is_set<std::list<int>>::value);
	EXPECT_FALSE(is_set<std::forward_list<int>>::value);
	EXPECT_FALSE(is_set<map<int>>::value);
	EXPECT_FALSE(is_set<multimap<int>>::value);
	EXPECT_FALSE(is_set<unordered_map<int>>::value);
	EXPECT_FALSE(is_set<unordered_multimap<int>>::value);

	//set
	EXPECT_TRUE(is_set<std::set<int>>::value);
	EXPECT_TRUE(is_set<std::multiset<int>>::value);
	EXPECT_TRUE(is_set<std::unordered_set<int>>::value);
	EXPECT_TRUE(is_set<std::unordered_multiset<int>>::value);
}

TEST(value_traits_test, is_list){
    //none list
    EXPECT_FALSE(is_list<std::set<int>>::value);
    EXPECT_FALSE(is_list<std::multiset<int>>::value);
    EXPECT_FALSE(is_list<std::unordered_set<int>>::value);
    EXPECT_FALSE(is_list<std::unordered_multiset<int>>::value);

    //list
    EXPECT_TRUE(is_list<array<int>>::value);
    EXPECT_TRUE(is_list<std::vector<int>>::value);
    EXPECT_TRUE(is_list<std::deque<int>>::value);
    EXPECT_TRUE(is_list<std::list<int>>::value);
    EXPECT_TRUE(is_list<std::forward_list<int>>::value);
}

namespace internal{
struct A{
    int a;
    UA_OBJECT(A, +a);
};
template<typename UA_T, typename UA_VISITOR>
inline void _ua_visit_native_object(UA_T& obj, UA_VISITOR& visitor){
    UA_FOR_EACH(UA_VISIT_FIELD, UA_EMPTY_TOKEN, UA_EMPTY_TOKEN)
}
template<typename T>
void func(){}

struct A2:public A{
    int a2;
    UA_OBJECT(A2, +a, +a2);
};
struct B{
    int b;
};
UA_MAKE_OBJECT(B, +b);

struct B2:public B{
    int b2;
};
UA_MAKE_OBJECT(B2, +b, +b2);
}

namespace internal2{
struct A3: public internal::A{
    int a3;
    UA_OBJECT(A3, +a,+a3);
};

struct B3:public internal::B{
    int b3;
};
UA_MAKE_OBJECT(B3, +b, +b3)
}

TEST(value_traits_test, is_native_object){
    //false
    EXPECT_FALSE(is_native_object<int>::value);
    EXPECT_FALSE(is_native_object<array<int>>::value);
    //true
    // classes declared using UA_NATIVE_OBJECT/UA_MAKE_NATIVE_OBJECT
    EXPECT_TRUE(is_native_object<internal::A>::value);
    EXPECT_TRUE(is_native_object<internal::B>::value);

    // classes declared using UA_NATIVE_OBJECT_DERIVED/UA_NAKE_NATIVE_OBJECT_DERIVED
    EXPECT_TRUE(is_native_object<internal::A2>::value);
    EXPECT_TRUE(is_native_object<internal::B2>::value);

    // derived class from other name space.
    EXPECT_TRUE(is_native_object<internal2::A3>::value);
    EXPECT_TRUE(is_native_object<internal2::B3>::value);
}

TEST(value_traits_test, can_get_size){
    //false
    EXPECT_FALSE(can_get_size<int>::value);
    EXPECT_FALSE(can_get_size<std::forward_list<int>>::value);
    //true
    EXPECT_TRUE(can_get_size<array<int>>::value);
    EXPECT_TRUE(can_get_size<std::vector<int>>::value);
    EXPECT_TRUE(can_get_size<std::list<int>>::value);
    EXPECT_TRUE(can_get_size<map<int>>::value);
    EXPECT_TRUE(can_get_size<std::set<int>>::value);
}

TEST(value_traits_test, can_clear){
    //false
    EXPECT_FALSE(can_clear<int>::value);
    EXPECT_FALSE(can_clear<array<int>>::value);

    //true
    EXPECT_TRUE(can_clear<std::vector<int>>::value);
    EXPECT_TRUE(can_clear<std::list<int>>::value);
    EXPECT_TRUE(can_clear<map<int>>::value);
    EXPECT_TRUE(can_clear<std::set<int>>::value);
    EXPECT_TRUE(can_clear<std::forward_list<int>>::value);
}

TEST(value_traits_test, can_declare_as_key){
    EXPECT_FALSE(key_trait<int>::can_declare);
    EXPECT_FALSE(key_trait<const char*>::can_declare);
    EXPECT_FALSE(key_trait<const wchar_t*>::can_declare);

    EXPECT_TRUE(key_trait<std::string>::can_declare);
    EXPECT_TRUE(key_trait<std::wstring>::can_declare);
}

TEST(value_traits_test, can_pass_out_as_key){
    EXPECT_FALSE(key_trait<int>::can_output);
    EXPECT_FALSE(key_trait<const char*>::can_output);
    EXPECT_FALSE(key_trait<const wchar_t*>::can_output);

    EXPECT_TRUE(key_trait<std::string>::can_output);
    EXPECT_TRUE(key_trait<std::wstring>::can_output);
}
TEST(value_traits_test, can_pass_in_as_key){
    EXPECT_FALSE(key_trait<int>::can_input);

    EXPECT_TRUE(key_trait<const char*>::can_input);
    EXPECT_TRUE(key_trait<const wchar_t*>::can_input);
    EXPECT_TRUE(key_trait<std::string>::can_input);
    EXPECT_TRUE(key_trait<std::wstring>::can_input);
}
namespace internal{
    enum class Abc{
        a,b,c
    };
    enum class UA_ENUM(Xyz,
        x,y,z
    );
    enum class Asd{
        a,s,d
    };
    UA_MAKE_ENUM(Asd, a,s,d);
    struct Holder{
        enum UA_NESTED_ENUM(Rgb, R,G,B);
    };

    struct Holder2{
        enum Rgb{R,G,B};
    };
    UA_MAKE_ENUM(Holder2::Rgb, R,G,B);
}

TEST(value_traits_test, is_native_enum){
    EXPECT_FALSE(is_native_enum<internal::Abc>::value);
    EXPECT_TRUE(is_native_enum<internal::Xyz>::value);
    EXPECT_TRUE(is_native_enum<internal::Asd>::value);
    EXPECT_TRUE(is_native_enum<internal::Holder::Rgb>::value);
    EXPECT_TRUE(is_native_enum<internal::Holder2::Rgb>::value);

}

