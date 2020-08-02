cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(card)

find_package(Torch REQUIRED)

set(CMAKE_CXX_FLAGS "-O3 -D_GLIBCXX_USE_CXX11_ABI=0")

file(GLOB LIB_SOURCES ../../include/common/*.cpp)
file(GLOB LIB_HEADERS ../../include/GiglDefaultInclude.h ../../include/common/*.h)
add_library(gigl_common_lib ${LIB_SOURCES} ${LIB_HEADERS})
set_property(TARGET gigl_common_lib PROPERTY CXX_STANDARD 14)

add_executable(card.exe main.cpp Player.cpp Player.h card.generated.h)
target_include_directories(card.exe PUBLIC ../../include/)
target_link_libraries(card.exe gigl_common_lib ${TORCH_LIBRARIES})
target_compile_features(card.exe PUBLIC cxx_range_for)
set_property(TARGET card.exe PROPERTY CXX_STANDARD 14)
