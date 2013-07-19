IF(APPLE) #cmake .. -DCMAKE_C_FLAGS='-arch x86_64 -arch i386'
	cmake_minimum_required(VERSION 2.8)
else(APPLE)
	cmake_minimum_required(VERSION 2.6)
endif(APPLE)

# Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  IF(POLICY CMP0017)
	cmake_policy(SET CMP0017 NEW)
  ENDIF()
  IF(POLICY CMP0020)
	cmake_policy(SET CMP0020 NEW)
  ENDIF()
endif (COMMAND cmake_policy)

set(PROJECT_NAME SkeletonExtractionTest)
set(PROJECT_DIR ../../components/Examples/Skeleton_Extraction)
set(PROJECT_TOOLS ${PROJECT_DIR}/src/tools)
set(CMAKE_BUILD_TYPE Debug)

PROJECT(${PROJECT_NAME})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake")
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${PROJECT_DIR}/cmake")
        
# Find OpenMesh libraries
FIND_PACKAGE(OpenMesh REQUIRED Core)
IF(OPENMESH_FOUND)
        INCLUDE_DIRECTORIES(${OPENMESH_INCLUDE_DIR})
        MESSAGE(STATUS "Found OPENMESH: ${OPENMESH_LIBRARIES}")
ELSE(OPENMESH_FOUND)
        MESSAGE(FATAL_ERROR "OPENMESH not found.")
ENDIF(OPENMESH_FOUND)

# Find Umfpack libraries
FIND_PACKAGE(Umfpack REQUIRED Core)
IF(UMFPACK_FOUND)
        INCLUDE_DIRECTORIES(${UMFPACK_INCLUDES})
        MESSAGE(STATUS "Found UMFPACK: ${UMFPACK_LIBRARIES}")
ELSE(UMFPACK_FOUND)
        MESSAGE(FATAL_ERROR "UMFPACK not found.")
ENDIF(UMFPACK_FOUND)

# Find Eigen libraries
FIND_PACKAGE(Eigen3 REQUIRED)
IF(EIGEN3_FOUND)
        INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
        MESSAGE(STATUS "Found EIGEN: ${EIGEN3_LIBRARIES}")
ELSE(EIGEN3_FOUND)
        MESSAGE(FATAL_ERROR "EIGEN3 not found.")
ENDIF(EIGEN3_FOUND)

file(
	GLOB_RECURSE
	source_files
        ${PROJECT_TOOLS}/*.cpp
)

INCLUDE_DIRECTORIES(${PROJECT_TOOLS})
add_executable(
	${PROJECT_NAME}
        ${source_files} main.cpp
)

#Link with OpenMesh
target_link_libraries( ${PROJECT_NAME} ${OPENMESH_LIBRARIES}  )


#Link with OpenMesh
target_link_libraries( ${PROJECT_NAME} ${UMFPACK_LIBRARIES})
