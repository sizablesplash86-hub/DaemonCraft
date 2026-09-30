cmake_minimum_required(VERSION 3.10)

# Define project name and language
project(NAP C)

# Set the C standard (C11 is standard, adjust if needed)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Option A: Option 1 approach (Single executable compiled from multiple sources)
add_executable(daemoncraft daemoncraft.c web.c lan_ip.c port.c)
