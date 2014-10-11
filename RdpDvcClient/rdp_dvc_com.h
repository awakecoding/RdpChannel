
#ifndef RDP_DVC_CLIENT_COM_H
#define RDP_DVC_CLIENT_COM_H

#include <winpr/crt.h>
#include <winpr/spec.h>
#include <winpr/windows.h>

#ifdef _WIN32
//#define WITH_DVC_API
#endif

#ifdef WITH_DVC_API

#include <tsvirtualchannels.h>

#else

#ifndef _WIN32

#include <winpr/error.h>

typedef USHORT VARTYPE;
typedef BOOL VARIANT_BOOL;
typedef BOOL _VARIANT_BOOL;

typedef double DATE;

typedef WCHAR OLECHAR;
typedef OLECHAR LPOLESTR;
typedef const OLECHAR* LPCOLESTR;

#define REFCLSID const IID * const

#define InlineIsEqualGUID(rguid1, rguid2) \
	(((ULONG*) rguid1)[0] == ((ULONG*) rguid2)[0] && \
	((ULONG*) rguid1)[1] == ((ULONG*) rguid2)[1] && \
	((ULONG*) rguid1)[2] == ((ULONG*) rguid2)[2] && \
	((ULONG*) rguid1)[3] == ((ULONG*) rguid2)[3])

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))

#define IsEqualIID(riid1, riid2)	IsEqualGUID(riid1, riid2)
#define IsEqualCLSID(rclsid1, rclsid2)	IsEqualGUID(rclsid1, rclsid2)

typedef union tagCY
{
	struct
	{
		ULONG Lo;
		LONG Hi;
	};
	LONGLONG int64;
} CY;

#define STDMETHODCALLTYPE

#define __RPC__in
#define __RPC__out
#define __RPC__in_opt
#define __RPC__in_range(_x, _y)
#define __RPC__in_string
#define __RPC__deref_out
#define __RPC__deref_out_opt
#define __RPC__in_ecount_full(_x)
#define __RPC__out_ecount_full(_x)

#endif

typedef void* ITypeInfo;
typedef void* IRecordInfo;

typedef struct _IUnknown IUnknown;
typedef struct _IUnknownVtbl IUnknownVtbl;

typedef struct _IClassFactory IClassFactory;
typedef struct _IClassFactoryVtbl IClassFactoryVtbl;

typedef struct _IDispatch IDispatch;
typedef struct _IDispatchVtbl IDispatchVtbl;

typedef struct _IErrorLog IErrorLog;
typedef struct _IErrorLogVtbl IErrorLogVtbl;

typedef struct _IPropertyBag IPropertyBag;
typedef struct _IPropertyBagVtbl IPropertyBagVtbl;

typedef LONG DISPID;
typedef DISPID MEMBERID;

typedef struct tagSAFEARRAYBOUND
{
	ULONG cElements;
	LONG lLbound;
} 	SAFEARRAYBOUND;

typedef struct tagSAFEARRAY
{
	USHORT cDims;
	USHORT fFeatures;
	ULONG cbElements;
	ULONG cLocks;
	PVOID pvData;
	SAFEARRAYBOUND rgsabound[1];
} SAFEARRAY;

typedef struct tagEXCEPINFO
{
	WORD  wCode;
	WORD  wReserved;
	BSTR  bstrSource;
	BSTR  bstrDescription;
	BSTR  bstrHelpFile;
	DWORD dwHelpContext;
	PVOID pvReserved;
	HRESULT (WINAPI *pfnDeferredFillIn)(struct tagEXCEPINFO*);
	SCODE scode;
} EXCEPINFO, *LPEXCEPINFO;

typedef struct tagVARIANT VARIANT;

struct tagVARIANT
{
	union
	{
		struct
		{
			VARTYPE vt;
			WORD wReserved1;
			WORD wReserved2;
			WORD wReserved3;
			union
			{
				LONGLONG llVal;
				LONG lVal;
				BYTE bVal;
				SHORT iVal;
				FLOAT fltVal;
				DOUBLE dblVal;
				VARIANT_BOOL boolVal;
				_VARIANT_BOOL bool;
				SCODE scode;
				CY cyVal;
				DATE date;
				BSTR bstrVal;
				IUnknown *punkVal;
				IDispatch *pdispVal;
				SAFEARRAY *parray;
				BYTE *pbVal;
				SHORT *piVal;
				LONG *plVal;
				LONGLONG *pllVal;
				FLOAT *pfltVal;
				DOUBLE *pdblVal;
				VARIANT_BOOL *pboolVal;
				_VARIANT_BOOL *pbool;
				SCODE *pscode;
				CY *pcyVal;
				DATE *pdate;
				BSTR *pbstrVal;
				IUnknown **ppunkVal;
				IDispatch **ppdispVal;
				SAFEARRAY **pparray;
				VARIANT *pvarVal;
				PVOID byref;
				CHAR cVal;
				USHORT uiVal;
				ULONG ulVal;
				ULONGLONG ullVal;
				INT intVal;
				UINT uintVal;
				DECIMAL *pdecVal;
				CHAR *pcVal;
				USHORT *puiVal;
				ULONG *pulVal;
				ULONGLONG *pullVal;
				INT *pintVal;
				UINT *puintVal;
				struct
				{
					PVOID pvRecord;
					IRecordInfo *pRecInfo;
				};
			};
		};
		DECIMAL decVal;
	};
};

