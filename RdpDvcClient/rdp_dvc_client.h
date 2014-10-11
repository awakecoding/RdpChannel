/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Echo Virtual Channel Extension
 *
 * Copyright 2013 Christian Hofstaedtler
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ECHO_MAIN_H
#define __ECHO_MAIN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

#endif /* __ECHO_MAIN_H */

