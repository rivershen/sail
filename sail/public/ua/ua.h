/*
 * ua.h
 *
 *  Created on: Aug 21, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_UA_H_
#define PUBLIC_UA_UA_H_
#include "ua_ref.h"
#include "maps.h"
#include "lists.h"
#include "values.h"
#include "enums.h"
#include "detail/traits.h"

/**
 * \mainpage Structural Access Infrastructure Layer
 *  \section background_sec Background
 *  A recursive variant type is a type whose instance can can hold a value of any types. In addition
 *  to scalar types, it can also contain a list or map whose elements/fields are also variant.
 *
 *  A variant is different to variables available natively to C++ language whose type is static,
 *  but are closer to variables in may dynamic languages, such as Javascript and Python.
 *
 *  In the rest of the document, we will use the term recursive variant type, variant type, dynamic
 *  type interchangeably.  The term variant, dynamic variable refer to an instance of variant type.
 *
 *  We will also use the term static typed variable and native variable interchangeably to
 *  refer to the instance of types provided by the C++ language and libraries, such as int, long,
 *  bool, std::string and std::vector<int>.
 *
 *
 *
 * \section intro_sec Introduction
 *  Structural Access Infrastructure Layer (abbreviated as SAIL) is a modern C++ library to create
 *  structure based type system on top of C++'s static name based type system. It detects and captures
 *  types' structure at compile time through C++ compile time meta-programming, save them into the
 *  associated mete data to be used at runtime.
 *
 *  With the captured meta data, SAIL can provide runtime type introspection and dynamic access capability
 *  to any C++'s static typed data structures with zero memory overhead and very small CPU overhead.
 *  With SAIL, developers can access any static typed  C++ variables as if they are recursive variants.
 *
 *  It also provides infrastructure to efficiently copy variables of distinct but structural equivalent
 *  types. It is possible to write generic routines to  convert between dynamic type variables and
 *  static typed variables.
 *
 *  variables.
 *  \section motive_sec Motivation
 *  Recursive variant types can provide flexibilities native types cannot provide.  People use variant
 *  types in C++ mostly because:
 *  - The program need to expose itself to a dynamic scripting language in which variables are dynamic
 *  in nature.
 *  - The input and output of the program is encoded in a format that is easily convertible to a variant
 *  type( e.g, a JSON-RPC client/server).
 *  - The program need to introspect the type, structure and capability of its variable at runtime.
 *
 *  While the use of variant types brings certain convenience to the program, it also has its drawbacks:
 *  - It bypasses the static type checking provided by C++ compiler. Programmer need to ensure the
 *  variant contains the correct type themselves. This makes C++ source code verbose and less readable.
 *  - Modern IDE features, such as refactoring and code completion are less accessible to C++ source
 *  code using variant excessively.
 *  - It introduce extra runtime memory and CPU overhead.
 *
 *  To mitigate the drawbacks related to using variant directly in application logic, some
 *  programmers tend to convert variant types into native types before entering application logic
 *  layer and convert the result back into variant when application logic ends. However, this translation
 *  layer is tedious to write, read and maintain.
 *
 *  SAIL provides an alternative solution to provide developers the same benefit of using a variant
 *  type without its drawbacks. A "shadow variant" -- a variant without its body( and overhead ).
 *
 *  Instead of creating a data structure to hold hierarchical data which inevitably requires dynamic memory
 *  allocation, and the runtime cost of copying the data round, a shadow variant do not store any data
 *  itself, it only hold a reference to a static typed variable and implement a read only variant like
 *  interface on top of it. Because of this, creating and copying a shadow variant is very fast, almost
 *  as fast as creating or copying a raw pointer (a shadow variant only contains two pointers, one points
 *  to the variable, the other to a static allocated meta data).
 *
 *  Developers can use the shadow variant and its native variable counterpart side by side. They can use
 *  the native variable for efficiency and type safety and use the shadow variant for runtime introspect
 *  and traverse; any modifications made to the native variable will be reflected by its shadow variant
 *  immediately.
 *
 *  To help programmers who want to use real variant as their applications' input and output and use
 *  native C++ types to implement the application logic, SAIL provides a single function call to convert
 *  between a variant and a native variable. This keeps the translation layer clean.
 *
 */

#endif /* PUBLIC_UA_UA_H_ */