typedef VARIANT *LPVARIANT;
typedef VARIANT VARIANTARG;
typedef VARIANT *LPVARIANTARG;

typedef struct tagDISPPARAMS
{
	VARIANTARG *rgvarg;
	DISPID *rgdispidNamedArgs;
	UINT cArgs;
	UINT cNamedArgs;
} 	DISPPARAMS;

/**
 * IUnknown
 */

struct _IUnknownVtbl
{
	HRESULT (WINAPI * QueryInterface)(IUnknown* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IUnknown* This);

	ULONG (WINAPI * Release)(IUnknown* This);
};

struct _IUnknown
{
	IUnknownVtbl* lpVtbl;
};

/**
 * IClassFactory
 */

struct _IClassFactoryVtbl
{        
	HRESULT (WINAPI * QueryInterface)(IClassFactory* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IClassFactory* This);

	ULONG (WINAPI * Release)(IClassFactory* This);

	HRESULT (WINAPI * CreateInstance)(IClassFactory* This, IUnknown* pUnkOuter, REFIID riid, void** ppvObject);

	HRESULT (WINAPI * LockServer)(IClassFactory* This, BOOL fLock);
};

struct _IClassFactory
{
	IClassFactoryVtbl* lpVtbl;
};

/**
 * IDispatch
 */

struct _IDispatchVtbl
{
	HRESULT (WINAPI * QueryInterface)(IDispatch* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IDispatch* This);

	ULONG (WINAPI * Release)(IDispatch* This);

	HRESULT (WINAPI * GetTypeInfoCount)(IDispatch* This, UINT *pctinfo);

	HRESULT (WINAPI * GetTypeInfo)(IDispatch* This,
			UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);

	HRESULT (WINAPI * GetIDsOfNames)(IDispatch* This,
			REFIID riid, LPOLESTR *rgszNames,
			UINT cNames, LCID lcid, DISPID *rgDispId);

	HRESULT (WINAPI * Invoke)(IDispatch* This,
			DISPID dispIdMember, REFIID riid, LCID lcid,
			WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult,
			EXCEPINFO *pExcepInfo, UINT *puArgErr);
};

struct _IDispatch
{
	IDispatchVtbl* lpVtbl;
};

/**
 * IErrorLog
 */

struct _IErrorLogVtbl
{
	HRESULT (WINAPI * QueryInterface)(IErrorLog* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IErrorLog* This);

	ULONG (WINAPI * Release)(IErrorLog* This);

	HRESULT (WINAPI * AddError)(IErrorLog* This, LPCOLESTR pszPropName, EXCEPINFO *pExcepInfo);
};

struct _IErrorLog
{
	IErrorLogVtbl* lpVtbl;
};

/**
 * IPropertyBag
 */

struct _IPropertyBagVtbl
{
	HRESULT (WINAPI * QueryInterface)(IPropertyBag* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IPropertyBag* This);

	ULONG (WINAPI * Release)(IPropertyBag* This);

	HRESULT (WINAPI * Read)(IPropertyBag* This,
			LPCOLESTR pszPropName, VARIANT *pVar, IErrorLog *pErrorLog);

	HRESULT (WINAPI * Write)(IPropertyBag* This, LPCOLESTR pszPropName, VARIANT *pVar);
};

struct _IPropertyBag
{
	IPropertyBagVtbl* lpVtbl;
};

/**
 * Dynamic Virtual Channel COM API
 */

typedef struct _IWTSPlugin IWTSPlugin;
typedef struct _IWTSPluginVtbl IWTSPluginVtbl;

typedef struct _IWTSListener IWTSListener;
typedef struct _IWTSListenerVtbl IWTSListenerVtbl;

typedef struct _IWTSListenerCallback IWTSListenerCallback;
typedef struct _IWTSListenerCallbackVtbl IWTSListenerCallbackVtbl;

