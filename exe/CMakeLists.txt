add_executable(tamarind tamarind.cpp)
target_link_libraries(tamarind f5-tamarind fost-cli)

if(TARGET stress)
endif()
