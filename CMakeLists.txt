cmake_minimum_required(VERSION 2.8)
project(snake_the_game)
#set(SOURCE main.cpp)
set(SOURCE main.cc snake.cpp)
add_executable(snake ${SOURCE})
target_include_directories(snake PRIVATE include/ /usr/include/SFML)
target_link_libraries(snake sfml-graphics sfml-window sfml-system sfml-audio)
install(TARGETS snake DESTINATION "./")
