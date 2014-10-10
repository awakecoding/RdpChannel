
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/crt.h>
#include <winpr/wlog.h>
#include <winpr/stream.h>
#include <winpr/collections.h>

#include "rdp_svc_client.h"

static wLog* g_Log = NULL;

static wListDictionary* g_InitHandles = NULL;
static wListDictionary* g_OpenHandles = NULL;

void rdp_svc_add_init_handle_data(void* pInitHandle, void* pUserData)
{
	if (!g_InitHandles)
		g_InitHandles = ListDictionary_New(TRUE);

	ListDictionary_Add(g_InitHandles, pInitHandle, pUserData);
}

void* rdp_svc_get_init_handle_data(void* pInitHandle)
{
	void* pUserData = NULL;
	pUserData = ListDictionary_GetItemValue(g_InitHandles, pInitHandle);
	return pUserData;
}

void rdp_svc_remove_init_handle_data(void* pInitHandle)
{
	ListDictionary_Remove(g_InitHandles, pInitHandle);
}

void rdp_svc_add_open_handle_data(DWORD openHandle, void* pUserData)
{
	void* pOpenHandle = (void*) (size_t) openHandle;

	if (!g_OpenHandles)
		g_OpenHandles = ListDictionary_New(TRUE);

	ListDictionary_Add(g_OpenHandles, pOpenHandle, pUserData);
}

void* rdp_svc_get_open_handle_data(DWORD openHandle)
{
	void* pUserData = NULL;
	void* pOpenHandle = (void*) (size_t) openHandle;
	pUserData = ListDictionary_GetItemValue(g_OpenHandles, pOpenHandle);
	return pUserData;
}

void rdp_svc_remove_open_handle_data(DWORD openHandle)
{
	void* pOpenHandle = (void*) (size_t) openHandle;
	ListDictionary_Remove(g_OpenHandles, pOpenHandle);
}

int rdp_svc_send(rdpSvcAddin* svc, wStream* s)
{
	UINT32 status = 0;

	if (!svc)
		return CHANNEL_RC_BAD_INIT_HANDLE;

	status = svc->channelEntryPoints.pVirtualChannelWrite(svc->OpenHandle,
			Stream_Buffer(s), Stream_Length(s), s);

	if (status != CHANNEL_RC_OK)
	{
		Stream_Free(s, TRUE);
		fprintf(stderr, "rdp_svc_send: VirtualChannelWrite failed %d\n", status);
	}

	return status;
}

static void rdp_svc_virtual_channel_event_data_received(rdpSvcAddin* svc,
		void* pData, UINT32 dataLength, UINT32 totalLength, UINT32 dataFlags)
{
	wStream* s;

	WLog_Print(g_Log, WLOG_DEBUG, "RdpSvcVirtualChannelEventDataReceived: dataLength: %d totalLength: %d",
			dataLength, totalLength);

	if ((dataFlags & CHANNEL_FLAG_SUSPEND) || (dataFlags & CHANNEL_FLAG_RESUME))
	{
		return;
	}

	if (dataFlags & CHANNEL_FLAG_FIRST)
	{
		if (svc->input)
			Stream_Free(svc->input, TRUE);

		svc->input = Stream_New(NULL, totalLength);
	}

	s = svc->input;
	Stream_EnsureRemainingCapacity(s, (int) dataLength);
	Stream_Write(s, pData, dataLength);

	if (dataFlags & CHANNEL_FLAG_LAST)
	{
		if (Stream_Capacity(s) != Stream_GetPosition(s))
		{
			fprintf(stderr, "rdp_svc_virtual_channel_event_data_received: read error\n");
		}

		svc->input = NULL;
		Stream_SealLength(s);
		Stream_SetPosition(s, 0);

		MessageQueue_Post(svc->MsgPipe->In, NULL, 0, (void*) s, NULL);
	}
}

static void rdp_svc_virtual_channel_event_write_complete(rdpSvcAddin* svc,
		void* pData, UINT32 dataLength, UINT32 totalLength, UINT32 dataFlags)
{
	wStream* s;

	s = (wStream*) pData;

	WLog_Print(g_Log, WLOG_DEBUG, "RdpSvcVirtualChannelEventWriteComplete");

	Stream_Free((wStream*) pData, TRUE);
}

static void rdp_svc_virtual_channel_open_event(UINT32 openHandle, UINT32 event,
		void* pData, UINT32 dataLength, UINT32 totalLength, UINT32 dataFlags)
{
	rdpSvcAddin* svc;

	WLog_Print(g_Log, WLOG_DEBUG, "RdpSvcVirtualChannelOpenEvent");

	svc = (rdpSvcAddin*) rdp_svc_get_open_handle_data(openHandle);

	if (!svc)
	{
		fprintf(stderr, "rdp_svc_virtual_channel_open_event: error no match\n");
		return;
	}

	switch (event)
	{
		case CHANNEL_EVENT_DATA_RECEIVED:
			rdp_svc_virtual_channel_event_data_received(svc, pData, dataLength, totalLength, dataFlags);
			break;

		case CHANNEL_EVENT_WRITE_COMPLETE:
			rdp_svc_virtual_channel_event_write_complete(svc, pData, dataLength, totalLength, dataFlags);
			break;

		case CHANNEL_EVENT_USER:
			break;
	}
}

int rdp_svc_process_receive(rdpSvcAddin* svc, wStream* s)
{
	wStream* output;

	output = Stream_New(NULL, Stream_Length(s));
	Stream_Write(output, Stream_Buffer(s), Stream_Length(s));
	Stream_SealLength(output);
	Stream_SetPosition(output, 0);

	rdp_svc_send(svc, output);

	return 0;
}

