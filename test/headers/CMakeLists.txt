add_library(tamarind-headers-tests STATIC EXCLUDE_FROM_ALL
        node.cpp
    )
target_link_libraries(tamarind-headers-tests f5-tamarind)
add_dependencies(check tamarind-headers-tests)
