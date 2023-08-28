cmake_minimum_required(VERSION 3.10)
set(TARGET todo_app_cpp_desktop)

project(${TARGET})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES dependencies/sqlite/*.c
                  dependencies/imgui/*.cpp
                  dependencies/imgui/backends/imgui_impl_win32.cpp
                  dependencies/imgui/backends/imgui_impl_dx11.cpp
                  src/*.cpp)

file(GLOB HEADERS dependencies/sqlite/*.h
                  dependencies/imgui/*.h
                  dependencies/imgui/backends/imgui_impl_win32.h
                  dependencies/imgui/backends/imgui_impl_dx11.h
                  src/*.hpp)

file(GLOB DEPS_SOURCES )

add_executable(${TARGET} ${SOURCES} ${HEADERS})

target_include_directories(${TARGET} PRIVATE dependencies/sqlite
                                             dependencies/imgui
                                             dependencies/imgui/backends
                                             src)
target_link_libraries(${TARGET} PRIVATE d3d11 
                                        d3dcompiler 
                                        dxguid)