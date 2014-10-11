
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rdp_dvc_com.h"

#ifndef WITH_DVC_API

const IID IID_IUnknown = { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

const IID IID_IWTSPlugin = { 0xA1230201, 0x1439, 0x4e62, { 0xa4, 0x14, 0x19, 0x0d, 0x0a, 0xc3, 0xd4, 0x0e } };
const IID IID_IWTSListener = { 0xA1230206, 0x9a39, 0x4d58, { 0x86, 0x74, 0xcd, 0xb4, 0xdf, 0xf4, 0xe7, 0x3b } };
const IID IID_IWTSListenerCallback = { 0xA1230203, 0xd6a7, 0x11d8, { 0xb9, 0xfd, 0x00, 0x0b, 0xdb, 0xd1, 0xf1, 0x98 } };
const IID IID_IWTSVirtualChannelCallback = { 0xA1230204, 0xd6a7, 0x11d8, { 0xb9, 0xfd, 0x00, 0x0b, 0xdb, 0xd1, 0xf1, 0x98 } };
const IID IID_IWTSVirtualChannelManager = { 0xA1230205, 0xd6a7, 0x11d8, { 0xb9, 0xfd, 0x00, 0x0b, 0xdb, 0xd1, 0xf1, 0x98 } };
const IID IID_IWTSVirtualChannel = { 0xA1230207, 0xd6a7, 0x11d8, { 0xb9, 0xfd, 0x00, 0x0b, 0xdb, 0xd1, 0xf1, 0x98 } };

#endif

static ULONG WinPR_IWTSPlugin_RefCount = 0;

static HRESULT STDMETHODCALLTYPE WinPR_IWTSPlugin_QueryInterface( 
            __RPC__in IWTSPlugin * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject)
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

ULONG STDMETHODCALLTYPE WinPR_IWTSPlugin_AddRef( 
            __RPC__in IWTSPlugin * This)
{
	WinPR_IWTSPlugin_RefCount++;
	return WinPR_IWTSPlugin_RefCount;
}
        
ULONG STDMETHODCALLTYPE WinPR_IWTSPlugin_Release( 
            __RPC__in IWTSPlugin * This)
{
	if (!WinPR_IWTSPlugin_RefCount)
		return 0;

	WinPR_IWTSPlugin_RefCount--;

	return WinPR_IWTSPlugin_RefCount;
}
        
HRESULT STDMETHODCALLTYPE WinPR_IWTSPlugin_Initialize( 
            __RPC__in IWTSPlugin * This,
            /* [in] */ __RPC__in_opt IWTSVirtualChannelManager *pChannelMgr)
{
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE WinPR_IWTSPlugin_Connected( 
            __RPC__in IWTSPlugin * This)
{
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE WinPR_IWTSPlugin_Disconnected( 
            __RPC__in IWTSPlugin * This,
            DWORD dwDisconnectCode)
{
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE WinPR_IWTSPlugin_Terminated( 
            __RPC__in IWTSPlugin * This)
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

HRESULT WINAPI DllCanUnloadNow(void)
{
	return S_OK;
}

HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return S_OK;
}

HRESULT WINAPI DllRegisterServer(void)
{
	return S_OK;
}

HRESULT WINAPI DllUnregisterServer(void)
{
	return S_OK;
}
