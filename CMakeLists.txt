cmake_minimum_required(VERSION 3.18)
project(bamgp-project-template)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}>)
set(ASSETS_DIR_NAME assets)

set(BUILD_SHARED_LIBS           FALSE)

set(SDL2MIXER_VENDORED          TRUE)
set(SDL2MIXER_WAVE              TRUE)
set(SDL2MIXER_OPUS              FALSE)
set(SDL2MIXER_FLAC_LIBFLAC      FALSE)
set(SDL2MIXER_MOD_XMP           FALSE)
set(SDL2MIXER_MOD               FALSE)
set(SDL2MIXER_MIDI_FLUIDSYNTH   FALSE)

set(SDL2TTF_VENDORED            TRUE)

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_mixer)
add_subdirectory(external/SDL_ttf)
add_subdirectory(external/SDL_image)

file(GLOB_RECURSE SRC_SOURCES src/*.c*)
file(GLOB_RECURSE SRC_HEADERS src/*.h*)

add_executable(${PROJECT_NAME} ${SRC_SOURCES} ${SRC_HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL2-static
    SDL2main
    SDL2_mixer
    SDL2_ttf
    SDL2_image
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    SDL2-static
    SDL2main
    SDL2_mixer
    SDL2_ttf
    SDL2_image
)

if (DEFINED ASSETS_DIR_NAME)
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${ASSETS_DIR_NAME}/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ASSETS_DIR_NAME})
endif()