cmake_minimum_required ( VERSION 3.1.0...3.27.0 )
project ( RayTracer LANGUAGES CXX )

# Set to C++11
set ( CMAKE_CXX_STANDARD 17 )
set ( CMAKE_CXX_STANDARD_REQUIRED True )
file(GLOB SOURCES "src/*.cpp")
add_executable(RayTracer ${SOURCES})

target_include_directories(RayTracer PRIVATE ${PROJECT_SOURCE_DIR}/src)

# Executables

set_property(TARGET RayTracer PROPERTY CXX_STANDARD 17)