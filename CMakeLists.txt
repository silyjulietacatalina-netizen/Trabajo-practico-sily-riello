cmake_minimum_required(VERSION 3.20)
project(TP_Libros C)

# Lenguaje C estandar (C11), sin extensiones del compilador
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(TP_Libros TPlibros.c)
