cmake_minimum_required(VERSION 4.1)

target_sources(search_engine
        PUBLIC main.cpp
        ${INCLUDE_DIR}/FunctionsForUniqueWords.h
        ${INCLUDE_DIR}/ConverterJSON.h
        ConverterJSON.cpp
        ${INCLUDE_DIR}/InvertedIndex.h
        InvertedIndex.cpp
        ${INCLUDE_DIR}/SearchServer.h
        SearchServer.cpp)

target_include_directories(search_engine
        PUBLIC ${INCLUDE_DIR})