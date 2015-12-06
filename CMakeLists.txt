cmake_minimum_required(VERSION 3.3)
project(snake_xenzia)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES main.c snake.c snake.h food.h game.c game.h)
add_executable(snake_xenzia ${SOURCE_FILES})

target_link_libraries(snake_xenzia ncurses)