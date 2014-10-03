#ifndef RDP_SVC_CLIENT_MAIN_H
#define RDP_SVC_CLIENT_MAIN_H

#include <winpr/crt.h>
#include <winpr/spec.h>
#include <winpr/synch.h>
#include <winpr/thread.h>
#include <winpr/stream.h>
#include <winpr/collections.h>

#include <freerdp/api.h>
#include <freerdp/svc.h>
#include <freerdp/addin.h>
#include <freerdp/constants.h>

struct rdp_svc_addin
{
	CHANNEL_DEF channelDef;
	CHANNEL_ENTRY_POINTS channelEntryPoints;

	HANDLE thread;
	wStream* input;
	void* InitHandle;
	UINT32 OpenHandle;
	wMessagePipe* MsgPipe;
};
typedef struct rdp_svc_addin rdpSvcAddin;

DECLSPEC_EXPORT int VirtualChannelEntry(PCHANNEL_ENTRY_POINTS pEntryPoints);

#endif /* RDP_SVC_CLIENT_MAIN_H */