typedef struct _IWTSVirtualChannelCallback IWTSVirtualChannelCallback;
typedef struct _IWTSVirtualChannelCallbackVtbl IWTSVirtualChannelCallbackVtbl;

typedef struct _IWTSVirtualChannelManager IWTSVirtualChannelManager;
typedef struct _IWTSVirtualChannelManagerVtbl IWTSVirtualChannelManagerVtbl;

typedef struct _IWTSVirtualChannel IWTSVirtualChannel;
typedef struct _IWTSVirtualChannelVtbl IWTSVirtualChannelVtbl;

/**
 * IWTSPlugin
 */

struct _IWTSPluginVtbl
{
	HRESULT (WINAPI * QueryInterface)(IWTSPlugin* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IWTSPlugin* This);

	ULONG (WINAPI * Release)(IWTSPlugin* This);

	HRESULT (WINAPI * Initialize)(IWTSPlugin* This, IWTSVirtualChannelManager* pChannelMgr);

	HRESULT (WINAPI * Connected)(IWTSPlugin* This);

	HRESULT (WINAPI * Disconnected)(IWTSPlugin* This, DWORD dwDisconnectCode);

	HRESULT (WINAPI * Terminated)(IWTSPlugin* This);
};

struct _IWTSPlugin
{
	IWTSPluginVtbl* lpVtbl;
};

/**
 * IWTSListener
 */

struct _IWTSListenerVtbl
{
	HRESULT (WINAPI * QueryInterface)(IWTSListener* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IWTSListener* This);

	ULONG (WINAPI * Release)(IWTSListener* This);

	HRESULT (WINAPI * GetConfiguration)(IWTSListener* This, IPropertyBag **ppPropertyBag);
};

struct _IWTSListener
{
	IWTSListenerVtbl* lpVtbl;
};

/**
 * IWTSListenerCallback
 */

struct _IWTSListenerCallbackVtbl
{
	HRESULT (WINAPI * QueryInterface)(IWTSListenerCallback* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IWTSListenerCallback* This);

	ULONG (WINAPI * Release)(IWTSListenerCallback* This);

	HRESULT (WINAPI * OnNewChannelConnection)(IWTSListenerCallback* This,
			IWTSVirtualChannel *pChannel, BSTR data, BOOL *pbAccept,
			IWTSVirtualChannelCallback **ppCallback);
};

struct _IWTSListenerCallback
{
	IWTSListenerCallbackVtbl* lpVtbl;
};

/**
 * IWTSVirtualChannelCallback
 */

struct _IWTSVirtualChannelCallbackVtbl
{
	HRESULT (WINAPI * QueryInterface)(IWTSVirtualChannelCallback* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IWTSVirtualChannelCallback* This);

	ULONG (WINAPI * Release)(IWTSVirtualChannelCallback* This);

	HRESULT (WINAPI * OnDataReceived)(IWTSVirtualChannelCallback* This, ULONG cbSize, BYTE *pBuffer);

	HRESULT (WINAPI * OnClose)(IWTSVirtualChannelCallback* This);
};

struct _IWTSVirtualChannelCallback
{
	IWTSVirtualChannelCallbackVtbl* lpVtbl;
};

/**
 * IWTSVirtualChannelManager
 */

struct _IWTSVirtualChannelManagerVtbl
{
	HRESULT (WINAPI * QueryInterface)(IWTSVirtualChannelManager* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IWTSVirtualChannelManager* This);

	ULONG (WINAPI * Release)(IWTSVirtualChannelManager* This);

	HRESULT (WINAPI * CreateListener)(IWTSVirtualChannelManager* This,
			const char *pszChannelName, ULONG uFlags,
			IWTSListenerCallback *pListenerCallback, IWTSListener **ppListener);
};

struct _IWTSVirtualChannelManager
{
	IWTSVirtualChannelManagerVtbl* lpVtbl;
};

/**
 * IWTSVirtualChannel
 */

struct _IWTSVirtualChannelVtbl
{
	HRESULT (WINAPI * QueryInterface)(IWTSVirtualChannel* This, REFIID riid, void** ppvObject);

	ULONG (WINAPI * AddRef)(IWTSVirtualChannel* This);

	ULONG (WINAPI * Release)(IWTSVirtualChannel* This);

	HRESULT (WINAPI * Write)(IWTSVirtualChannel* This,
			ULONG cbSize, BYTE *pBuffer, IUnknown *pReserved);

	HRESULT (WINAPI * Close)(IWTSVirtualChannel* This);
};

struct _IWTSVirtualChannel
{
	IWTSVirtualChannelVtbl* lpVtbl;
};

#endif

#endif /* RDP_DVC_CLIENT_COM_H */
