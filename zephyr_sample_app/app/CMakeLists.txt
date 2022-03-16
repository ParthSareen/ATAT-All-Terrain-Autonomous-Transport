#-------------------------------------------------------------------------------
# Zephyr Application
#

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C VERSION 1.0.0)
target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/app/include src include)
target_sources(app PRIVATE 
    src/main.c
)
