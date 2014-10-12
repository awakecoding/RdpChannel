
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rdp_dvc_com.h"

static wLog* g_Log = NULL;

#ifndef WITH_DVC_API

const IID IID_IUnknown = { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
const IID IID_IClassFactory = { 0x00000001, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

const IID IID_IWTSPlugin = { 0xA1230201, 0x1439, 0x4e62, { 0xa4, 0x14, 0x19, 0x0d, 0x0a, 0xc3, 0xd4, 0x0e } };
const IID IID_IWTSListener = { 0xA1230206, 0x9a39, 0x4d58, { 0x86, 0x74, 0xcd, 0xb4, 0xdf, 0xf4, 0xe7, 0x3b } };
const IID IID_IWTSListenerCallback = { 0xA1230203, 0xd6a7, 0x11d8, { 0xb9, 0xfd, 0x00, 0x0b, 0xdb, 0xd1, 0xf1, 0x98 } };
const IID IID_IWTSVirtualChannelCallback = { 0xA1230204, 0xd6a7, 0x11d8, { 0xb9, 0xfd, 0x00, 0x0b, 0xdb, 0xd1, 0xf1, 0x98 } };
const IID IID_IWTSVirtualChannelManager = { 0xA1230205, 0xd6a7, 0x11d8, { 0xb9, 0xfd, 0x00, 0x0b, 0xdb, 0xd1, 0xf1, 0x98 } };
const IID IID_IWTSVirtualChannel = { 0xA1230207, 0xd6a7, 0x11d8, { 0xb9, 0xfd, 0x00, 0x0b, 0xdb, 0xd1, 0xf1, 0x98 } };

#endif

static IClassFactory WinPR_IClassFactory;
static HRESULT WINAPI WinPR_IClassFactory_CreateInstance(IClassFactory* This, IUnknown* pUnkOuter, REFIID riid, void** ppvObject);

static IWTSVirtualChannel* g_Channel = NULL;

/**
 * IWTSPlugin
 */

static ULONG WinPR_IWTSPlugin_RefCount = 0;

static HRESULT WINAPI WinPR_IWTSPlugin_QueryInterface(IWTSPlugin* This, REFIID riid, void** ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, &IID_IWTSPlugin) || IsEqualIID(riid, &IID_IUnknown))
	{
		*ppvObject = This;
	}

	if (!(*ppvObject))
		return E_NOINTERFACE;

	This->lpVtbl->AddRef(This);

	return S_OK;
}

static ULONG WINAPI WinPR_IWTSPlugin_AddRef(IWTSPlugin* This)
{
	WinPR_IWTSPlugin_RefCount++;
	return WinPR_IWTSPlugin_RefCount;
}

static ULONG WINAPI WinPR_IWTSPlugin_Release(IWTSPlugin* This)
{
	if (!WinPR_IWTSPlugin_RefCount)
		return 0;

	WinPR_IWTSPlugin_RefCount--;

	return WinPR_IWTSPlugin_RefCount;
}

static HRESULT WINAPI WinPR_IWTSPlugin_Initialize(IWTSPlugin* This, IWTSVirtualChannelManager* pChannelMgr)
{
	HRESULT hr;
	IWTSListener* pListener = NULL;
	IWTSListenerCallback* pListenerCallback = NULL;

	WLog_Print(g_Log, WLOG_DEBUG, "IWTSPlugin::Initialize");

	hr = WinPR_IClassFactory_CreateInstance(&WinPR_IClassFactory, NULL,
		(REFIID) &IID_IWTSListenerCallback, (void**) &pListenerCallback);

	if (FAILED(hr))
	{
		WLog_Print(g_Log, WLOG_DEBUG, "IClassFactory::CreateInstance(IWTSListener) failed: 0x%04X", hr);
		return hr;
	}

	hr = pChannelMgr->lpVtbl->CreateListener(pChannelMgr, "RdpDvc", 0, pListenerCallback, &pListener);

	if (FAILED(hr))
	{
		WLog_Print(g_Log, WLOG_DEBUG, "IWTSVirtualChannelManager::CreateListener failed: 0x%04X", hr);
		return hr;
	}

	return S_OK;
}
        
static HRESULT WINAPI WinPR_IWTSPlugin_Connected(IWTSPlugin* This)
{
	WLog_Print(g_Log, WLOG_DEBUG, "IWTSPlugin::Connected");
	return S_OK;
}
        
static HRESULT WINAPI WinPR_IWTSPlugin_Disconnected(IWTSPlugin* This, DWORD dwDisconnectCode)
{
	WLog_Print(g_Log, WLOG_DEBUG, "IWTSPlugin::Disconnected");
	return S_OK;
}
        
