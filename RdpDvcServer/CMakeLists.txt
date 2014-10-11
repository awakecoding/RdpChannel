set(MODULE_NAME "rdp-dvc-server")
set(MODULE_PREFIX "RDP_DVC_SERVER")

include_directories(${WinPR_INCLUDE_DIR})
include_directories(${FreeRDP_INCLUDE_DIR})

set(${MODULE_PREFIX}_SRCS
	rdp_dvc_server.c
	rdp_dvc_server.h)

add_executable(${MODULE_NAME} ${${MODULE_PREFIX}_SRCS})

list(APPEND ${MODULE_PREFIX}_LIBS winpr)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

install(TARGETS ${MODULE_NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")
