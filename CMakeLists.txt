cmake_minimum_required(VERSION 3.10)
project(pdfTopng)


# 查找 Poppler 库
find_package(PkgConfig REQUIRED)
pkg_check_modules(Poppler REQUIRED poppler-cpp)

# 包含头文件路径
include_directories(${Poppler_INCLUDE_DIRS})

# 添加可执行文件
add_executable(pdfTopng main.cpp)

# 链接库
target_link_libraries(pdfTopng ${Poppler_LIBRARIES})
target_link_libraries(pdfTopng poppler-cpp png)

