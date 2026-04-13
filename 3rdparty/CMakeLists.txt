# GoogleTest -----------------------------
# 检查是否已经存在 GTest 目标（由父项目或其他依赖提供）
if(NOT TARGET GTest::gtest)
    message(STATUS "vtest: Adding GoogleTest from 3rdparty")
    set(INSTALL_GTEST off)
    set(BUILD_GMOCK on)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    add_subdirectory(googletest)
else()
    message(STATUS "vtest: GoogleTest already available, skipping 3rdparty/googletest")
endif()