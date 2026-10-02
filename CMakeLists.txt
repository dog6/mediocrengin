cmake_minimum_required(VERSION 4.1.1)

project(smallengine)


file(GLOB_RECURSE source_files "src/*.cpp" "external/*.cpp")
list(APPEND source_files "external/glad/glad.c")

find_package(OpenGL REQUIRED)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if(MSVC)
    add_compile_options(/FS)
endif()

add_executable(smallengine ${source_files})
target_include_directories(smallengine PRIVATE
    include/
    external/)

target_compile_features(smallengine PRIVATE cxx_std_17)

target_link_libraries(smallengine PRIVATE
    ${CMAKE_SOURCE_DIR}/external/GLFW/lib/glfw3.lib
    ${CMAKE_SOURCE_DIR}/external/assimp/assimp-vc143-mt.lib
    ${CMAKE_SOURCE_DIR}/external/lua/lua54.lib
    OpenGL::GL
    comdlg32
)