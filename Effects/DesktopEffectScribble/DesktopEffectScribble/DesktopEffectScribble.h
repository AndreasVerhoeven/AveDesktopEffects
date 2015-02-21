

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Mon May 19 00:08:03 2008
 */
/* Compiler settings for .\DesktopEffectScribble.idl:
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

#ifndef __DesktopEffectScribble_h__
#define __DesktopEffectScribble_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAveDesktopEffectScribble_FWD_DEFINED__
#define __IAveDesktopEffectScribble_FWD_DEFINED__
typedef interface IAveDesktopEffectScribble IAveDesktopEffectScribble;
#endif 	/* __IAveDesktopEffectScribble_FWD_DEFINED__ */


#ifndef __AveDesktopEffectScribble_FWD_DEFINED__
#define __AveDesktopEffectScribble_FWD_DEFINED__

#ifdef __cplusplus
typedef class AveDesktopEffectScribble AveDesktopEffectScribble;
#else
typedef struct AveDesktopEffectScribble AveDesktopEffectScribble;
#endif /* __cplusplus */

#endif 	/* __AveDesktopEffectScribble_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAveDesktopEffectScribble_INTERFACE_DEFINED__
#define __IAveDesktopEffectScribble_INTERFACE_DEFINED__

/* interface IAveDesktopEffectScribble */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAveDesktopEffectScribble;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0225A8D1-A0D5-4438-AFF2-811C143C687A")
    IAveDesktopEffectScribble : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAveDesktopEffectScribbleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAveDesktopEffectScribble * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAveDesktopEffectScribble * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAveDesktopEffectScribble * This);
        
        END_INTERFACE
    } IAveDesktopEffectScribbleVtbl;

    interface IAveDesktopEffectScribble
    {
        CONST_VTBL struct IAveDesktopEffectScribbleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAveDesktopEffectScribble_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAveDesktopEffectScribble_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAveDesktopEffectScribble_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAveDesktopEffectScribble_INTERFACE_DEFINED__ */



#ifndef __DesktopEffectScribbleLib_LIBRARY_DEFINED__
#define __DesktopEffectScribbleLib_LIBRARY_DEFINED__

/* library DesktopEffectScribbleLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DesktopEffectScribbleLib;

EXTERN_C const CLSID CLSID_AveDesktopEffectScribble;

#ifdef __cplusplus

class DECLSPEC_UUID("00D81C23-8E8C-45CE-BD1F-809EB4F26852")
AveDesktopEffectScribble;
#endif
#endif /* __DesktopEffectScribbleLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


