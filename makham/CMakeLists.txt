add_library(f5-makham
        executor.cpp
        task.cpp
        unit.cpp
    )
target_include_directories(f5-makham PUBLIC ../include)
target_link_libraries(f5-makham PUBLIC fost-core thread-pool)
target_compile_features(f5-makham PUBLIC cxx_std_17)
target_compile_options(f5-makham PUBLIC -fcoroutines-ts)
install(DIRECTORY ../include/f5 DESTINATION include)
