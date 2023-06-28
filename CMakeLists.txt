cmake_minimum_required(VERSION 3.18)

# Set default build type if not specified by the user and normalize it
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set the project name
project(latencyMonitor)

# For OPCUA
find_package(open62541)

# Add compile options
add_compile_options(-Wall)

# Set the source files
set(SOURCE_FILES
    src/cli.c
    lib/latency.c
)

# Include directories
include_directories(inc)

# Create shared library
add_library(AppLatency SHARED ${SOURCE_FILES})

# Set output directory for the shared library
set_target_properties(AppLatency PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Link the shared library with open62541
target_link_libraries(AppLatency PRIVATE open62541::open62541)

# Create the executable
add_executable(cli ${SOURCE_FILES})

# Link the executable with AppLatency and open62541
target_link_libraries(cli PRIVATE AppLatency open62541::open62541)

# Set the output directory for the executable
set_target_properties(cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

