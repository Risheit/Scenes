set(HEADER_FILES

        )

set(SOURCE_FILES
        "EventTests.cpp"
        "EventLogTests.cpp"
        "LogTests.cpp"
        "Line.cpp"
        )

set(ALL_FILES
        ${HEADER_FILES}
        ${SOURCE_FILES}
        )

## Set Visual Studio Source Groups
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})

## Define Tests
create_gtest(EVENT_TEST EventTests.cpp)
create_gtest(EVENT_LOG_TEST EventLogTests.cpp)
create_gtest(LOG_TEST LogTests.cpp)
create_gtest(LINES_TEST LineTests.cpp)
