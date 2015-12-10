/*
 * tvs_examples.cpp
 *
 *  Created on: May 5, 2015
 *      Author: chushen
 */
#include <ua/metas.h>
#include <vector>
#include <map>
#include "examples.h"

//! [extract_nested_type]
UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_value_type, value_type);
UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_key_type, key_type);
void extract_nested_type(){
	// std::vector has a nest type named value_type
	static_assert(has_value_type<std::vector<int>>::exists, "");

	// std::vector does not have a nest type named key_type
	static_assert(!has_key_type<std::vector<int>>::exists, "");
}
//! [extract_nested_type]

//![detect_methods]
UA_DECLARE_METHOD_DETECTOR(has_push_back, void, push_back,(const int&));
void detect_methods(){
	// signature matches
	static_assert(has_push_back<std::vector<int>>::exists, "");
	// vector<float>::push_back takes a float not an int.
	static_assert(!has_push_back<std::vector<float>>::exists, "");
}
//![detect_methods]

//![detect_template_methods]
UA_DECLARE_TEMPLATE_METHOD_DETECTOR(has_assign, void, assign, (int*), (int*, int*));
void detect_template_methods(){
	static_assert(has_assign<std::vector<int>>::exists, "");
}
//![detect_template_methods]

template <typename T>
struct Detector{
	UA_DECLARE_METHOD_DETECTOR(has_push_back, void, push_back, (const T&));
};

void parameterized_detector(){
	static_assert(Detector<float>::has_push_back<std::vector<float>>::exists, "");
}

//![declare_native_object_intrusive]

//![declare_native_object_non_intrusive]
#define PERSON_FIELDS +name, +year_of_birth
class Person{
protected:
      std::string name;
      int year_of_birth;
      UA_OBJECT(Person, PERSON_FIELDS);
};

class Employee:public Person{
    std::string position;
public:
     UA_OBJECT(Employee, PERSON_FIELDS, +position);
};
//![declare_native_object_intrusive]
#define PERSION2_FIELDS +name,+year_of_birth
class Person2{
protected:
      std::string name;
      int year_of_birth;
};

class Employee2:public Person2{
protected:
    std::string position;
};

UA_MAKE_OBJECT(Person2, PERSION2_FIELDS);
UA_MAKE_OBJECT(Employee2, PERSION2_FIELDS,+position);
//![declare_native_object_non_intrusive]

