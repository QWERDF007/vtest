cmake_minimum_required(VERSION 3.30)

project("vtest"
        LANGUAGES C CXX
        VERSION 0.0.1
        DESCRIPTION "测试套件"
)

# Used when creating special builds
set(PROJECT_VERSION_SUFFIX "-beta")

# 如果用户没指定安装前缀
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" CACHE PATH "where ${PROJECT_NAME} will be installed" FORCE)
endif()

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

# 配置构建树的选项
option(${PROJECT_NAME_UPPER}_BUILD_TESTS "Enable testsuite" ON)

# 配置构建树
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

include(ConfigBuildTree)
include(ConfigCompiler)

# 第三方库
add_subdirectory(3rdparty EXCLUDE_FROM_ALL)

add_subdirectory(src)

# 构建测试用例
if(${PROJECT_NAME_UPPER}_BUILD_TESTS)
    enable_testing()
    # add_subdirectory(tests)
endif()