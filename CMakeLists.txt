cmake_minimum_required(VERSION 3.28)
project(dswet2)

set(CMAKE_CXX_STANDARD 17)

add_executable(dswet2 main.cpp
        wet2util.h
        dspotify25b2.cpp
        dspotify25b2.cpp
        UnionFind.h
        Song.h
        Genre.h)
