cmake_minimum_required(VERSION 3.22)
project(ProjetFEM C)

set(CMAKE_C_STANDARD 11)

add_executable(ProjetFEM src/main.c src/homework.c src/fem.c)
target_link_libraries(ProjetFEM m)