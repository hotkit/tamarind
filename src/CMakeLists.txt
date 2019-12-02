add_library(f5-tamarind
        parse.cpp
    )
target_include_directories(f5-tamarind PUBLIC ../include)
target_link_libraries(f5-tamarind PUBLIC fost-core)
target_link_libraries(f5-tamarind PRIVATE thread-pool)
target_compile_features(f5-tamarind PUBLIC cxx_std_17)
target_compile_options(f5-tamarind PUBLIC -fcoroutines-ts)
install(DIRECTORY ../include/f5 DESTINATION include)
