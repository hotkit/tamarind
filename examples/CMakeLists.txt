add_executable(tamarind-counter counter.cpp)
target_link_libraries(tamarind-counter f5-tamarind fost-cli)
install(TARGETS tamarind-counter EXPORT tamarind-counter RUNTIME DESTINATION bin)
