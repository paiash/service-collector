cmake_minimum_required(VERSION 3.5)
project(ServiceCollector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

set(SOURCE_FILES ../main.cpp)

add_executable(service_collector ${SOURCE_FILES})
target_link_libraries(service_collector Threads::Threads)

set(TEST_SOURCE_FILES ../serviceCollectorTest.cpp)

add_executable(service_collector_test ${TEST_SOURCE_FILES})
target_link_libraries(service_collector_test GTest::GTest GTest::Main Threads::Threads)

add_custom_target(run_tests
    COMMAND service_collector_test
    DEPENDS service_collector_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(create_output_dir
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/collected_data
)

set(TRIGGER_DIR "--trigger-dir")
set(OUTPUT_DIR "--output-dir")

set(DEFAULT_TRIGGER_DIR "default_input_dir" CACHE STRING "../input")
set(DEFAULT_OUTPUT_DIR  "default_output_dir" CACHE STRING "../output")

set(DEFAULT_COMMAND_LINE_ARGS "${TRIGGER_DIR} ${DEFAULT_TRIGGER_DIR} ${OUTPUT_DIR} ${DEFAULT_OUTPUT_DIR}")

set(DEFAULT_TEST_COMMAND_LINE_ARGS "${TRIGGER_DIR} ${DEFAULT_TRIGGER_DIR} ${OUTPUT_DIR} ${DEFAULT_OUTPUT_DIR}")

target_compile_definitions(service_collector PUBLIC DEFAULT_COMMAND_LINE_ARGS="${DEFAULT_COMMAND_LINE_ARGS}")
target_compile_definitions(service_collector_test PUBLIC DEFAULT_COMMAND_LINE_ARGS="${DEFAULT_TEST_COMMAND_LINE_ARGS}")

