cmake_minimum_required(VERSION 3.10)

set (TARGET_NAME mfgfoundation)

project(${TARGET_NAME})

set(SOURECS_FILES
	src/main.cpp
	src/datastream.cpp
)

set(HEADER_FILES
	include/mfgfoundation/datastream.h
	include/mfgfoundation/mfg_lib_foundation_version.h
)

