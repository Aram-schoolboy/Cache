cmake_minimum_required(VERSION 3.26)
project(se_ca24_backlog_cache_isa_Aram_schoolboy)

set(CMAKE_CXX_STANDARD 20)

add_executable(se_ca24_backlog_cache_isa_Aram_schoolboy
        main.cpp
        Cash.cpp
        Cash.h
        Parser.cpp
        Parser.h
        AsmExecutor.cpp
        AsmExecutor.h)
