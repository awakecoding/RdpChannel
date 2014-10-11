cmake_minimum_required(VERSION 2.8)

project(RdpChannel C)

if(WITH_THIRD_PARTY)
	set(FREERDP_BUILD 1)
endif()

if(NOT FREERDP_BUILD)
	include(GNUInstallDirs)
	include(FindPackageHandleStandardArgs)

	find_package(WinPR CONFIG REQUIRED)
	find_package(FreeRDP CONFIG REQUIRED)

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
endif()

add_subdirectory(RdpSvcClient)
add_subdirectory(RdpSvcServer)

