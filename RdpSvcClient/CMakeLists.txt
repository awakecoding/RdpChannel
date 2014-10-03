
set(MODULE_NAME "rdp-svc-client")
set(MODULE_PREFIX "RDP_SVC_CLIENT")

include_directories(${WinPR_INCLUDE_DIR})
include_directories(${FreeRDP_INCLUDE_DIR})

set(${MODULE_PREFIX}_SRCS
	rdp_svc_client.c
	rdp_svc_client.h)

add_library(${MODULE_NAME} SHARED ${${MODULE_PREFIX}_SRCS})

set_target_properties(${MODULE_NAME} PROPERTIES PREFIX "")

list(APPEND ${MODULE_PREFIX}_LIBS winpr)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

install(TARGETS ${MODULE_NAME} DESTINATION "${CMAKE_INSTALL_LIBDIR}/freerdp")
