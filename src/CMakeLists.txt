cmake_minimum_required(VERSION 3.10)

# Define project name and language
project(DAEMONCRAFT C)

# Set the C standard (C11 is standard, adjust if needed)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Automatically collect all .c files in the core directory    also... change this so this file is in the root & not here, just putting it here for now
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS "src/*.c")

# Create the executable from the collected core sources
add_executable(daemoncraft ${CORE_SOURCES})
target_include_directories(daemoncraft PRIVATE include)