add_library(f5-tamarind
        parse.cpp
        runtime.cpp
    )
target_include_directories(f5-tamarind PUBLIC ../include)
target_link_libraries(f5-tamarind PUBLIC fost-core f5-makham)
target_compile_features(f5-tamarind PUBLIC cxx_std_17)
install(DIRECTORY ../include/f5 DESTINATION include)

add_executable(tamarind main.cpp)
target_link_libraries(tamarind f5-tamarind fost-cli)

if(TARGET stress)
endif()
