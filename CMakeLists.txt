cmake_minimum_required(VERSION 3.10)
project(f5-tamarind)

add_subdirectory(thread-pool)

add_library(f5-tamarind INTERFACE)
target_include_directories(f5-tamarind INTERFACE include)
target_link_libraries(f5-tamarind INTERFACE thread-pool)
target_compile_features(f5-tamarind INTERFACE cxx_std_17)
install(DIRECTORY include/f5 DESTINATION include)
