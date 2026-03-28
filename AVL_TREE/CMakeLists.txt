cmake_minimum_required(VERSION 3.10)
project(AVLTree C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SRC
  main.c
  avl_tree.c
  file.c
)

add_executable(AVLTree ${SRC})
