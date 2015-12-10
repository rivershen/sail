AUX_SOURCE_DIRECTORY(src ALL_TESTS) 
add_executable(tvs_unittests
src/main.cpp
src/at_sfinae_method_detector.cpp
src/at_sfinae_type_detector.cpp
src/at_macros.cpp
src/at_value_traits.cpp
src/at_converter.cpp
src/at_meta_value.cpp
src/at_list_accessor.cpp
src/at_map_accessor.cpp
src/at_meta_class.cpp
src/at_meta_enum.cpp
src/at_key_converter.cpp
src/at_value_ref.cpp
src/at_map_ref.cpp
src/at_list_ref.cpp
)
target_link_libraries(tvs_unittests gtest jsoncpp ${CMAKE_THREAD_LIBS_INIT})
