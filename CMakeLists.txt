cmake_minimum_required(VERSION 3.11)

# Require modern C++
set(CMAKE_CXX_STANDARD 20)
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
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

add_subdirectory("lib/b2d/Box2D")
set(B2D_INCS "lib/b2d/Box2D")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# # Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/")
target_link_libraries(lib_tile_level_loader sfml-graphics lib_maths)

# # Entity Component Model library
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm/")
target_link_libraries(lib_ecm PRIVATE lib_maths)

# Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

# # Engine Code
file(GLOB_RECURSE SOURCE_FILES lib_engine/*.cpp lib_engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" PUBLIC SYSTEM ${B2D_INCS})
target_link_libraries(lib_engine PRIVATE lib_tile_level_loader lib_maths lib_ecm Box2D sfml-graphics sfml-audio)

# # JSON Reader lib
file(GLOB_RECURSE SOURCE_FILES lib_JSON_reader/*.hpp lib_JSON_reader/*.h lib_JSON_reader/*.cpp)
add_library(lib_JSON_reader STATIC ${SOURCE_FILES})
target_include_directories(lib_JSON_reader INTERFACE "${CMAKE_SOURCE_DIR}/lib_JSON_reader/")
set_target_properties(lib_JSON_reader PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(lib_JSON_reader lib_maths)

function(copy_sfml_dll MyTarget)
    add_custom_command(TARGET ${MyTarget} POST_BUILD # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_directory # which executes "cmake - E copy_if_different..."
        "${CMAKE_BINARY_DIR}/lib/sfml/bin/$<CONFIGURATION>" # <--this is in-file
        $<TARGET_FILE_DIR:${MyTarget}>) # <--this is out-file path
endfunction()

# ### GAME ####
file(GLOB_RECURSE SOURCES code/*.cpp code/*.h)
file(GLOB_RECURSE CMPNTS code/components/*.cpp code/components/*.h)
file(GLOB_RECURSE SCENES code/scenes/*.cpp code/scenes/*.h)
add_executable(CARTHARSIS ${SOURCES})
source_group("components" FILES ${CMPNTS})
source_group("scenes" FILES ${SCENES})

target_include_directories(CARTHARSIS PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(CARTHARSIS lib_engine)

copy_sfml_dll(CARTHARSIS)
add_dependencies(CARTHARSIS copy_resources)

set_target_properties(CARTHARSIS
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)
