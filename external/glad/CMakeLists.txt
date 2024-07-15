cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(GLAD)

add_library(glad STATIC src/glad.c include/glad/glad.h)
target_include_directories(glad PUBLIC include/)

