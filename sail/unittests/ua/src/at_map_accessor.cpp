/*
 * at_map_accessor.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: chushen
 */


#include "ua/ua.h"
#include "common_types.h"
#include "ua/detail/key_converter.h"
#include "gtest/gtest.h"


namespace{
using namespace sail;

template<typename T>
struct map_accessor_tests:public testing::Test{
};
TYPED_TEST_CASE(map_accessor_tests, IntMapTestDataList);

TYPED_TEST(map_accessor_tests, static_obtain){
    typedef typename TypeParam::Type Type;
    const Type& v = TypeParam::getPopulated();
    TypeParam::getAccessor(v);
}
TYPED_TEST(map_accessor_tests, dynamic_obtain){
    typedef typename TypeParam::Type Type;
    const Type& v = TypeParam::getPopulated();
    EXPECT_EQ(&TypeParam::getAccessor(v), metas::get_meta_value<Type>().to_map(&v));
    EXPECT_EQ(NULL, metas::get_meta_value<Type>().to_list(&v));
}

TYPED_TEST(map_accessor_tests, get_value_meta_type){
    typedef typename TypeParam::Type Type;
    const Type& r = TypeParam::getPopulated();
    const meta_value* value_meta = TypeParam::getAccessor(r).get_value_meta_type(&r);
    if(value_meta){
        EXPECT_EQ(&metas::get_meta_value<int>(),value_meta);
    }
}
TYPED_TEST(map_accessor_tests, for_each_with_string){
    struct Helper{
       static int check(const value_ref& item, void* context){
           Helper* self = static_cast<Helper*>(context);
           EXPECT_EQ(self->expected.at(self->receiver.get()), item.as<int>());
           return 0;
       }
       key_converter<std::string> receiver;
       std::map<std::string, int> expected;
       Helper() :expected({ { "R", 0 }, { "G", 1 }, { "B", 2 } }){
       }
   };
   Helper validator;
   typedef typename TypeParam::Type Type;
   const Type& map = TypeParam::getPopulated();
   TypeParam::getAccessor(map).for_each_field_of(&map,&Helper::check, validator.receiver,&validator);
}
//////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct call_exists_helper{
     static bool exists(const map_accessor& accessor, const void* v, const T& key){
       call_exists_helper<T> o(accessor, v, key);
       accessor.run_with_key_receiver(v, &call_exists, &o);
       return o._result;
   }
private:
   explicit call_exists_helper(const map_accessor& accessor, const void* v, const T& key)
   :_accessor(accessor),_address(v),_key(key), _result(false){
   }
   static void call_exists(key_receiver& receiver, void* context){
       call_exists_helper<T>* self = (call_exists_helper<T>*)context;
       ((generic_key_receiver&) receiver).set(self->_key);
       self->_result = self->_accessor.exists(self->_address,receiver);
   }
   const map_accessor& _accessor;
   const void* _address;
   const T& _key;
   bool _result;
   call_exists_helper* operator=(const call_exists_helper&) = delete;
};

TYPED_TEST(map_accessor_tests, exists_with_string){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_TRUE(call_exists_helper<std::string>::exists(TypeParam::getAccessor(m), &m, "R"));
    EXPECT_TRUE(call_exists_helper<std::string>::exists(TypeParam::getAccessor(m), &m, "G"));
    EXPECT_TRUE(call_exists_helper<std::string>::exists(TypeParam::getAccessor(m), &m, "B"));
    EXPECT_FALSE(call_exists_helper<std::string>::exists(TypeParam::getAccessor(m), &m, "X"));
}
TYPED_TEST(map_accessor_tests, exists_with_cstr){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_TRUE(call_exists_helper<const char*>::exists(TypeParam::getAccessor(m), &m, "R"));
    EXPECT_TRUE(call_exists_helper<const char*>::exists(TypeParam::getAccessor(m), &m, "G"));
    EXPECT_TRUE(call_exists_helper<const char*>::exists(TypeParam::getAccessor(m), &m, "B"));
    EXPECT_FALSE(call_exists_helper<const char*>::exists(TypeParam::getAccessor(m), &m, "X"));
}

TYPED_TEST(map_accessor_tests, exists_with_wstring){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_TRUE(call_exists_helper<std::wstring>::exists(TypeParam::getAccessor(m), &m, L"R"));
    EXPECT_TRUE(call_exists_helper<std::wstring>::exists(TypeParam::getAccessor(m), &m, L"G"));
    EXPECT_TRUE(call_exists_helper<std::wstring>::exists(TypeParam::getAccessor(m), &m, L"B"));
    EXPECT_FALSE(call_exists_helper<std::wstring>::exists(TypeParam::getAccessor(m), &m, L"X"));
}
TYPED_TEST(map_accessor_tests, exists_with_wcstr){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_TRUE(call_exists_helper<const wchar_t*>::exists(TypeParam::getAccessor(m), &m, L"R"));
    EXPECT_TRUE(call_exists_helper<const wchar_t*>::exists(TypeParam::getAccessor(m), &m, L"G"));
    EXPECT_TRUE(call_exists_helper<const wchar_t*>::exists(TypeParam::getAccessor(m), &m, L"B"));
    EXPECT_FALSE(call_exists_helper<const wchar_t*>::exists(TypeParam::getAccessor(m), &m, L"X"));
}

