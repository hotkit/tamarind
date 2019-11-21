cmake_minimum_required(VERSION 3.10)
project(f5-tamarind)

add_library(f5-tamarind INTERFACE)
target_include_directories(f5-tamarind INTERFACE include)
target_compile_features(f5-tamarind INTERFACE cxx_std_17)
target_compile_options(f5-tamarind INTERFACE -fcoroutines-ts)
install(DIRECTORY include/f5 DESTINATION include)

if(TARGET check)
    set_property(TARGET check PROPERTY EXCLUDE_FROM_ALL TRUE)
    add_subdirectory(test)
endif()
