cmake_minimum_required(VERSION 3.16)
project(OpenIKEv2Integration VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

# Add libopenikev2 as external project
set(LIBOPENIKEV2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libopenikev2")
set(LIBOPENIKEV2_INCLUDE "${LIBOPENIKEV2_ROOT}/src")
set(LIBOPENIKEV2_LIB "${LIBOPENIKEV2_ROOT}/build")

# Include directories
include_directories(${LIBOPENIKEV2_INCLUDE})
include_directories(${LIBOPENIKEV2_ROOT}/build)
include_directories(src)

# Source files
set(SOURCES
    src/main.cpp
    src/integration_layer.cpp
    src/session_manager.cpp
    src/state_monitor.cpp
    src/config_manager.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Threads::Threads
    ${LIBOPENIKEV2_LIB}/libopenikev2.a
    ssl
    crypto
)

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall 
    -Wextra 
    -O2 
    -DCPPHTTPLIB_OPENSSL_SUPPORT
)

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES config.json DESTINATION etc)
