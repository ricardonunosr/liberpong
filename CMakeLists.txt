cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(Pong)

find_package(OpenGL REQUIRED)

set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/Pong/lib
    CACHE PATH "Output directory for static libraries.")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/Pong/lib
    CACHE PATH "Output directory for shared libraries.")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/Pong/bin
    CACHE PATH "Output directory for executables and DLL's.")

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/Pong")

if(CMAKE_CONFIGURATION_TYPES)
  # Directory output files for multi config generators
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        "${OUTPUT_DIR}/bin"
        CACHE PATH "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        "${OUTPUT_DIR}/lib"
        CACHE PATH "" FORCE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        "${OUTPUT_DIR}/lib"
        CACHE PATH "" FORCE)
  endforeach()
endif()

if(WIN32)
  set(PLATFORM "win32")
elseif(UNIX)
  set(PLATFORM "linux")
else()
  set(PLATFORM "unknown")
endif()

message(STATUS "Detected Platform: ${PLATFORM}")

set(SOURCES
    src/entrypoint.cpp
    src/platform/platform_${PLATFORM}.cpp
    src/shader.cpp
    src/paddel.cpp
    src/ball.cpp
    src/game_object.cpp
    src/gl.cpp)

set(HEADERS
    src/platform/platform.h
    src/IDKMath.h
    src/shader.h
    src/paddel.h
    src/ball.h
    src/game_object.h
    src/pong.h
    src/gl
    src/KHR)

if(WIN32)
  set(RESOURCE_FILE "src/platform/resources.rc")
endif()

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCE_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC src/)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARY})

if(WIN32)
  set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY
      LINK_OPTIONS
      $<$<CXX_COMPILER_ID:MSVC>:$<IF:$<CONFIG:Release>,/SUBSYSTEM:WINDOWS,/SUBSYSTEM:CONSOLE>>
  )
else()
  set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_OPTIONS $<>)
  find_package(X11 REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
endif()

#
# Copy assets
#
add_custom_target(assets ALL)
add_custom_command(
  TARGET assets
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Coppying assets..."
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
