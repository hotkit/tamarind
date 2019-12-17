add_library(makham-headers-tests STATIC EXCLUDE_FROM_ALL
        async.cpp
        executor.cpp
        future.cpp
        task.cpp
        unit.cpp
    )
target_link_libraries(makham-headers-tests f5-tamarind)
add_dependencies(check makham-headers-tests)
