cmake_minimum_required(VERSION 3.30)
project(Peak-o-CAD LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Locate pre-fetched SFML
find_package(SFML COMPONENTS graphics window system REQUIRED)

# Locate OpenGL
find_package(OpenGL REQUIRED)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system OpenGL::GL)

target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()

