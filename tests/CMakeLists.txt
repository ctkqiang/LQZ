# 单元测试与 mock 集成测试接线(CTest)
set(UTIL_SRC
    ${CMAKE_SOURCE_DIR}/src/util/sha256.cc
    ${CMAKE_SOURCE_DIR}/src/util/json.cc
    ${CMAKE_SOURCE_DIR}/src/util/args.cc)

foreach(test_name sha256 json args custody)
    add_executable(test_${test_name}
        test_main.cc
        test_${test_name}.cc
        ${UTIL_SRC}
        ${CMAKE_SOURCE_DIR}/src/evidence/evidence.cc)
    target_include_directories(test_${test_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME unit_${test_name} COMMAND test_${test_name})
endforeach()

# mock adb 集成测试:通过 PATH 注入假 adb/mvt-android。
# 跳过 Windows:mock 是 POSIX sh 脚本;单测已覆盖全部逻辑。
if(NOT WIN32)
    add_test(NAME cli_integration
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/integration_cli.sh
                $<TARGET_FILE:phone_forensic>
                ${CMAKE_CURRENT_BINARY_DIR}/cli_integration)
    set_tests_properties(cli_integration PROPERTIES
        ENVIRONMENT "PATH=${CMAKE_CURRENT_SOURCE_DIR}/mock:$ENV{PATH}")
endif()
