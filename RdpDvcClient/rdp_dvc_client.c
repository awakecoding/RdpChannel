
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/crt.h>
#include <winpr/stream.h>
#include <winpr/cmdline.h>

#include "rdp_dvc_client.h"

int rdp_dvc_on_data_received(IWTSVirtualChannelCallback* pChannelCallback, wStream* s)
{
	int status;
	BYTE* pBuffer;
	UINT32 cbSize;
	rdpDvcChannelCallback* callback = (rdpDvcChannelCallback*) pChannelCallback;

	pBuffer = Stream_Pointer(s);
	cbSize = Stream_GetRemainingLength(s);

	status = callback->channel->Write(callback->channel, cbSize, pBuffer, NULL);

	return status;
}

int rdp_dvc_on_close(IWTSVirtualChannelCallback* pChannelCallback)
{
	rdpDvcChannelCallback* callback = (rdpDvcChannelCallback*) pChannelCallback;

	free(callback);

	return 0;
}

int rdp_dvc_on_new_channel_connection(IWTSListenerCallback* pListenerCallback,
	IWTSVirtualChannel* pChannel, BYTE* Data, int* pbAccept,
	IWTSVirtualChannelCallback** ppCallback)
{
	rdpDvcChannelCallback* channelCallback;
	rdpDvcListenerCallback* listenerCallback = (rdpDvcListenerCallback*) pListenerCallback;

	channelCallback = (rdpDvcChannelCallback*) calloc(1, sizeof(rdpDvcChannelCallback));

	if (!channelCallback)
		return -1;

	channelCallback->iface.OnDataReceived = rdp_dvc_on_data_received;
	channelCallback->iface.OnClose = rdp_dvc_on_close;
	channelCallback->plugin = listenerCallback->plugin;
	channelCallback->channelManager = listenerCallback->channelManager;
	channelCallback->channel = pChannel;

	*ppCallback = (IWTSVirtualChannelCallback*) channelCallback;

	return 0;
}

int rdp_dvc_initialize(IWTSPlugin* pPlugin, IWTSVirtualChannelManager* pChannelMgr)
{
	rdpDvcAddin* dvc = (rdpDvcAddin*) pPlugin;

	dvc->listenerCallback = (rdpDvcListenerCallback*) calloc(1, sizeof(rdpDvcListenerCallback));

	if (!dvc->listenerCallback)
		return -1;

	dvc->listenerCallback->iface.OnNewChannelConnection = rdp_dvc_on_new_channel_connection;
	dvc->listenerCallback->plugin = pPlugin;
	dvc->listenerCallback->channelManager = pChannelMgr;

	return pChannelMgr->CreateListener(pChannelMgr, "RdpDvc", 0,
		(IWTSListenerCallback*) dvc->listenerCallback, NULL);
}

int rdp_dvc_terminated(IWTSPlugin* pPlugin)
{
	rdpDvcAddin* dvc = (rdpDvcAddin*) pPlugin;

	if (dvc)
	{
		free(dvc);
	}

	return 0;
}

int DVCPluginEntry(IDRDYNVC_ENTRY_POINTS* pEntryPoints)
{
	int status = 0;
	rdpDvcAddin* dvc;

	dvc = (rdpDvcAddin*) pEntryPoints->GetPlugin(pEntryPoints, "RdpDvc");

	if (!dvc)
	{
		dvc = (rdpDvcAddin*) calloc(1, sizeof(rdpDvcAddin));

		if (!dvc)
			return -1;

		dvc->iface.Initialize = rdp_dvc_initialize;
		dvc->iface.Connected = NULL;
		dvc->iface.Disconnected = NULL;
		dvc->iface.Terminated = rdp_dvc_terminated;

		status = pEntryPoints->RegisterPlugin(pEntryPoints, "RdpDvc", (IWTSPlugin*) dvc);
	}

	return status;
}
