set(HEADER_FILES
    
)

set(SOURCE_FILES
    "EventTests.cpp"
)

set(ALL_FILES
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

## Set Visual Studio Source Groups
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})

## Define Tests
enable_testing()

# Test Event Class
set(TEST_DRIVER EVENT_DRIVER)
add_executable(${TEST_DRIVER} EventTest.cpp)
target_link_libraries(${TEST_DRIVER} PRIVATE ${CMAKE_PROJECT_NAME} gtest_main)
add_test(
    NAME EVENT_TEST 
    COMMAND ${TEST_DRIVER})
