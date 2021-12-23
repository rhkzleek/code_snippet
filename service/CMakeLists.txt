#声明要求的最小版本
cmake_minimum_required(VERSION 2.6)
#项目名称
PROJECT(hello)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -O2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -O2")
#添加头文件的查找目录 && 在编译参数"-l" ，指定搜索头文件的路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src SEC_LIST)

ADD_EXECUTABLE(main ${SEC_LIST})
 
 
#EXECUTABLE_OUTPUT_PATH && LIBRARY_OUTPUT_PATH用来指定二进制文件位置和库文件路径，这里不包括中间文件
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
 
INSTALL(TARGETS main DESTINATION bin)

#test :make test，测试程序是否正常执行
ENABLE_TESTING()
ADD_TEST(NAME SimpleTest COMMAND main)

#增加客制化命令make run运行main
ADD_CUSTOM_TARGET(run COMMAND main)