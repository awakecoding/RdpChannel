
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rdp_dvc_com.h"

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
	return S_OK;
}
        
static HRESULT WINAPI WinPR_IWTSPlugin_Connected(IWTSPlugin* This)
{
	return S_OK;
}
        
static HRESULT WINAPI WinPR_IWTSPlugin_Disconnected(IWTSPlugin* This, DWORD dwDisconnectCode)
{
	return S_OK;
}
        
static HRESULT WINAPI WinPR_IWTSPlugin_Terminated(IWTSPlugin* This)
{
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

IWTSPlugin WinPR_IWTSPlugin =
{
	&WinPR_IWTSPluginVtbl
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

IClassFactory WinPR_IClassFactory =
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
