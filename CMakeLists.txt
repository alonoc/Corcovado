cmake_minimum_required (VERSION 3.1)
project (Corcovado)

#General Setup
include_directories(include)
file(GLOB TEST_SOURCES "test/*.cpp")

#Setup Testing
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
link_directories($(GTEST_SOURCE_DIR)/lib)

#Create tests executable
add_executable(CorcovadoTests ${TEST_SOURCES})

#Link tests against gtest and pthreads
target_link_libraries(CorcovadoTests gtest Threads::Threads)