if(TARGET check)
    add_executable(tamarind-test-parser parser.cpp)
    target_link_libraries(tamarind-test-parser f5-tamarind fost-cli)
    add_dependencies(check tamarind-test-parser)

    set(parse-tests)
    macro(parse name)
        add_custom_command(OUTPUT ${name}.json
                COMMAND tamarind-test-parser -b false -o ${CMAKE_CURRENT_SOURCE_DIR}/${name}.json
                    ${CMAKE_CURRENT_SOURCE_DIR}/${name}.workflow)
        list(APPEND parse-tests ${name}.json)
    endmacro()

    parse(empty)
    parse(simple)

    add_custom_target(tamarind-parse-tests DEPENDS ${parse-tests})
    set_property(TARGET tamarind-parse-tests PROPERTY EXCLUDE_FROM_ALL TRUE)
    add_dependencies(check tamarind-parse-tests)

endif()
