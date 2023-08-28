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

include(FetchContent)
FetchContent_Declare( googletest 
                    URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
                    DOWNLOAD_EXTRACT_TIMESTAMP 1                    )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_TARGET ${TARGET}_test)
file(GLOB TEST_SOURCES test/*.cpp src/*.cpp)
file(GLOB TEST_HEADERS test/*.hpp src/*.hpp)
list(APPEND TEST_SOURCES ${SOURCES})
list(APPEND TEST_HEADERS ${HEADERS})

list(REMOVE_ITEM TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp )


add_executable(${TEST_TARGET} ${TEST_SOURCES} ${TEST_HEADERS} ${DEPS_SOURCES} test/main.cpp)

target_link_libraries(
  ${TEST_TARGET}
  GTest::gtest_main
  GTest::gmock_main
  d3d11
  d3dcompiler
  dxguid
)

target_include_directories(${TEST_TARGET} PRIVATE dependencies/sqlite
                                             dependencies/imgui
                                             dependencies/imgui/backends
                                             src)

add_definitions(-DUNIT_TEST_MODE)

add_test(
  NAME ${TEST_TARGET}
  COMMAND ${TEST_TARGET})