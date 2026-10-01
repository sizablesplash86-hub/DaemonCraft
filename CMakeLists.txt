cmake_minimum_required(VERSION 3.10)

# Define project name and language
project(DAEMONCRAFT C)

# Set the C standard (C11 is standard, adjust if needed)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Automatically collect all .c files in the core directory
file(GLOB CORE_SOURCES CONFIGURE_DEPENDS "core/*.c")

# Create the executable from the collected core sources
add_executable(daemoncraft ${CORE_SOURCES})
