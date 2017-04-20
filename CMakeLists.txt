cmake_minimum_required(VERSION 3.7)
project(mms_lib)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c mms_api.c mms_api.h)
add_executable(mms_lib ${SOURCE_FILES})