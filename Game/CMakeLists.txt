cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

project(Game)

set(EXECUTABLE_NAME ${PROJECT_NAME})

add_executable(${EXECUTABLE_NAME})

target_sources(${EXECUTABLE_NAME}
PRIVATE
    src/main.cpp
    src/Menu.cpp
    src/Utils.cpp
    src/Settings.cpp
    src/MusicPlayer.cpp
    src/Game.cpp
    src/GameWon.cpp
    src/GameOver.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)

add_subdirectory(SDL EXCLUDE_FROM_ALL)

# ttf
set(SDLTTF_VENDORED ON) 
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)	

# audio
set(SDLMIXER_MIDI_NATIVE OFF)     
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)     
set(SDLMIXER_MOD OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_VENDORED ON)  
add_subdirectory(SDL_mixer EXCLUDE_FROM_ALL)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer 
    SDL3::SDL3              
)

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC SDL_MAIN_USE_CALLBACKS)

if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        winmm
        ole32
        oleaut32
        imm32
        version
        setupapi
    )
    set(CMAKE_EXECUTABLE_SUFFIX ".exe")
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE -static-libgcc -static-libstdc++)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "-static")
endif()

macro(copy_helper filename)
    set(outname "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/${filename}")
    add_custom_command(POST_BUILD
        TARGET "${EXECUTABLE_NAME}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/src/${filename}" "${outname}"
        DEPENDS "${filename}"
    )
endmacro()
copy_helper("Inter-VariableFont.ttf")
copy_helper("Boss.mp3")
copy_helper("Retro.mp3")
copy_helper("Menu.mp3")
copy_helper("Hope.mp3")