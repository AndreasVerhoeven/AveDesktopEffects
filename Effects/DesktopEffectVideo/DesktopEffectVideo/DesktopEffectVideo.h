

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Fri May 02 14:35:55 2008
 */
/* Compiler settings for .\DesktopEffectVideo.idl:
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

#ifndef __DesktopEffectVideo_h__
#define __DesktopEffectVideo_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDesktopEffectVideoWallpaper_FWD_DEFINED__
#define __IDesktopEffectVideoWallpaper_FWD_DEFINED__
typedef interface IDesktopEffectVideoWallpaper IDesktopEffectVideoWallpaper;
#endif 	/* __IDesktopEffectVideoWallpaper_FWD_DEFINED__ */


#ifndef __DesktopEffectVideoWallpaper_FWD_DEFINED__
#define __DesktopEffectVideoWallpaper_FWD_DEFINED__

#ifdef __cplusplus
typedef class DesktopEffectVideoWallpaper DesktopEffectVideoWallpaper;
#else
typedef struct DesktopEffectVideoWallpaper DesktopEffectVideoWallpaper;
#endif /* __cplusplus */

#endif 	/* __DesktopEffectVideoWallpaper_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDesktopEffectVideoWallpaper_INTERFACE_DEFINED__
#define __IDesktopEffectVideoWallpaper_INTERFACE_DEFINED__

/* interface IDesktopEffectVideoWallpaper */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDesktopEffectVideoWallpaper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7AC3FE8B-7830-407C-ADFA-8F4793BB5D9D")
    IDesktopEffectVideoWallpaper : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDesktopEffectVideoWallpaperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDesktopEffectVideoWallpaper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDesktopEffectVideoWallpaper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDesktopEffectVideoWallpaper * This);
        
        END_INTERFACE
    } IDesktopEffectVideoWallpaperVtbl;

    interface IDesktopEffectVideoWallpaper
    {
        CONST_VTBL struct IDesktopEffectVideoWallpaperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDesktopEffectVideoWallpaper_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDesktopEffectVideoWallpaper_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDesktopEffectVideoWallpaper_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDesktopEffectVideoWallpaper_INTERFACE_DEFINED__ */



#ifndef __DesktopEffectVideoLib_LIBRARY_DEFINED__
#define __DesktopEffectVideoLib_LIBRARY_DEFINED__

/* library DesktopEffectVideoLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DesktopEffectVideoLib;

EXTERN_C const CLSID CLSID_DesktopEffectVideoWallpaper;

#ifdef __cplusplus

class DECLSPEC_UUID("74C39C8B-7830-407C-ADFA-8F4792AA5D9D")
DesktopEffectVideoWallpaper;
#endif
#endif /* __DesktopEffectVideoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


