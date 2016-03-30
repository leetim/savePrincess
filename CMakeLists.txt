cmake_minimum_required(VERSION 3.0)

project(savePrincess)

include_directories(./include)

set(CURSES_LIBRARY_NAME "curses")
set(NCURSES_LIBRARY_NAME "ncurses")

# find_library(NCURSES )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(SOURCE_EXECUTABLE ./source/main.cpp)
set(CHARACTER_SOURCE ./source/character.cpp)
set(GAMEMAP_SOURCE ./source/game_map.cpp)
set(GENMAP_SOURCE ./source/genmap.cpp)

add_executable(main ${SOURCE_EXECUTABLE} ${CHARACTER_SOURCE} ${GAMEMAP_SOURCE} ${GENMAP_SOURCE})
target_link_libraries(main ncurses)