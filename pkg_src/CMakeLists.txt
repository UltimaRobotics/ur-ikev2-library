cmake_minimum_required(VERSION 3.16)
project(OpenIKEv2Integration VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)

# Add libopenikev2 as subdirectory
add_subdirectory(libopenikev2)

# Include directories
include_directories(libopenikev2/src)
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

# Link libraries - directly link to libopenikev2 target
target_link_libraries(${PROJECT_NAME} 
    libopenikev2
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
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
