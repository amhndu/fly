cmake_minimum_required(VERSION 3.10)
project(Fly)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(BUILD_STATIC FALSE CACHE STRING "Set this to link external libraries statically")

if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wnull-dereference -g -Og")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

# Add directory containing FindSFML.cmake to module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")

# Add sources
file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/Debug/*.cpp"
    "${PROJECT_SOURCE_DIR}/extlibs/src/*.cpp"
)

# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#     add_definitions(-DDEBUG)
# endif()

# Copy shaders and resources
file(COPY shaders DESTINATION .)
file(COPY resources DESTINATION .)

# Will add __FILENAME__ macros for all source files, which is the filename without full find_path
# Courtesy of SO
function(define_file_basename_for_sources targetname)
    get_target_property(source_files "${targetname}" SOURCES)
    foreach(sourcefile ${source_files})
        # Get source file's current list of compile definitions.
        get_property(defs SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS)
        # Add the FILE_BASENAME=filename compile definition to the list.
        get_filename_component(basename "${sourcefile}" NAME)
        list(APPEND defs "__FILENAME__=\"${basename}\"")
        # Set the updated compile definitions on the source file.
        set_property(
            SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS ${defs})
    endforeach()
endfunction()

# Specify include Directory
include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/extlibs")

# Set static if BUILD_STATIC is set
if (BUILD_STATIC)
    set(SFML_STATIC_LIBRARIES TRUE)
    # Link libgcc and libstc++ statically as well
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
    endif()
endif()

# Find SFML
if (SFML_OS_WINDOWS AND SFML_COMPILER_MSVC)
    find_package(SFML 2 COMPONENTS main graphics window system REQUIRED)
else()
    find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
endif()

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SFML_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

add_executable(Fly ${SOURCES})
target_link_libraries(Fly ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

set_property(TARGET Fly PROPERTY CXX_STANDARD 14)
set_property(TARGET Fly PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(Fly)
define_file_basename_for_sources(Fly)
