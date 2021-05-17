cmake_minimum_required(VERSION 3.0)
project(Twister)                            #项目名字

include_directories(./include)              #头文件位置
aux_source_directory(./src/ DIR_TOOT_SRCS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")            #开启调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")         #开启警告
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm")           #链接数学库
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")    #使用c++11

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

set(CMAKE_CXX_FLAGS_DEBUG   "-O0" )                     #调试不优化
set(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG " )           #release优化

# set(CMAKE_BUILD_TYPE "Debug")   #当前为debug模式
set(CMAKE_BUILD_TYPE "Release")

add_executable(${PROJECT_NAME} ${DIR_TOOT_SRCS})