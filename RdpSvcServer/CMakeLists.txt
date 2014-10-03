set(MODULE_NAME "rdp-svc-server")
set(MODULE_PREFIX "RDP_SVC_SERVER")

include_directories(${WinPR_INCLUDE_DIR})
include_directories(${FreeRDP_INCLUDE_DIR})

set(${MODULE_PREFIX}_SRCS
	rdp_svc_server.c
	rdp_svc_server.h)

add_executable(${MODULE_NAME} ${${MODULE_PREFIX}_SRCS})

set(${MODULE_PREFIX}_LIBS "")

if(WIN32)
	list(APPEND ${MODULE_PREFIX}_LIBS wtsapi32)
endif()

list(APPEND ${MODULE_PREFIX}_LIBS winpr)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

install(TARGETS ${MODULE_NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")