static void* rdp_svc_virtual_channel_client_thread(void* arg)
{
	wStream* s;
	wMessage message;
	rdpSvcAddin* svc = (rdpSvcAddin*) arg;

	while (1)
	{
		if (!MessageQueue_Wait(svc->MsgPipe->In))
			break;

		if (MessageQueue_Peek(svc->MsgPipe->In, &message, TRUE))
		{
			if (message.id == WMQ_QUIT)
				break;

			if (message.id == 0)
			{
				s = (wStream*) message.wParam;
				rdp_svc_process_receive(svc, s);
			}
		}
	}

	ExitThread(0);
	return NULL;
}

static void rdp_svc_virtual_channel_event_connected(rdpSvcAddin* svc, void* pData, UINT32 dataLength)
{
	UINT32 status;

	WLog_Print(g_Log, WLOG_DEBUG, "RdpSvcVirtualChannelEventConnected");

	status = svc->channelEntryPoints.pVirtualChannelOpen(svc->InitHandle,
		&svc->OpenHandle, svc->channelDef.name, rdp_svc_virtual_channel_open_event);

	rdp_svc_add_open_handle_data(svc->OpenHandle, svc);

	if (status != CHANNEL_RC_OK)
	{
		fprintf(stderr, "rdp_svc_virtual_channel_event_connected: open failed: status: %d\n", status);
		return;
	}

	svc->MsgPipe = MessagePipe_New();

	svc->thread = CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE) rdp_svc_virtual_channel_client_thread, (void*) svc, 0, NULL);
}

static void rdp_svc_virtual_channel_event_disconnected(rdpSvcAddin* svc)
{
	WLog_Print(g_Log, WLOG_DEBUG, "RdpSvcVirtualChannelEventDisconnected");
}

static void rdp_svc_virtual_channel_event_terminated(rdpSvcAddin* svc)
{
	WLog_Print(g_Log, WLOG_DEBUG, "RdpSvcVirtualChannelEventTerminated");

	MessagePipe_PostQuit(svc->MsgPipe, 0);
	WaitForSingleObject(svc->thread, INFINITE);

	MessagePipe_Free(svc->MsgPipe);
	CloseHandle(svc->thread);

	if (svc->input)
		Stream_Free(svc->input, TRUE);

	svc->channelEntryPoints.pVirtualChannelClose(svc->OpenHandle);

	rdp_svc_remove_open_handle_data(svc->OpenHandle);
	rdp_svc_remove_init_handle_data(svc->InitHandle);
}

static void rdp_svc_virtual_channel_init_event(void* pInitHandle, UINT32 event, void* pData, UINT32 dataLength)
{
	rdpSvcAddin* svc;

	WLog_Print(g_Log, WLOG_DEBUG, "RdpSvcVirtualChannelInitEvent");

	svc = (rdpSvcAddin*) rdp_svc_get_init_handle_data(pInitHandle);

	if (!svc)
	{
		fprintf(stderr, "rdp_svc_virtual_channel_init_event: error\n");
		return;
	}

	switch (event)
	{
		case CHANNEL_EVENT_CONNECTED:
			fprintf(stderr, "svc channel client: CHANNEL_EVENT_CONNECTED\n");
			rdp_svc_virtual_channel_event_connected(svc, pData, dataLength);
			break;

		case CHANNEL_EVENT_DISCONNECTED:
			fprintf(stderr, "svc channel client: CHANNEL_EVENT_DISCONNECTED\n");
			rdp_svc_virtual_channel_event_disconnected(svc);
			break;

		case CHANNEL_EVENT_TERMINATED:
			fprintf(stderr, "svc channel client: CHANNEL_EVENT_TERMINATED\n");
			rdp_svc_virtual_channel_event_terminated(svc);
			break;
	}
}

int VirtualChannelEntry(PCHANNEL_ENTRY_POINTS pEntryPoints)
{
	rdpSvcAddin* svc;

	svc = (rdpSvcAddin*) calloc(1, sizeof(rdpSvcAddin));

	if (!svc)
		return -1;

#ifdef _WIN32
	MessageBoxA(NULL, "RdpSvcClient", "RdpSvcClient", 0);
#endif

	fprintf(stderr, "loading RdpSvc channel client\n");

	svc->channelDef.options =
			CHANNEL_OPTION_INITIALIZED |
			CHANNEL_OPTION_ENCRYPT_RDP |
			CHANNEL_OPTION_COMPRESS_RDP;

	strcpy(svc->channelDef.name, "RdpSvc");

	CopyMemory(&(svc->channelEntryPoints), pEntryPoints, sizeof(svc->channelEntryPoints));

	svc->channelEntryPoints.pVirtualChannelInit(&svc->InitHandle,
		&svc->channelDef, 1, VIRTUAL_CHANNEL_VERSION_WIN2000, rdp_svc_virtual_channel_init_event);

	rdp_svc_add_init_handle_data(svc->InitHandle, (void*) svc);

	g_Log = WLog_Get("rdp.svc.client");

#ifdef _WIN32
	WLog_SetLogAppenderType(g_Log, WLOG_APPENDER_FILE);
	WLog_OpenAppender(g_Log);
#endif

	WLog_SetLogLevel(g_Log, WLOG_DEBUG);

	WLog_Print(g_Log, WLOG_DEBUG, "VirtualChannelEntry");

	return 1;
}

