cmake_minimum_required(VERSION 3.11)

# Require modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Cartharsis)

# ### Setup Directories ####
# Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# ### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

function(copy_sfml_dll MyTarget)
    add_custom_command(TARGET ${MyTarget} POST_BUILD # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_directory # which executes "cmake - E copy_if_different..."
        "${CMAKE_BINARY_DIR}/lib/sfml/bin/$<CONFIGURATION>" # <--this is in-file
        $<TARGET_FILE_DIR:${MyTarget}>) # <--this is out-file path
endfunction()

# ### Practical 1 ####
file(GLOB_RECURSE SOURCES code/*.cpp code/*.h)
add_executable(CARTHARSIS ${SOURCES})
target_include_directories(CARTHARSIS PRIVATE ${SFML_INCS})
target_link_libraries(CARTHARSIS sfml-graphics)
set_target_properties(CARTHARSIS
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)

copy_sfml_dll(CARTHARSIS)