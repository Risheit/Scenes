cmake_minimum_required(VERSION 3.22.2)

## Project properties
project(Scenes CXX)
set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

## Options
option(ENABLE_TESTING "Creates ctest tests" ON)


## Testing Tools
if (ENABLE_TESTING)
    # Include Google Test 
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Test Creation Function
    #   TEST_NAME - name of added test/executable
    #   TEST_FILE - name of source file containing test

    function(create_gtest TEST_NAME TEST_FILE)
        add_executable(${TEST_NAME} ${TEST_FILE})
        target_link_libraries(${TEST_NAME} ${CMAKE_PROJECT_NAME} gtest_main)
        target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
            "${CMAKE_SOURCE_DIR}/external/"
            "${CMAKE_SOURCE_DIR}/include/"
        )
        add_test(
            NAME ${TEST_NAME}
            COMMAND ${TEST_NAME}
        )   
        gtest_discover_tests(${TEST_NAME})
    endfunction()
endif()

## Set Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

## Subdirectories 
add_subdirectory(src)

if (ENABLE_TESTING)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests/unittests)
endif()