TYPED_TEST(map_accessor_tests, exists_with_enum){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_TRUE(call_exists_helper<Rgb>::exists(TypeParam::getAccessor(m), &m,  Rgb::R));
    EXPECT_TRUE(call_exists_helper<Rgb>::exists(TypeParam::getAccessor(m), &m, Rgb::G));
    EXPECT_TRUE(call_exists_helper<Rgb>::exists(TypeParam::getAccessor(m), &m, Rgb::B));
    std::map<std::string, int> x = {{"a",1}};
    x.clear();
}
/////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct get_field_helper{
     static value_ref get_field(const map_accessor& accessor, const void* v, const T& key){
         get_field_helper<T> o(accessor, v, key);
       accessor.run_with_key_receiver(v, &call_get_field, &o);
       return o._result;
   }
private:
   get_field_helper(const map_accessor& accessor, const void* v, const T& key)
   :_accessor(accessor),_address(v),_key(key){
   }
   static void call_get_field(key_receiver& receiver, void* context){
       get_field_helper<T>* self = (get_field_helper<T>*)context;
       ((generic_key_receiver&) receiver).set(self->_key);
       self->_result = self->_accessor.get_field_at(self->_address,receiver);
   }
   const map_accessor& _accessor;
   const void* _address;
   const T& _key;
   value_ref _result;
   get_field_helper* operator=(const get_field_helper&) = delete;
};

TYPED_TEST(map_accessor_tests, get_field_with_string){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_EQ(0, get_field_helper<std::string>::get_field(TypeParam::getAccessor(m), &m, "R")
            .template as<int>());
    EXPECT_EQ(1, get_field_helper<std::string>::get_field(TypeParam::getAccessor(m), &m, "G")
                .template as<int>());
    EXPECT_EQ(2, get_field_helper<std::string>::get_field(TypeParam::getAccessor(m), &m, "B")
                .template as<int>());
    EXPECT_TRUE(get_field_helper<std::string>::get_field(TypeParam::getAccessor(m), &m, "X").is_none());
}
TYPED_TEST(map_accessor_tests, get_field_with_cstr){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_EQ(0, get_field_helper<const char*>::get_field(TypeParam::getAccessor(m), &m, "R")
            .template as<int>());
    EXPECT_EQ(1, get_field_helper<const char*>::get_field(TypeParam::getAccessor(m), &m, "G")
                .template as<int>());
    EXPECT_EQ(2, get_field_helper<const char*>::get_field(TypeParam::getAccessor(m), &m, "B")
                .template as<int>());
    EXPECT_TRUE(get_field_helper<const char*>::get_field(TypeParam::getAccessor(m), &m, "X").is_none());
}
TYPED_TEST(map_accessor_tests, get_field_with_wstring){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_EQ(0, get_field_helper<std::wstring>::get_field(TypeParam::getAccessor(m), &m, L"R")
            .template as<int>());
    EXPECT_EQ(1, get_field_helper<std::wstring>::get_field(TypeParam::getAccessor(m), &m, L"G")
                .template as<int>());
    EXPECT_EQ(2, get_field_helper<std::wstring>::get_field(TypeParam::getAccessor(m), &m, L"B")
                .template as<int>());
    EXPECT_TRUE(get_field_helper<std::wstring>::get_field(TypeParam::getAccessor(m), &m, L"X").is_none());
}
TYPED_TEST(map_accessor_tests, get_field_with_wcstr){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_EQ(0, get_field_helper<const wchar_t*>::get_field(TypeParam::getAccessor(m), &m, L"R")
            .template as<int>());
    EXPECT_EQ(1, get_field_helper<const wchar_t*>::get_field(TypeParam::getAccessor(m), &m, L"G")
                .template as<int>());
    EXPECT_EQ(2, get_field_helper<const wchar_t*>::get_field(TypeParam::getAccessor(m), &m, L"B")
                .template as<int>());
    EXPECT_TRUE(get_field_helper<const wchar_t*>::get_field(TypeParam::getAccessor(m), &m, L"X").is_none());
}
TYPED_TEST(map_accessor_tests, get_field_with_enum){
    typedef typename TypeParam::Type Type;
    const Type& m = TypeParam::getPopulated();
    EXPECT_EQ(0, get_field_helper<Rgb>::get_field(TypeParam::getAccessor(m), &m, Rgb::R)
            .template as<int>());
    EXPECT_EQ(1, get_field_helper<Rgb>::get_field(TypeParam::getAccessor(m), &m, Rgb::G)
                .template as<int>());
    EXPECT_EQ(2, get_field_helper<Rgb>::get_field(TypeParam::getAccessor(m), &m, Rgb::B)
                .template as<int>());
}
////////////////////////////////////////////////////////////////////////////////////
TEST(map_accessor_tests2, get_value_meta_type){
    Record r;
    EXPECT_EQ(NULL, metas::get_map_accessor<Record>().get_value_meta_type(&r));
}
}

