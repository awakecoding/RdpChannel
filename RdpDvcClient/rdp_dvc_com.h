
#ifndef RDP_DVC_CLIENT_COM_H
#define RDP_DVC_CLIENT_COM_H

#include <winpr/crt.h>
#include <winpr/windows.h>

#ifdef _WIN32
#define WITH_DVC_API
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
    SAFEARRAYBOUND rgsabound[ 1 ];
    } 	SAFEARRAY;

typedef struct tagEXCEPINFO
{
    WORD  wCode;
    WORD  wReserved;
    BSTR  bstrSource;
    BSTR  bstrDescription;
    BSTR  bstrHelpFile;
    DWORD dwHelpContext;
    PVOID pvReserved;
    HRESULT (WINAPI *pfnDeferredFillIn)(struct tagEXCEPINFO *);
    SCODE scode;
} EXCEPINFO, * LPEXCEPINFO;

#define __tagVARIANT
#define __VARIANT_NAME_1
#define __VARIANT_NAME_2
#define __VARIANT_NAME_3
#define __tagBRECORD
#define __VARIANT_NAME_4

typedef struct tagVARIANT VARIANT;

struct tagVARIANT
{
    union 
        {
        struct __tagVARIANT
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
                struct __tagBRECORD
                    {
                    PVOID pvRecord;
                    IRecordInfo *pRecInfo;
                    } 	__VARIANT_NAME_4;
                } 	__VARIANT_NAME_3;
            } 	__VARIANT_NAME_2;
        DECIMAL decVal;
        } 	__VARIANT_NAME_1;
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

    struct _IUnknownVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUnknown * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUnknown * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUnknown * This);
    };

    struct _IUnknown
    {
        IUnknownVtbl *lpVtbl;
    };

    struct _IDispatchVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IDispatch * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IDispatch * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IDispatch * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            __RPC__in IDispatch * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            __RPC__in IDispatch * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            __RPC__in IDispatch * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ __RPC__in_range(0,16384) UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDispatch * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
    };

    struct _IDispatch
    {
        IDispatchVtbl *lpVtbl;
    };

    struct _IErrorLogVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IErrorLog * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IErrorLog * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IErrorLog * This);
        
        HRESULT ( STDMETHODCALLTYPE *AddError )( 
            __RPC__in IErrorLog * This,
            /* [in] */ __RPC__in LPCOLESTR pszPropName,
            /* [in] */ __RPC__in EXCEPINFO *pExcepInfo);
    };

    struct _IErrorLog
    {
        IErrorLogVtbl *lpVtbl;
    };

    struct _IPropertyBagVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IPropertyBag * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IPropertyBag * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IPropertyBag * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Read )( 
            IPropertyBag * This,
            /* [in] */ LPCOLESTR pszPropName,
            /* [out][in] */ VARIANT *pVar,
            /* [unique][in] */ IErrorLog *pErrorLog);
        
        HRESULT ( STDMETHODCALLTYPE *Write )( 
            __RPC__in IPropertyBag * This,
            /* [in] */ __RPC__in LPCOLESTR pszPropName,
            /* [in] */ __RPC__in VARIANT *pVar);
    };

    struct _IPropertyBag
    {
        IPropertyBagVtbl *lpVtbl;
    };

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

    struct _IWTSPluginVtbl
    {
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IWTSPlugin * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IWTSPlugin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IWTSPlugin * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            __RPC__in IWTSPlugin * This,
            /* [in] */ __RPC__in_opt IWTSVirtualChannelManager *pChannelMgr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Connected )( 
            __RPC__in IWTSPlugin * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Disconnected )( 
            __RPC__in IWTSPlugin * This,
            DWORD dwDisconnectCode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Terminated )( 
            __RPC__in IWTSPlugin * This);
    };

    struct _IWTSPlugin
    {
        IWTSPluginVtbl *lpVtbl;
    };

    struct _IWTSListenerVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IWTSListener * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IWTSListener * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IWTSListener * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetConfiguration )( 
            __RPC__in IWTSListener * This,
            /* [out] */ __RPC__deref_out_opt IPropertyBag **ppPropertyBag);
    };

    struct _IWTSListener
    {
        IWTSListenerVtbl *lpVtbl;
    };

    struct _IWTSListenerCallbackVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IWTSListenerCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IWTSListenerCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IWTSListenerCallback * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnNewChannelConnection )( 
            __RPC__in IWTSListenerCallback * This,
            /* [in] */ __RPC__in_opt IWTSVirtualChannel *pChannel,
            /* [full][in] */ __RPC__in_opt BSTR data,
            /* [out] */ __RPC__out BOOL *pbAccept,
            /* [out] */ __RPC__deref_out_opt IWTSVirtualChannelCallback **ppCallback);
    };

    struct _IWTSListenerCallback
    {
        IWTSListenerCallbackVtbl *lpVtbl;
    };

    struct _IWTSVirtualChannelCallbackVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IWTSVirtualChannelCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IWTSVirtualChannelCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IWTSVirtualChannelCallback * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnDataReceived )( 
            __RPC__in IWTSVirtualChannelCallback * This,
            /* [in] */ ULONG cbSize,
            /* [size_is][in] */ __RPC__in_ecount_full(cbSize) BYTE *pBuffer);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnClose )( 
            __RPC__in IWTSVirtualChannelCallback * This);
    };

    struct _IWTSVirtualChannelCallback
    {
        IWTSVirtualChannelCallbackVtbl *lpVtbl;
    };

    struct _IWTSVirtualChannelManagerVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IWTSVirtualChannelManager * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IWTSVirtualChannelManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IWTSVirtualChannelManager * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CreateListener )( 
            __RPC__in IWTSVirtualChannelManager * This,
            /* [string][in] */ __RPC__in_string const char *pszChannelName,
            /* [in] */ ULONG uFlags,
            /* [in] */ __RPC__in_opt IWTSListenerCallback *pListenerCallback,
            /* [out] */ __RPC__deref_out_opt IWTSListener **ppListener);
    };

    struct _IWTSVirtualChannelManager
    {
        IWTSVirtualChannelManagerVtbl *lpVtbl;
    };

    struct _IWTSVirtualChannelVtbl
    {        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IWTSVirtualChannel * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IWTSVirtualChannel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IWTSVirtualChannel * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Write )( 
            __RPC__in IWTSVirtualChannel * This,
            /* [in] */ ULONG cbSize,
            /* [size_is][in] */ __RPC__in_ecount_full(cbSize) BYTE *pBuffer,
            /* [in] */ __RPC__in_opt IUnknown *pReserved);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            __RPC__in IWTSVirtualChannel * This);
    };

    struct _IWTSVirtualChannel
    {
        IWTSVirtualChannelVtbl *lpVtbl;
    };

#endif

#endif /* RDP_DVC_CLIENT_COM_H */
