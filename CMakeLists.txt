cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
        SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)

project(WebCrawler)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(gumbo)
find_package(gumbo CONFIG REQUIRED)

hunter_add_package(
        Boost
        COMPONENTS
        program_options
        thread
)
find_package(
        Boost
        CONFIG REQUIRED
        program_options
        thread
)

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)
find_package(Threads)

add_executable(
        WebCrawler
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/Utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/Downloader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/Parser.cpp
)

target_include_directories(
        WebCrawler
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
        WebCrawler
        gumbo::gumbo
        Boost::program_options
        Boost::thread
        OpenSSL::SSL
)

