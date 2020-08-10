cmake_minimum_required(VERSION 2.8.4)
project(VoxelRecontruction)

set(CMAKE_VERBOSE_MAKEFILE OFF)

message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_definitions(-O0)
        add_definitions(-g3)
        add_definitions(-DDEBUG)
else(CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_BUILD_TYPE Release)
        add_definitions(-O3)
        add_definitions(-g0)
        add_definitions(-DNDEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_definitions(-std=c++11)
add_definitions(-DTIXML_USE_TICPP)
add_definitions(-pthread)

find_package(GLUT 3 REQUIRED)
find_package(OpenGL 1 REQUIRED)
find_package(OpenCV 2.4 COMPONENTS core highgui imgproc calib3d REQUIRED)
find_package(OpenMP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")

set(OpenCV_USE_SHARED_LIBS ON)

include_directories (${GLUT_INCLUDE_DIR})
include_directories (${OPENGL_INCLUDE_DIR})
include_directories (${OpenCV_INCLUDE_DIR})

#set the default path for built executables to the BUILD_TYPE directory ("Release" or "Debug")
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})

#############################################

add_executable (
	${CMAKE_PROJECT_NAME}

	#$ find src .|grep -v "\.svn"|grep -v "\./"|grep cpp|sort
	##########
	src/controllers/arcball.cpp
	src/controllers/Camera.cpp
	src/controllers/Glut.cpp
	src/controllers/Reconstructor.cpp
	src/controllers/Scene3DRenderer.cpp
	src/main.cpp
	src/utilities/General.cpp
	src/VoxelReconstruction.cpp
)

#############################################

target_link_libraries (${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries (${CMAKE_PROJECT_NAME} ${GLUT_LIBRARIES})
target_link_libraries (${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries (${CMAKE_PROJECT_NAME} ${OpenMP_LIBRARIES})
target_link_libraries (${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries (${CMAKE_PROJECT_NAME})
