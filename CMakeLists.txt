cmake_minimum_required(VERSION 3.30)
project(CWebServer)

# Collect all .cpp files in the src directory
file(GLOB_RECURSE SOURCES "./*.c")

# Add an executable target
add_executable(Server ${SOURCES})
