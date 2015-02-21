

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Fri May 02 14:35:32 2008
 */
/* Compiler settings for .\DesktopEffectUnderwater.idl:
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

#ifndef __DesktopEffectUnderwater_h__
#define __DesktopEffectUnderwater_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDesktopEffectWaterWallpaper_FWD_DEFINED__
#define __IDesktopEffectWaterWallpaper_FWD_DEFINED__
typedef interface IDesktopEffectWaterWallpaper IDesktopEffectWaterWallpaper;
#endif 	/* __IDesktopEffectWaterWallpaper_FWD_DEFINED__ */


#ifndef __DesktopEffectWaterWallpaper_FWD_DEFINED__
#define __DesktopEffectWaterWallpaper_FWD_DEFINED__

#ifdef __cplusplus
typedef class DesktopEffectWaterWallpaper DesktopEffectWaterWallpaper;
#else
typedef struct DesktopEffectWaterWallpaper DesktopEffectWaterWallpaper;
#endif /* __cplusplus */

#endif 	/* __DesktopEffectWaterWallpaper_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDesktopEffectWaterWallpaper_INTERFACE_DEFINED__
#define __IDesktopEffectWaterWallpaper_INTERFACE_DEFINED__

/* interface IDesktopEffectWaterWallpaper */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDesktopEffectWaterWallpaper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("78DDFA41-420E-47D0-8D21-874D288BCAE8")
    IDesktopEffectWaterWallpaper : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDesktopEffectWaterWallpaperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDesktopEffectWaterWallpaper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDesktopEffectWaterWallpaper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDesktopEffectWaterWallpaper * This);
        
        END_INTERFACE
    } IDesktopEffectWaterWallpaperVtbl;

    interface IDesktopEffectWaterWallpaper
    {
        CONST_VTBL struct IDesktopEffectWaterWallpaperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDesktopEffectWaterWallpaper_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDesktopEffectWaterWallpaper_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDesktopEffectWaterWallpaper_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDesktopEffectWaterWallpaper_INTERFACE_DEFINED__ */



#ifndef __DesktopEffectUnderwaterLib_LIBRARY_DEFINED__
#define __DesktopEffectUnderwaterLib_LIBRARY_DEFINED__

/* library DesktopEffectUnderwaterLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DesktopEffectUnderwaterLib;

EXTERN_C const CLSID CLSID_DesktopEffectWaterWallpaper;

#ifdef __cplusplus

class DECLSPEC_UUID("0F110EB8-E4BE-421D-9E71-5D580F220270")
DesktopEffectWaterWallpaper;
#endif
#endif /* __DesktopEffectUnderwaterLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


