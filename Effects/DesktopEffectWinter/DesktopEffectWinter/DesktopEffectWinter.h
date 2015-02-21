

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Fri May 02 14:36:19 2008
 */
/* Compiler settings for .\DesktopEffectWinter.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __DesktopEffectWinter_h__
#define __DesktopEffectWinter_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDesktopEffectSnow_FWD_DEFINED__
#define __IDesktopEffectSnow_FWD_DEFINED__
typedef interface IDesktopEffectSnow IDesktopEffectSnow;
#endif 	/* __IDesktopEffectSnow_FWD_DEFINED__ */


#ifndef __DesktopEffectSnow_FWD_DEFINED__
#define __DesktopEffectSnow_FWD_DEFINED__

#ifdef __cplusplus
typedef class DesktopEffectSnow DesktopEffectSnow;
#else
typedef struct DesktopEffectSnow DesktopEffectSnow;
#endif /* __cplusplus */

#endif 	/* __DesktopEffectSnow_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDesktopEffectSnow_INTERFACE_DEFINED__
#define __IDesktopEffectSnow_INTERFACE_DEFINED__

/* interface IDesktopEffectSnow */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDesktopEffectSnow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7245CD05-8A20-4ACC-A35F-FEAD5331222F")
    IDesktopEffectSnow : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDesktopEffectSnowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDesktopEffectSnow * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDesktopEffectSnow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDesktopEffectSnow * This);
        
        END_INTERFACE
    } IDesktopEffectSnowVtbl;

    interface IDesktopEffectSnow
    {
        CONST_VTBL struct IDesktopEffectSnowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDesktopEffectSnow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDesktopEffectSnow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDesktopEffectSnow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDesktopEffectSnow_INTERFACE_DEFINED__ */



#ifndef __DesktopEffectWinterLib_LIBRARY_DEFINED__
#define __DesktopEffectWinterLib_LIBRARY_DEFINED__

/* library DesktopEffectWinterLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DesktopEffectWinterLib;

EXTERN_C const CLSID CLSID_DesktopEffectSnow;

#ifdef __cplusplus

class DECLSPEC_UUID("A9ABADD0-5490-4EE0-A472-CE07F530BBB0")
DesktopEffectSnow;
#endif
#endif /* __DesktopEffectWinterLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


