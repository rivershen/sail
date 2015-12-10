/*
 * at_list_accessor.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: chushen
 */

#include "ua/ua.h"
#include "common_types.h"
#include "gtest/gtest.h"


namespace{
using namespace sail;

template<typename T>
struct list_accessor_tests:public testing::Test{
};
TYPED_TEST_CASE(list_accessor_tests, IntListTestDataList);

TYPED_TEST(list_accessor_tests, static_obtain){
    typedef typename TypeParam::Type Type;
    const Type& v = TypeParam::getPopulated();
    TypeParam::getAccessor(v);
}
TYPED_TEST(list_accessor_tests, dynamic_obtain){
    typedef typename TypeParam::Type Type;
    const Type& v=TypeParam::getPopulated();
    EXPECT_EQ(&TypeParam::getAccessor(v), metas::get_meta_value<Type>().to_list(&v));
    EXPECT_EQ(NULL, metas::get_meta_value<Type>().to_map(&v));
}
TYPED_TEST(list_accessor_tests, for_each_element){
    struct Helper{
        int mCounter = 0;
        static int check(const value_ref& item, void* context){
            Helper* self = static_cast<Helper*>(context);
            EXPECT_EQ(self->mCounter, item.as<int>());
            ++self->mCounter;
            return 0;
        }
    };
    typedef typename TypeParam::Type Type;
    const Type& v = TypeParam::getPopulated();
    const list_accessor& ops = TypeParam::getAccessor(v);
    Helper helper;
    ops.for_each_element_of(&v,&Helper::check, &helper);
}
TYPED_TEST(list_accessor_tests, size_of){
    typedef typename TypeParam::Type Type;
    const Type& v = TypeParam::getPopulated();
    const list_accessor& ops = TypeParam::getAccessor(v);
    EXPECT_EQ(6u, ops.size_of(&v));
}
TYPED_TEST(list_accessor_tests, get_element_at){
    typedef typename TypeParam::Type Type;
    const Type& v = TypeParam::getPopulated();
    const list_accessor& ops = TypeParam::getAccessor(v);
    EXPECT_EQ(0, ops.get_element_at(&v, 0).template as<int>());
    EXPECT_EQ(1, ops.get_element_at(&v, 1).template as<int>());
    EXPECT_EQ(2, ops.get_element_at(&v, 2).template as<int>());
    EXPECT_EQ(3, ops.get_element_at(&v, 3).template as<int>());
    EXPECT_EQ(4, ops.get_element_at(&v, 4).template as<int>());
    EXPECT_EQ(5, ops.get_element_at(&v, 5).template as<int>());
    EXPECT_TRUE(ops.get_element_at(&v,6).is_none());
}
TYPED_TEST(list_accessor_tests, get_value_meta_type){
    typedef typename TypeParam::Type Type;
    const Type& v = TypeParam::getPopulated();
    const meta_value* meta = TypeParam::getAccessor(v).get_value_meta_type(&v);
    if(meta){
        EXPECT_EQ(&metas::get_meta_value<int>(), meta);
    }
}

TYPED_TEST(list_accessor_tests, load_from_same_type){
    typedef typename TypeParam::Type Type;
    const Type& from = TypeParam::getPopulated();
    Type to;
    const list_accessor& meta = TypeParam::getAccessor(from);
    EXPECT_EQ(bind_status::SUCCESS, meta.load_from(values::make_ref(from),&to));
    EXPECT_EQ(from, to);
}
TYPED_TEST(list_accessor_tests, load_from_different_type){
    typedef typename TypeParam::Type Type;
    const Type& expected = TypeParam::getPopulated();
    Json::Value from;
    from.append(0);
    from.append(1);
    from.append(2);
    from.append(3);
    from.append(4);
    from.append(5);
    Type to;
    const meta_value& meta = metas::get_meta_value<Type>();
    EXPECT_EQ(bind_status::SUCCESS, meta.load_from(values::make_ref(from),&to));
    EXPECT_EQ(expected, to);

}
}


