set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/Scenes)

set(HEADER_FILES
        "${INCLUDE_DIR}/Event.hpp"
        "${INCLUDE_DIR}/Log.hpp"
        "${INCLUDE_DIR}/EventLog.hpp"
        "${INCLUDE_DIR}/Line.hpp"
        "${INCLUDE_DIR}/Section.hpp"
        "${INCLUDE_DIR}/ScriptReader.hpp"
        "pch.h"
        )

set(SOURCE_FILES
        "Log.cpp"
        "EventLog.cpp"
        "Line.cpp"
        "Section.cpp"
        "ScriptReader.cpp"
        )

set(ALL_FILES
        ${HEADER_FILES}
        ${SOURCE_FILES}
        )

## Set Visual Studio Source Groups
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})


## Create Target
add_library(${CMAKE_PROJECT_NAME} STATIC ${ALL_FILES})

## Target Dependencies
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
        )

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
        "${CMAKE_SOURCE_DIR}/external;"
        "${INCLUDE_DIR}"
        )

