cmake_minimum_required(VERSION 3.24)
project(ReferencePlugin)

MESSAGE("Compiler: ${CMAKE_CXX_COMPILER_ID}")
# Setting global compiler options
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

function(set_cxx_standard target)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON)
endfunction()

# Defining OpenIPC::PluginInterface
add_library(PluginInterface INTERFACE)
add_library(OpenIPC::PluginInterface ALIAS PluginInterface)
target_include_directories(PluginInterface INTERFACE "include")
target_compile_definitions(PluginInterface INTERFACE PROBEPLUGIN_EXPORTS)
install(TARGETS PluginInterface
    EXPORT PluginInterfaceTargets
    PUBLIC_HEADER DESTINATION include
)

# Reference Plugin
add_library(ReferencePlugin SHARED "example/reference_plugin.cpp")
set_cxx_standard(ReferencePlugin)
target_link_libraries(ReferencePlugin OpenIPC::PluginInterface)
set_target_properties(ReferencePlugin PROPERTIES OUTPUT_NAME "ProbePluginReference_x64")

# Test
include(CTest)
enable_testing()
add_executable(ReferencePlugin_Test test/test.cpp)
set_cxx_standard(ReferencePlugin_Test)
target_link_libraries(ReferencePlugin_Test OpenIPC::PluginInterface)
target_compile_definitions(ReferencePlugin_Test PRIVATE PROBEPLUGIN_IMPORTS)
get_target_property(test_target_name ReferencePlugin OUTPUT_NAME)
add_test(Test1 ReferencePlugin_Test ${CMAKE_SHARED_LIBRARY_PREFIX}${test_target_name}${CMAKE_SHARED_LIBRARY_SUFFIX})