static HRESULT WINAPI WinPR_IWTSPlugin_Terminated(IWTSPlugin* This)
{
	WLog_Print(g_Log, WLOG_DEBUG, "IWTSPlugin::Terminated");
	return S_OK;
}

static IWTSPluginVtbl WinPR_IWTSPluginVtbl =
{
	/* IUnknown */
	WinPR_IWTSPlugin_QueryInterface,
	WinPR_IWTSPlugin_AddRef,
	WinPR_IWTSPlugin_Release,

	/* IWTSPlugin */
	WinPR_IWTSPlugin_Initialize,
	WinPR_IWTSPlugin_Connected,
	WinPR_IWTSPlugin_Disconnected,
	WinPR_IWTSPlugin_Terminated
};

static IWTSPlugin WinPR_IWTSPlugin =
{
	&WinPR_IWTSPluginVtbl
};

/**
 * IWTSListener
 */

static ULONG WinPR_IWTSListener_RefCount = 0;

static HRESULT WINAPI WinPR_IWTSListener_QueryInterface(IWTSListener* This, REFIID riid, void** ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, &IID_IWTSListener) || IsEqualIID(riid, &IID_IUnknown))
	{
		*ppvObject = This;
	}

	if (!(*ppvObject))
		return E_NOINTERFACE;

	This->lpVtbl->AddRef(This);

	return S_OK;
}

static ULONG WINAPI WinPR_IWTSListener_AddRef(IWTSListener* This)
{
	WinPR_IWTSListener_RefCount++;
	return WinPR_IWTSListener_RefCount;
}

static ULONG WINAPI WinPR_IWTSListener_Release(IWTSListener* This)
{
	if (!WinPR_IWTSListener_RefCount)
		return 0;

	WinPR_IWTSListener_RefCount--;

	return WinPR_IWTSListener_RefCount;
}

static HRESULT WINAPI WinPR_IWTSListener_GetConfiguration(IWTSListener* This, IPropertyBag** ppPropertyBag)
{
	WLog_Print(g_Log, WLOG_DEBUG, "IWTSListener::GetConfiguration");
	return S_OK;
}

static IWTSListenerVtbl WinPR_IWTSListenerVtbl =
{
	/* IUnknown */
	WinPR_IWTSListener_QueryInterface,
	WinPR_IWTSListener_AddRef,
	WinPR_IWTSListener_Release,

	/* IWTSListener */
	WinPR_IWTSListener_GetConfiguration
};

static IWTSListener WinPR_IWTSListener =
{
	&WinPR_IWTSListenerVtbl
};

/**
 * IWTSListenerCallback
 */

static ULONG WinPR_IWTSListenerCallback_RefCount = 0;

static HRESULT WINAPI WinPR_IWTSListenerCallback_QueryInterface(IWTSListenerCallback* This, REFIID riid, void** ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, &IID_IWTSListenerCallback) || IsEqualIID(riid, &IID_IUnknown))
	{
		*ppvObject = This;
	}

	if (!(*ppvObject))
		return E_NOINTERFACE;

	This->lpVtbl->AddRef(This);

	return S_OK;
}

static ULONG WINAPI WinPR_IWTSListenerCallback_AddRef(IWTSListenerCallback* This)
{
	WinPR_IWTSListenerCallback_RefCount++;
	return WinPR_IWTSListenerCallback_RefCount;
}

static ULONG WINAPI WinPR_IWTSListenerCallback_Release(IWTSListenerCallback* This)
{
	if (!WinPR_IWTSListenerCallback_RefCount)
		return 0;

	WinPR_IWTSListenerCallback_RefCount--;

	return WinPR_IWTSListenerCallback_RefCount;
}

static HRESULT WINAPI WinPR_IWTSListenerCallback_OnNewChannelConnection(IWTSListenerCallback* This,
		IWTSVirtualChannel* pChannel, BSTR data, BOOL* pbAccept, IWTSVirtualChannelCallback** ppCallback)
{
	HRESULT hr;
	IWTSVirtualChannelCallback* pCallback = NULL;

	WLog_Print(g_Log, WLOG_DEBUG, "IWTSListenerCallback::OnNewChannelConnection");

	hr = WinPR_IClassFactory_CreateInstance(&WinPR_IClassFactory, NULL,
			(REFIID) &IID_IWTSVirtualChannelCallback, (void**) &pCallback);

	if (FAILED(hr))
		return hr;

	pCallback->lpVtbl->AddRef(pCallback);

	g_Channel = pChannel;

	*ppCallback = pCallback;
	*pbAccept = TRUE;

	return S_OK;
}

static IWTSListenerCallbackVtbl WinPR_IWTSListenerCallbackVtbl =
{
	/* IUnknown */
	WinPR_IWTSListenerCallback_QueryInterface,
	WinPR_IWTSListenerCallback_AddRef,
	WinPR_IWTSListenerCallback_Release,

	/* IWTSListenerCallback */
	WinPR_IWTSListenerCallback_OnNewChannelConnection
};

