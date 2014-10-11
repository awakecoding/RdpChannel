
#ifndef RDP_DVC_CLIENT_MAIN_H
#define RDP_DVC_CLIENT_MAIN_H

#include <winpr/crt.h>
#include <winpr/spec.h>

#include <freerdp/dvc.h>
#include <freerdp/addin.h>

struct rdp_dvc_listener_callback
{
	IWTSListenerCallback iface;

	IWTSPlugin* plugin;
	IWTSVirtualChannelManager* channelManager;
};
typedef struct rdp_dvc_listener_callback rdpDvcListenerCallback;

struct rdp_dvc_channel_callback
{
	IWTSVirtualChannelCallback iface;

	IWTSPlugin* plugin;
	IWTSVirtualChannelManager* channelManager;
	IWTSVirtualChannel* channel;
};
typedef struct rdp_dvc_channel_callback rdpDvcChannelCallback;

struct rdp_dvc_addin
{
	IWTSPlugin iface;

	rdpDvcListenerCallback* listenerCallback;
};
typedef struct rdp_dvc_addin rdpDvcAddin;

#endif /* RDP_DVC_CLIENT_MAIN_H */

