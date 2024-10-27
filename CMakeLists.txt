cmake_minimum_required(VERSION 3.29)
project(ProyectoFinalAlgoritmos)

set(CMAKE_CXX_STANDARD 20)


include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(ProyectoFinalAlgoritmos main.cpp
        users.cpp
        users.h
        initialsetup.cpp
        initialsetup.h
        plans.cpp
        plans.h
        tickets.cpp
        tickets.h
        main.h
        jsonutils.cpp
        jsonutils.h
        userplans.cpp
        userplans.h)
