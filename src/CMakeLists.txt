set(HEADER_FILES
    "${CMAKE_SOURCE_DIR}/include/Scenes/Event.hpp"
    "${CMAKE_SOURCE_DIR}/include/Scenes/Log.hpp"
    "${CMAKE_SOURCE_DIR}/include/Scenes/EventLog.hpp"
    "pch.h"
)

set(SOURCE_FILES
"Log.cpp"
"EventLog.cpp"
"pch.cpp"
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
        "${CMAKE_SOURCE_DIR}/include/Scenes"
    )

