cmake_minimum_required(VERSION 3.21.1)

set(CMAKE_CXX_STANDARD 17)

project(Pong)

#GLFW additions
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Pong/lib 
  CACHE PATH "Output directory for static libraries.")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Pong/lib
  CACHE PATH "Output directory for shared libraries.")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Pong/bin
  CACHE PATH "Output directory for executables and DLL's.")

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/Pong")

if(CMAKE_CONFIGURATION_TYPES)
  # Directory output files for multi config generators
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${OUTPUT_DIR}/bin" CACHE PATH "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${OUTPUT_DIR}/lib" CACHE PATH "" FORCE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${OUTPUT_DIR}/lib" CACHE PATH "" FORCE)
  endforeach()
endif()

set(SOURCES 
    src/main.cpp
	src/shader.cpp
	src/paddel.cpp
	src/ball.cpp
	src/game_object.cpp
)

set(HEADERS
	src/shader.h
	src/IDKMath.h
	src/paddel.h
	src/ball.h
	src/game_object.h
	src/pong.h
)

link_directories("external/glfw/lib")
add_subdirectory("external/glad/")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glfw/include
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE 
		glfw3
		glad
)

#
# Copy assets
#
add_custom_target(assets ALL)
add_custom_command(TARGET assets POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Coppying assets..."
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
