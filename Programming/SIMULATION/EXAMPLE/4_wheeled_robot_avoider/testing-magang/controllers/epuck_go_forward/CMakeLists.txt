cmake_minimum_required(VERSION 3.15)

get_filename_component(PROJECT ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT})

# Check for WEBOTS_HOME environment variable
if(NOT DEFINED ENV{WEBOTS_HOME})
  message(FATAL_ERROR "WEBOTS_HOME environment variable not set. Please set it to your Webots installation directory.")
endif()

set(WEBOTS_HOME "$ENV{WEBOTS_HOME}")
string(REPLACE "\\" "/" WEBOTS_HOME "${WEBOTS_HOME}")

set(SOURCES 
    epuck_go_forward.cpp
)

set(LIBRARIES
    ${WEBOTS_HOME}/lib/controller/libController.a
    ${WEBOTS_HOME}/lib/controller/libCppController.a
)

# # Verify that each library file actually exists
# foreach(LIBRARY ${LIBRARIES})
#   if(NOT EXISTS "${LIBRARY}")
#     message(FATAL_ERROR "Webots library not found at '${LIBRARY}'. Please check your Webots installation.")
#   endif()
# endforeach()

# message(STATUS "Using Webots Library: ${LIBRARIES}")

include_directories(
    ${WEBOTS_HOME}/include/controller/cpp
    ${WEBOTS_HOME}/include/controller/c
)

# Setup the target executable.
add_executable(${PROJECT} ${SOURCES})
target_compile_definitions(${PROJECT} PRIVATE WEBOTS_USED_BY_CONTROLLER)
target_link_libraries(${PROJECT} PRIVATE ${LIBRARIES})

# Copy the target executable at the right location.
add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
  copy $<TARGET_FILE:${PROJECT}> ${CMAKE_CURRENT_SOURCE_DIR}
)