cmake_minimum_required(VERSION 3.30)
project(CWebServer)


file(GLOB SOURCES "./*.c")
file(GLOB DATASTRUCTURES_SOURCES "DataStructures/*.c")

list(APPEND SOURCES ${DATASTRUCTURES_SOURCES})

add_executable(Server ${SOURCES})
