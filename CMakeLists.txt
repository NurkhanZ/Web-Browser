cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME webBrowser)

project(${PROJECT_NAME})

add_library(website_compiler_flags INTERFACE)
target_compile_features(website_compiler_flags INTERFACE cxx_std_17)

find_package(SDL2 REQUIRED)

find_package(SDL2_ttf REQUIRED)

find_package(SDL2_image REQUIRED)

find_package(Freetype REQUIRED)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# target_compile_options(website_compiler_flags INTERFACE 
#                       "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;>>"
#                       "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>")      

target_link_libraries(${PROJECT_NAME} PRIVATE mingw32
                                              SDL2::SDL2main
                                              SDL2::SDL2
                                              SDL2_image::SDL2_image
                                              SDL2_ttf::SDL2_ttf
                                              Freetype::Freetype
                                              website_compiler_flags
                                              )

file(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)