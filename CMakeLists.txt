cmake_minimum_required(VERSION 2.8)

project(RdpChannel C)

include(GNUInstallDirs)
include(FindPackageHandleStandardArgs)

find_package(WinPR CONFIG REQUIRED)
find_package(FreeRDP CONFIG REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

add_subdirectory(RdpSvcClient)
add_subdirectory(RdpSvcServer)

