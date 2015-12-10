/*
 * at_sfinae_method_detector.cpp
 *
 *  Created on: 2015-04-22
 *      Author: chushen
 */
#include <ua/detail/member_detector.h>
#include "gtest/gtest.h"
namespace{
class BaseClass{
public:
    void a();
    int b(int, int) const;
    template<typename T>
    void c(T);

    void d(int);
    void d(int) const;
    void d(float);
    static int d(char);

    static void s(int);
    int operator [](int);
    int operator [](int)const;
    template<typename T>
    int operator +(T);
private:
    void p();
protected:
    void p2();
};

class DerivedClass:public BaseClass{

};
class VirtuallyDerivedClass: virtual public BaseClass{
};

 typedef ::testing::Types<
        BaseClass,
        DerivedClass,
        VirtuallyDerivedClass > TestTypes;
}

template<typename K>
struct SfinaeMethodDetectorTests:public testing::Test{
};

TYPED_TEST_CASE(SfinaeMethodDetectorTests, TestTypes);

//////////////////////////////////////////////////////////////
namespace{
UA_DECLARE_METHOD_DETECTOR(has_a, void, a, ());
UA_DECLARE_METHOD_DETECTOR(has_b, int, b, (int, int) const);
UA_DECLARE_STATIC_METHOD_DETECTOR(has_s, void, s, (int));
}
TYPED_TEST(SfinaeMethodDetectorTests, matching_methods){
    // instance method
    EXPECT_TRUE(has_a<TypeParam>::exists);
    EXPECT_TRUE(has_b<TypeParam>::exists);

    //static methods
    EXPECT_TRUE(has_s<TypeParam>::exists);
}

////////////////////////////////////////////////////////////////
namespace{
UA_DECLARE_TEMPLATE_METHOD_DETECTOR(has_c, void, c, (int), (int));
UA_DECLARE_TEMPLATE_METHOD_DETECTOR(has_c2, void, c, (bool), (bool));
UA_DECLARE_TEMPLATE_METHOD_DETECTOR(has_c3, void, c, (char), (char));
UA_DECLARE_TEMPLATE_METHOD_DETECTOR(has_op3, int, operator+, (char), (char));
}
TYPED_TEST(SfinaeMethodDetectorTests, matching_template_methods){
    EXPECT_TRUE(has_c<TypeParam>::exists);
    EXPECT_TRUE(has_c2<TypeParam>::exists);
    EXPECT_TRUE(has_c3<TypeParam>::exists);
    EXPECT_TRUE(has_op3<TypeParam>::exists);
}

/////////////////////////////////////////////////////////////////
namespace{
UA_DECLARE_METHOD_DETECTOR(has_d, void, d, (int));
UA_DECLARE_METHOD_DETECTOR(has_d2, void, d, (int) const);
UA_DECLARE_METHOD_DETECTOR(has_d3, void, d, (float));
UA_DECLARE_STATIC_METHOD_DETECTOR(has_d4, int, d, (char));
UA_DECLARE_METHOD_DETECTOR(has_op, int, operator[], (int));
UA_DECLARE_METHOD_DETECTOR(has_op2, int, operator[], (int)const);
}
TYPED_TEST(SfinaeMethodDetectorTests, matching_overloaded_methods){
    EXPECT_TRUE(has_d<TypeParam>::exists);
    EXPECT_TRUE(has_d2<TypeParam>::exists);
    EXPECT_TRUE(has_d3<TypeParam>::exists);
    EXPECT_TRUE(has_d4<TypeParam>::exists);

    //overloaded operators
    EXPECT_TRUE(has_op<TypeParam>::exists);
    EXPECT_TRUE(has_op2<TypeParam>::exists);
}

//////////////////////////////////////////////////////////
namespace{
UA_DECLARE_METHOD_DETECTOR(has_a2, void, a, ()const);
UA_DECLARE_METHOD_DETECTOR(has_b2, int, b, (int, int));
UA_DECLARE_METHOD_DETECTOR(has_a3, int, a, ()const);
UA_DECLARE_METHOD_DETECTOR(has_b3, int, b, (int));
UA_DECLARE_TEMPLATE_METHOD_DETECTOR(has_c4, int, c, (char), (char));
}
TYPED_TEST(SfinaeMethodDetectorTests, mismatched_methods){
   // const qualifier mismatch
   EXPECT_FALSE(has_a2<TypeParam>::exists);
   EXPECT_FALSE(has_b2<TypeParam>::exists);

   // parameter/return type mismatch
   EXPECT_FALSE(has_a3<TypeParam>::exists);
   EXPECT_FALSE(has_b3<TypeParam>::exists);
   
   // parameter mismatched on template method 
   EXPECT_FALSE(has_c4<TypeParam>::exists);
}

#if UA_HAS_EXPRESSION_SFINAE_SUPPORT == 1
/////////////////////////////////////////////////////////////
namespace{
 UA_DECLARE_METHOD_DETECTOR(has_p, void, p, ());
 UA_DECLARE_METHOD_DETECTOR(has_p2, void, p, ());
}
TYPED_TEST(SfinaeMethodDetectorTests, none_public_methods){
    // private/protected methods should not be detected
    EXPECT_FALSE(has_p<TypeParam>::exists);
    EXPECT_FALSE(has_p2<TypeParam>::exists);
}
#endif
/////////////////////////////////////////////////////////////

namespace{
UA_DECLARE_METHOD_DETECTOR(has_o1, void, o, ());
UA_DECLARE_TEMPLATE_METHOD_DETECTOR(has_o2, void, o, (int), ());
UA_DECLARE_STATIC_METHOD_DETECTOR(has_o3, void, o, ());

}
TYPED_TEST(SfinaeMethodDetectorTests, none_existing_methods){
    EXPECT_FALSE(has_o1<TypeParam>::exists);
    EXPECT_FALSE(has_o2<TypeParam>::exists);
    EXPECT_FALSE(has_o3<TypeParam>::exists);
}

/////////////////////////////////////////////////////////////
namespace{
    UA_DECLARE_METHOD_DETECTOR(has_member, void, o, ());
    UA_DECLARE_STATIC_METHOD_DETECTOR(has_static_member, void, o, ());
    UA_DECLARE_TEMPLATE_METHOD_DETECTOR(has_template_member, void, o, (int), ());

}

TEST(SfinaeMethodDetectorTests2, primitive_type){
    EXPECT_FALSE(has_member<int>::exists);
    EXPECT_FALSE(has_static_member<int>::exists);
    EXPECT_FALSE(has_template_member<int>::exists);
}
