/*
 * common_types.h
 *
 *  Created on: Aug 14, 2015
 *      Author: chushen
 */

#ifndef UNITTESTS_UA_SRC_COMMON_TYPES_H_
#define UNITTESTS_UA_SRC_COMMON_TYPES_H_
#include "ua/macros.h"
#include "gtest/gtest.h"
#include <string>
#include <map>
#include <unordered_map>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <set>
#include <unordered_set>
#include "jsoncpp_support.h"
enum class UA_ENUM(Rgb,
        R,G,B
);
struct Abc{
    int a,b,c;
    UA_OBJECT(Abc, +a,+b,+c);
};
struct Record{
    int id;
    std::string name;
    UA_OBJECT(Record, +id,+name);
};

struct rgb_t{
    int R,G,B;
    UA_OBJECT(rgb_t, +R,+G,+B);
};
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct IntMapTestData{
    typedef T Type;
    template<typename U> struct Tag{};
    static T getPopulated(){
        return _getPopulated((const Tag<typename T::key_type>*)0);
    }
    static const sail::map_accessor& getAccessor(const Type&){
        return sail::metas::get_map_accessor<Type>();
    }
private:
    static T _getPopulated(const Tag<std::string>*){
        return T{ { "R", 0 }, { "G", 1 }, { "B", 2 } };
    }
    static T _getPopulated(const Tag<std::wstring>*){
        return T{ { L"R", 0 }, { L"G", 1 }, { L"B", 2 } };
    }
    static T _getPopulated(const Tag<Rgb>*){
        return T{ { Rgb::R, 0 }, { Rgb::G, 1 }, { Rgb::B, 2 } };
    }
};

template<>
struct IntMapTestData<rgb_t>{
    typedef rgb_t Type;
    static Type getPopulated(){
        Type result = {0,1,2};
        return result;
    }
    static const sail::map_accessor& getAccessor(const Type&){
        return sail::metas::get_map_accessor<Type>();
    }
};
template<>
struct IntMapTestData<Json::Value>{
    typedef Json::Value Type;
    static Type getPopulated(){
        Json::Value v;
        v["R"] = 0;
        v["G"] = 1;
        v["B"] = 2;
        return v;
    }
    static const sail::map_accessor& getAccessor(const Type& v){
        const sail::map_accessor* accessor =  sail::metas::get_meta_value<Type>()
                .to_map(&v);
        assert(accessor);
        return *accessor;
    }
};

typedef ::testing::Types<
        IntMapTestData<Json::Value>,
        IntMapTestData<rgb_t>,
        IntMapTestData<std::map<Rgb, int>>,
        IntMapTestData<std::map<std::string, int>>,
        IntMapTestData<std::multimap<std::string, int>>,
        IntMapTestData<std::unordered_map<std::string, int>>,
        IntMapTestData<std::unordered_multimap<std::string,int>>,
        IntMapTestData<std::map<std::wstring, int>>,
        IntMapTestData<std::multimap<std::wstring, int>>,
        IntMapTestData<std::unordered_map<std::wstring, int>>,
        IntMapTestData<std::unordered_multimap<std::wstring,int>>
        > IntMapTestDataList;
////////////////////////////////////////////////////////////////////////
template<typename T>
struct IntListTestData{
    typedef T Type;
    static T getPopulated(){
        return T{{0,1,2,3,4,5}};
    }
    static const sail::list_accessor& getAccessor(const Type&){
        return sail::metas::get_list_accessor<Type>();
    }
};
template<>
struct IntListTestData<Json::Value>{
    typedef Json::Value Type;
    static Type getPopulated(){
        Json::Value v;
        v.append(0);
        v.append(1);
        v.append(2);
        v.append(3);
        v.append(4);
        v.append(5);
        return v;
    }
    static const sail::list_accessor& getAccessor(const Type& v){
        const sail::list_accessor* accessor = sail::metas::get_meta_value<Type>().to_list(&v);
        assert(accessor);
        return *accessor;
    }
};
typedef ::testing::Types<
        IntListTestData<Json::Value>,
        IntListTestData<std::array<int,6>>,
        IntListTestData<std::vector<int>>,
        IntListTestData<std::forward_list<int>>,
        IntListTestData<std::list<int>>,
        IntListTestData<std::deque<int>>,
        IntListTestData<std::set<int>>,
        IntListTestData<std::multiset<int>>
        > IntListTestDataList;
#endif /* UNITTESTS_UA_SRC_COMMON_TYPES_H_ */
