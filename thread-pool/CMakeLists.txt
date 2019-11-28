cmake_minimum_required(VERSION 3.10)
project(thread-pool CXX)

add_library(thread-pool INTERFACE)
target_include_directories(thread-pool INTERFACE include)
target_link_libraries(thread-pool INTERFACE)
target_compile_features(thread-pool INTERFACE cxx_std_17)
install(DIRECTORY include/thread-pool DESTINATION include)