static IWTSListenerCallback WinPR_IWTSListenerCallback =
{
	&WinPR_IWTSListenerCallbackVtbl
};

/**
 * IWTSVirtualChannelCallback
 */

static ULONG WinPR_IWTSVirtualChannelCallback_RefCount = 0;

static HRESULT WINAPI WinPR_IWTSVirtualChannelCallback_QueryInterface(IWTSVirtualChannelCallback* This, REFIID riid, void** ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, &IID_IWTSVirtualChannelCallback) || IsEqualIID(riid, &IID_IUnknown))
	{
		*ppvObject = This;
	}

	if (!(*ppvObject))
		return E_NOINTERFACE;

	This->lpVtbl->AddRef(This);

	return S_OK;
}

static ULONG WINAPI WinPR_IWTSVirtualChannelCallback_AddRef(IWTSVirtualChannelCallback* This)
{
	WinPR_IWTSVirtualChannelCallback_RefCount++;
	return WinPR_IWTSVirtualChannelCallback_RefCount;
}

static ULONG WINAPI WinPR_IWTSVirtualChannelCallback_Release(IWTSVirtualChannelCallback* This)
{
	if (!WinPR_IWTSVirtualChannelCallback_RefCount)
		return 0;

	WinPR_IWTSVirtualChannelCallback_RefCount--;

	return WinPR_IWTSVirtualChannelCallback_RefCount;
}

static HRESULT WINAPI WinPR_IWTSVirtualChannelCallback_OnDataReceived(IWTSVirtualChannelCallback* This, ULONG cbSize, BYTE* pBuffer)
{
	HRESULT hr;
	IWTSVirtualChannel* pChannel = g_Channel;

	WLog_Print(g_Log, WLOG_DEBUG, "IWTSVirtualChannelCallback::OnDataReceived");

	if (!pChannel)
		return S_FALSE;

	hr = pChannel->lpVtbl->Write(pChannel, cbSize, pBuffer, NULL);

	return hr;
}

static HRESULT WINAPI WinPR_IWTSVirtualChannelCallback_OnClose(IWTSVirtualChannelCallback* This)
{
	HRESULT hr;
	IWTSVirtualChannel* pChannel = g_Channel;

	WLog_Print(g_Log, WLOG_DEBUG, "IWTSVirtualChannelCallback::OnClose");

	if (!pChannel)
		return S_FALSE;

	hr = pChannel->lpVtbl->Close(pChannel);

	return hr;
}

static IWTSVirtualChannelCallbackVtbl WinPR_IWTSVirtualChannelCallbackVtbl =
{
	/* IUnknown */
	WinPR_IWTSVirtualChannelCallback_QueryInterface,
	WinPR_IWTSVirtualChannelCallback_AddRef,
	WinPR_IWTSVirtualChannelCallback_Release,

	/* IWTSVirtualChannelCallback */
	WinPR_IWTSVirtualChannelCallback_OnDataReceived,
	WinPR_IWTSVirtualChannelCallback_OnClose
};

static IWTSVirtualChannelCallback WinPR_IWTSVirtualChannelCallback =
{
	&WinPR_IWTSVirtualChannelCallbackVtbl
};

/**
 * IClassFactory
 */

static ULONG WinPR_IClassFactory_RefCount = 0;

static HRESULT WINAPI WinPR_IClassFactory_QueryInterface(IClassFactory* This, REFIID riid, void** ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, &IID_IClassFactory) || IsEqualIID(riid, &IID_IUnknown))
	{
		*ppvObject = This;
	}

	if (!(*ppvObject))
		return E_NOINTERFACE;

	This->lpVtbl->AddRef(This);

	return S_OK;
}
        
static ULONG WINAPI WinPR_IClassFactory_AddRef(IClassFactory* This)
{
	WinPR_IClassFactory_RefCount++;
	return WinPR_IClassFactory_RefCount;
}
        
static ULONG WINAPI WinPR_IClassFactory_Release(IClassFactory* This)
{
	if (!WinPR_IClassFactory_RefCount)
		return 0;

	WinPR_IClassFactory_RefCount--;

	return WinPR_IClassFactory_RefCount;
}
        
