cmake_minimum_required(VERSION 3.8)

project(PwnAdventure3Mod)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(PwnAdventure3Mod SHARED ${SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET PwnAdventure3Mod PROPERTY CXX_STANDARD 23)
endif()

target_include_directories(PwnAdventure3Mod PRIVATE
    "${CMAKE_SOURCE_DIR}/vendor/minhook/include"
)

target_link_libraries(PwnAdventure3Mod PRIVATE
    "${CMAKE_SOURCE_DIR}/vendor/minhook/lib/libMinHook.x86.lib"
)

# NOTE: Set your PwnAdventure3.exe absolute path in order to move the file into the game directory.
set(GAME_EXE "GAME_PATH")

add_custom_command(TARGET PwnAdventure3Mod POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:PwnAdventure3Mod>
        "${GAME_EXE}/../$<TARGET_FILE_NAME:PwnAdventure3Mod>"
)