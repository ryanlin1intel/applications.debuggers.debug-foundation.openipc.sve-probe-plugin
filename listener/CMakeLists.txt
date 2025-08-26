cmake_minimum_required(VERSION 3.20)

project(receiver LANGUAGES CXX)

# Choose your standard (adjust if you need)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# App target
add_executable(receiver
    src/receiver.cpp
    src/shift.cpp
    src/utils.cpp
)

# Public headers live in include/
target_include_directories(receiver
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# MSVC / Windows-specific bits to match cl command
if (MSVC)
    # /EHsc matches exception model request
    target_compile_options(receiver PRIVATE /EHsc)
endif()

# Link system libs (Windows)
if (WIN32)
    target_link_libraries(receiver PRIVATE ws2_32 msxml6)
endif()