static HRESULT WINAPI WinPR_IClassFactory_CreateInstance(IClassFactory* This, IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
{
	HRESULT hr = S_OK;

	*ppvObject = NULL;

	if (pUnkOuter)
		return CLASS_E_NOAGGREGATION;

	if (IsEqualCLSID(riid, &IID_IWTSPlugin))
	{
		IWTSPlugin* pObj;

		pObj = (IWTSPlugin*) calloc(1, sizeof(IWTSPlugin));

		if (!pObj)
			return E_OUTOFMEMORY;

		CopyMemory(pObj, &WinPR_IWTSPlugin, sizeof(IWTSPlugin));

		hr = pObj->lpVtbl->QueryInterface(pObj, riid, ppvObject);
	}
	else if (IsEqualCLSID(riid, &IID_IWTSListener))
	{
		IWTSListener* pObj;

		pObj = (IWTSListener*) calloc(1, sizeof(IWTSListener));

		if (!pObj)
			return E_OUTOFMEMORY;

		CopyMemory(pObj, &WinPR_IWTSListener, sizeof(IWTSListener));

		hr = pObj->lpVtbl->QueryInterface(pObj, riid, ppvObject);
	}
	else if (IsEqualCLSID(riid, &IID_IWTSListenerCallback))
	{
		IWTSListenerCallback* pObj;

		pObj = (IWTSListenerCallback*) calloc(1, sizeof(IWTSListenerCallback));

		if (!pObj)
			return E_OUTOFMEMORY;

		CopyMemory(pObj, &WinPR_IWTSListenerCallback, sizeof(IWTSListenerCallback));

		hr = pObj->lpVtbl->QueryInterface(pObj, riid, ppvObject);
	}
	else if (IsEqualCLSID(riid, &IID_IWTSVirtualChannelCallback))
	{
		IWTSVirtualChannelCallback* pObj;

		pObj = (IWTSVirtualChannelCallback*) calloc(1, sizeof(IWTSVirtualChannelCallback));

		if (!pObj)
			return E_OUTOFMEMORY;

		CopyMemory(pObj, &WinPR_IWTSVirtualChannelCallback, sizeof(IWTSVirtualChannelCallback));

		hr = pObj->lpVtbl->QueryInterface(pObj, riid, ppvObject);
	}
	else
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	return hr;
}

static HRESULT WINAPI WinPR_IClassFactory_LockServer(IClassFactory* This, BOOL fLock)
{
	return S_OK;
}

static IClassFactoryVtbl WinPR_IClassFactoryVtbl =
{
	/* IUnknown */
	WinPR_IClassFactory_QueryInterface,
	WinPR_IClassFactory_AddRef,
	WinPR_IClassFactory_Release,

	/* IClassFactory */
	WinPR_IClassFactory_CreateInstance,
	WinPR_IClassFactory_LockServer
};

static IClassFactory WinPR_IClassFactory =
{
	&WinPR_IClassFactoryVtbl
};

/**
 * Exported COM Functions
 */

HRESULT WINAPI DllCanUnloadNow(void)
{
	return S_OK;
}

HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	HRESULT hr;

	if (!g_Log)
	{
		g_Log = WLog_Get("rdp.dvc.client");

#ifdef _WIN32
		WLog_SetLogAppenderType(g_Log, WLOG_APPENDER_FILE);
		WLog_OpenAppender(g_Log);
#endif

		WLog_SetLogLevel(g_Log, WLOG_DEBUG);
	}

	WLog_Print(g_Log, WLOG_DEBUG, "DllGetClassObject");

	*ppv = NULL;

	if (!IsEqualCLSID(rclsid, &IID_IWTSPlugin))
		return CLASS_E_CLASSNOTAVAILABLE;

	hr = WinPR_IClassFactory_QueryInterface(&WinPR_IClassFactory, riid, ppv);

	return hr;
}

HRESULT WINAPI DllRegisterServer(void)
{
	return S_OK;
}

HRESULT WINAPI DllUnregisterServer(void)
{
	return S_OK;
}

/**
 * VirtualChannelGetInstance Entry Point
 * http://msdn.microsoft.com/en-us/library/jj662753/
 */

DECLSPEC_EXPORT HRESULT VCAPITYPE VirtualChannelGetInstance(REFIID refiid, ULONG* pNumObjs, VOID** ppObjArray)
{
	if (!g_Log)
	{
		g_Log = WLog_Get("rdp.dvc.client");

#ifdef _WIN32
		WLog_SetLogAppenderType(g_Log, WLOG_APPENDER_FILE);
		WLog_OpenAppender(g_Log);
#endif

		WLog_SetLogLevel(g_Log, WLOG_DEBUG);
	}

	WLog_Print(g_Log, WLOG_DEBUG, "VirtualChannelGetInstance");

	if (!IsEqualCLSID(refiid, &IID_IWTSPlugin))
		return CLASS_E_CLASSNOTAVAILABLE;

	*pNumObjs = 1;

	if (!ppObjArray)
		return S_OK;

	*((ULONG_PTR*) ppObjArray) = (ULONG_PTR) &WinPR_IWTSPlugin;

	return S_OK;
}
