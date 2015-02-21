

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Thu Sep 18 01:47:09 2008
 */
/* Compiler settings for .\DesktopEffectFlash.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DesktopEffectFlash_h__
#define __DesktopEffectFlash_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDesktopEffectFlashHost_FWD_DEFINED__
#define __IDesktopEffectFlashHost_FWD_DEFINED__
typedef interface IDesktopEffectFlashHost IDesktopEffectFlashHost;
#endif 	/* __IDesktopEffectFlashHost_FWD_DEFINED__ */


#ifndef __DesktopEffectFlashHost_FWD_DEFINED__
#define __DesktopEffectFlashHost_FWD_DEFINED__

#ifdef __cplusplus
typedef class DesktopEffectFlashHost DesktopEffectFlashHost;
#else
typedef struct DesktopEffectFlashHost DesktopEffectFlashHost;
#endif /* __cplusplus */

#endif 	/* __DesktopEffectFlashHost_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDesktopEffectFlashHost_INTERFACE_DEFINED__
#define __IDesktopEffectFlashHost_INTERFACE_DEFINED__

/* interface IDesktopEffectFlashHost */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDesktopEffectFlashHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("199F7876-2B14-4A04-BA41-E9CB2133E399")
    IDesktopEffectFlashHost : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDesktopEffectFlashHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDesktopEffectFlashHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDesktopEffectFlashHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDesktopEffectFlashHost * This);
        
        END_INTERFACE
    } IDesktopEffectFlashHostVtbl;

    interface IDesktopEffectFlashHost
    {
        CONST_VTBL struct IDesktopEffectFlashHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDesktopEffectFlashHost_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDesktopEffectFlashHost_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDesktopEffectFlashHost_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDesktopEffectFlashHost_INTERFACE_DEFINED__ */



#ifndef __DesktopEffectFlashLib_LIBRARY_DEFINED__
#define __DesktopEffectFlashLib_LIBRARY_DEFINED__

/* library DesktopEffectFlashLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DesktopEffectFlashLib;

EXTERN_C const CLSID CLSID_DesktopEffectFlashHost;

#ifdef __cplusplus

class DECLSPEC_UUID("B9DD0CF2-4CF1-4676-A3DD-7F7359C30339")
DesktopEffectFlashHost;
#endif
#endif /* __DesktopEffectFlashLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


