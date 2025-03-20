cmake_minimum_required(VERSION 3.10)
project(untitled)

# 查找 Poppler 库

find_package(PkgConfig REQUIRED)
pkg_check_modules(Poppler REQUIRED poppler-cpp)

# 包含头文件路径
include_directories(${Poppler_INCLUDE_DIRS})
include_directories(/usr/include/poppler/cpp)
# 添加可执行文件
add_executable(untitled main.cpp)

# 链接库
target_link_libraries(untitled ${Poppler_LIBRARIES})
#cmake_minimum_required(VERSION 3.10)
#project(untitled)
#
## 使用 pkg-config 查找 Poppler
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(Poppler REQUIRED poppler-cpp)
#
## 包含头文件路径
#include_directories(${Poppler_INCLUDE_DIRS})
#
## 添加可执行文件
#add_executable(untitled main.cpp)
#
## 链接库
#target_link_libraries(untitled ${Poppler_LIBRARIES})