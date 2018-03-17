# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Project configuration
project(open-pode)
set(EXECUTABLE_NAME open-pode)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find pkgconfig
find_package(PkgConfig REQUIRED)

# Find Bluez5
pkg_check_modules(BLUEZ REQUIRED bluez)
include_directories(${BLUEZ_INCLUDE_DIRS})
link_directories(${BLUEZ_LIBRARY_DIRS})
list(APPEND LIBRARIES_LIST ${BLUEZ_LIBRARIES})

# Find Threads
#set( THREADS_PTHREAD_ARG "PLEASE_FILL_OUT-FAILED_TO_RUN" CACHE STRING "Result from TRY_RUN" FORCE)
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)
#list(APPEND LIBRARIES_LIST Threads::Threads)

# Find OpenCV
# WARNING : OpenCVConfig.cmake seems to be broken with gcc 7.2
#find_package(OpenCV  REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
#link_directories(${OpenCV_LIB_DIR})
#list(APPEND LIBRARIES_LIST opencv_core opencv_highgui opencv_imgproc opencv_videoio) #${OpenCV_LIBS})

# Set source and header directories
set(PROJECT_SOURCE_DIR src)
set(PROJECT_HEADER_DIR src)

# Find all header and source files
file(GLOB_RECURSE PROJECT_SOURCE_LIST ${PROJECT_HEADER_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.c ${PROJECT_HEADER_DIR}/*.hpp ${PROJECT_SOURCE_DIR}/*.cpp)

# Include the following directories
include_directories(${PROJECT_HEADER_DIR})
include_directories(${PROJECT_SOURCE_DIR})

# Add project static lib directory
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Add the executable
add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCE_LIST})

# Link all needed libraries
target_link_libraries(${EXECUTABLE_NAME} ${LIBRARIES_LIST} bcm2835.a)
