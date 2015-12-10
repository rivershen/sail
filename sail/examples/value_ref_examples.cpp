/*
 * value_ref_examples.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: chushen
 */
#include "ua/ua.h"
#include <cassert>
#include <vector>
#include <map>
//! [make_value_ref_example]
void make_value_ref_example(){
    using namespace sail;
    //bind to a scalar value
    int int_val = 100;
    value_ref ref = values::make_ref(int_val);
    assert( int_val == ref.get_typed_reference<int>());

    //bind to a list-like container
    std::vector<int> vector_val = {{1,2,3}};
    ref = values::make_ref(vector_val);
    assert(ref.is_list());
    assert(ref.to_list().size() == 3);
    assert(ref.to_list()[0].get_typed_reference<int>() == 1);
    assert(ref.to_list()[1].get_typed_reference<int>() == 2);
    assert(ref.to_list()[2].get_typed_reference<int>() == 3);

    //bind to a map-like container
    std::map<std::string, int> map_val = {{"a", 1},{"b", 2},{"c", 3}};
    ref = values::make_ref(map_val);
    assert(ref.is_map());
    assert(ref.to_map().size() == 3);
    assert(ref.to_map()["a"].get_typed_reference<int>() == 1);
    assert(ref.to_map()["b"].get_typed_reference<int>() == 2);
    assert(ref.to_map()["c"].get_typed_reference<int>() == 3);

    // bind to a pointer causes a compilation error.
    //int * int_ptr = & int_val;
    //ref = values::make_ref(int_ptr);
}
//! [make_value_ref_example]

