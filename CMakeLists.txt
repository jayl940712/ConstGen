cmake_minimum_required (VERSION 2.8.11)
set (CMAKE_VERBOSE_MAKEFILE ON)

project (ConstGen)
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")

# output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#set(DOCUMENT_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/docs)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX_DIR ${CMAKE_INSTALL_PREFIX})
else()
    set(CMAKE_INSTALL_PREFIX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)
endif()
unset(CMAKE_INSTALL_PREFIX)
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

#pybind11
if (PYBIND11_DIR)
    set(PYBIND11_ROOT_DIR ${PYBIND11_DIR})
    message(STATUS "Use pybind11: ${PYBIND11_ROOT_DIR}")
else()
    set(PYBIND11_ROOT_DIR $ENV{PYBIND11_DIR})
    message(STATUS "Use system pybind11: ${PYBIND11_ROOT_DIR}")
endif()
unset(PYBIND11_DIR CACHE)

add_subdirectory(${PYBIND11_ROOT_DIR} "./pybind11")

# Python
find_package(PythonLibs REQUIRED)

#Print out the used build type
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Using build type DEBUG")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Using build type RELEASE")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Profile)
    message(STATUS "Using build type PROFILE")
ELSE()
    message(STATUS "Using build type DEFAULT: using Release flag")
    set(CMAKE_BUILD_TYPE Release)
ENDIF()
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-inline ")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -frename-registers -fprofile-use -fprofile-correction") 
set(CMAKE_CXX_FLAGS_PROFILE "-Ofast -pg -Winline")
set(CMAKE_EXE_LINKER_FLAGS "-pthread")

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

link_libraries (
    ${PYTHON_LIBRARIES}
)

file (GLOB SOURCES
    src/parser/*.h      src/parser/*.cpp
    src/global/*.h      src/global/*.cpp
    src/db/*.h          src/db/*.cpp
    src/sym_detect/*.h  src/sym_detect/*.cpp
)

file (GLOB PROJECT_SOURCES
    src/main/main.cpp
    ${SOURCES}
)

file(GLOB PY_API_SOURCES src/api/*.cpp ${SOURCES})

# Add modules to pybind
pybind11_add_module(${PROJECT_NAME} ${PY_API_SOURCES})

#add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
#set_target_properties (${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static")

