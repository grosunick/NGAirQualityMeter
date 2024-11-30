cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE) # Define the build type
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})
if(CMAKE_BUILD_TYPE MATCHES Test)
    set(CMAKE_BUILD_TYPE "Debug")
    add_subdirectory(Lib/ngSTLib)
else()
    include("cmake/gcc-arm-none-eabi.cmake") # Include toolchain file
    set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # Enable compile command to ease indexing with e.g. clang

    # Enable CMake support for ASM, C and С++ languages
    enable_language(CXX C ASM)
    set(CMAKE_CXX_STANDARD 23)

    set(CMAKE_PROJECT_NAME mastering) # Set the project name
    project(${CMAKE_PROJECT_NAME}) # Core project settings

    include(cmake/app.cmake)
endif()