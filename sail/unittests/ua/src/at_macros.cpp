/*
 * at_sfinae_misc.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: chushen
 */

#include <ua/macros.h>
#include "gtest/gtest.h"

TEST(preprocessor_test, for_each){
#define LOCAL_APPEND(a, b) b,
    std::vector<int> v = {
        UA_FOR_EACH(LOCAL_APPEND, 0,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
        31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
        51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
        61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
        71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
        81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
        91, 92, 93, 94, 95, 96
        )
    };
#undef LOCAL_APPEND
    EXPECT_EQ(96, v.size());
    EXPECT_EQ(1, v.front());
    EXPECT_EQ(96, v.back());
}

