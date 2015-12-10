/*
 * at_sfinae_type_detector.cpp
 *
 *  Created on: 2015-04-22
 *      Author: chushen
 */
#include <ua/detail/member_detector.h>
#include "gtest/gtest.h"
namespace{
    class Base{
    public:
        typedef int Integer;
        using Float = float;
        class partial_type;
    private:
        typedef char Char;
    protected:
        typedef long Long;
    };

    class Derived:public Base{};

    class VirtuallyDerived:virtual public Base{};

    typedef ::testing::Types<
           Base,
           Derived,
           VirtuallyDerived> TestTypes;
}

template<typename T>
struct SfinaeNestedTypeDetectorTests:public testing::Test{
};

TYPED_TEST_CASE(SfinaeNestedTypeDetectorTests, TestTypes);


//////////////////////////////////////////////////////////
namespace{
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_integer_type, Integer) ;
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_float_type, Float);
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_partial_type, partial_type);
}
TYPED_TEST(SfinaeNestedTypeDetectorTests, matching_types){
    EXPECT_TRUE(has_integer_type<TypeParam>::exists);
    EXPECT_TRUE(has_float_type<TypeParam>::exists);
    EXPECT_TRUE(has_partial_type<TypeParam>::exists);
}

//////////////////////////////////////////////////////////
#if UA_HAS_EXPRESSION_SFINAE_SUPPORT
namespace{
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_char_type, Char);
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_long_type, Long);

}
TYPED_TEST(SfinaeNestedTypeDetectorTests, none_public_types){
    // private/protected type should be excluded.
    EXPECT_FALSE(has_char_type<TypeParam>::exists);
    EXPECT_FALSE(has_long_type<TypeParam>::exists);
}
#endif

///////////////////////////////////////////////////////////
namespace{
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_other_type, Other);
}
TYPED_TEST(SfinaeNestedTypeDetectorTests, none_existence_types){
    // non-existence type
    EXPECT_FALSE(has_other_type<TypeParam>::exists);
}

///////////////////////////////////////////////////////////////
namespace{
    template<typename T>
      class Wrapper{
      public:
          typedef T value_type;
      };
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_value_type, value_type);
}
TEST(SfinaeNestedTypeDetectorTests2, parameterized_type){
    // on template class
    EXPECT_TRUE(has_value_type<Wrapper<int>>::exists);
}

///////////////////////////////////////////////////////////
namespace{
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_x_type, x_type);
}

TEST(SfinaeNestedTypeDetectorTests2, primitive_type){
    EXPECT_FALSE(has_x_type<int>::exists);
}
