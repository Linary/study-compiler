# TODO: 这个文件还是不太会写，找时间补充完整

cmake_minimum_required(VERSION 3.20)
# 设置项目名称
project(study-compiler)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE debug)
set(VCPKG_DIR "/Users/liningrui/Service/vcpkg")

add_definitions(-DSTUDY_COMPILER_HOME=${CMAKE_SOURCE_DIR})

# 增加一个cmake文件的目录
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(ThirdPartyConfig)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${VCPKG_DIR}/installed/x64-osx/include)
link_directories(${VCPKG_DIR}/installed/x64-osx/lib)
#link_directories(${VCPKG_DIR}/installed/x64-osx/share)

#add_executable(demo demo.cpp)       # 生成可执行文件
#add_library(common STATIC util.cpp) # 生成静态库
#add_library(common SHARED util.cpp) # 生成动态库或共享库

# aux_source_directory (dir VAR) 发现一个目录下所有的源代码文件并将列表存储在一个变量中
# 搜索当前目录下的所有.cpp文件
aux_source_directory(src SRC_LIST)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_LIST})

# 链接其他库文件
#target_link_directories(${PROJECT_NAME} glog)
