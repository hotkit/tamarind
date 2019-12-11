cmake_minimum_required(VERSION 3.10)
project(f5-tamarind)

add_subdirectory(makham)
add_subdirectory(thread-pool)
add_subdirectory(tamarind)

if(TARGET check)
    set_property(TARGET check PROPERTY EXCLUDE_FROM_ALL TRUE)
    add_subdirectory(test)
endif()
