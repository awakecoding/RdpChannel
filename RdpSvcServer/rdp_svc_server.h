#ifndef RDP_SVC_SERVER_MAIN_H
#define RDP_SVC_SERVER_MAIN_H

#include <winpr/crt.h>
#include <winpr/synch.h>
#include <winpr/thread.h>
#include <winpr/stream.h>
#include <winpr/collections.h>

#include <freerdp/api.h>
#include <freerdp/svc.h>
#include <freerdp/addin.h>
#include <freerdp/constants.h>

#ifndef WTS_CURRENT_SERVER_HANDLE
#define WTS_CURRENT_SERVER_HANDLE                ((HANDLE)NULL)
#endif

#ifndef WTS_CURRENT_SESSION
#define WTS_CURRENT_SESSION                      ((DWORD)-1)
#endif

#endif /* RDP_SVC_SERVER_MAIN_H */
