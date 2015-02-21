

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Fri May 02 14:31:40 2008
 */
/* Compiler settings for .\DesktopEffectAutoFader.idl:
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

#ifndef __DesktopEffectAutoFader_h__
#define __DesktopEffectAutoFader_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDesktopEffectAutoWallpaperFader_FWD_DEFINED__
#define __IDesktopEffectAutoWallpaperFader_FWD_DEFINED__
typedef interface IDesktopEffectAutoWallpaperFader IDesktopEffectAutoWallpaperFader;
#endif 	/* __IDesktopEffectAutoWallpaperFader_FWD_DEFINED__ */


#ifndef __DesktopEffectAutoWallpaperFader_FWD_DEFINED__
#define __DesktopEffectAutoWallpaperFader_FWD_DEFINED__

#ifdef __cplusplus
typedef class DesktopEffectAutoWallpaperFader DesktopEffectAutoWallpaperFader;
#else
typedef struct DesktopEffectAutoWallpaperFader DesktopEffectAutoWallpaperFader;
#endif /* __cplusplus */

#endif 	/* __DesktopEffectAutoWallpaperFader_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDesktopEffectAutoWallpaperFader_INTERFACE_DEFINED__
#define __IDesktopEffectAutoWallpaperFader_INTERFACE_DEFINED__

/* interface IDesktopEffectAutoWallpaperFader */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDesktopEffectAutoWallpaperFader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FF51AC1F-9D92-401D-9C85-F25E3E72B28E")
    IDesktopEffectAutoWallpaperFader : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDesktopEffectAutoWallpaperFaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDesktopEffectAutoWallpaperFader * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDesktopEffectAutoWallpaperFader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDesktopEffectAutoWallpaperFader * This);
        
        END_INTERFACE
    } IDesktopEffectAutoWallpaperFaderVtbl;

    interface IDesktopEffectAutoWallpaperFader
    {
        CONST_VTBL struct IDesktopEffectAutoWallpaperFaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDesktopEffectAutoWallpaperFader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDesktopEffectAutoWallpaperFader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDesktopEffectAutoWallpaperFader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDesktopEffectAutoWallpaperFader_INTERFACE_DEFINED__ */



#ifndef __DesktopEffectAutoFaderLib_LIBRARY_DEFINED__
#define __DesktopEffectAutoFaderLib_LIBRARY_DEFINED__

/* library DesktopEffectAutoFaderLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DesktopEffectAutoFaderLib;

EXTERN_C const CLSID CLSID_DesktopEffectAutoWallpaperFader;

#ifdef __cplusplus

class DECLSPEC_UUID("CF79C32A-9CFB-4C86-BC7F-CAEDC7FB98FB")
DesktopEffectAutoWallpaperFader;
#endif
#endif /* __DesktopEffectAutoFaderLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


