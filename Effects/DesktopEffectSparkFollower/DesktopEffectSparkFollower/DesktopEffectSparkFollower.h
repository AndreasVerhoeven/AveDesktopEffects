

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Fri May 02 14:34:44 2008
 */
/* Compiler settings for .\DesktopEffectSparkFollower.idl:
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

#ifndef __DesktopEffectSparkFollower_h__
#define __DesktopEffectSparkFollower_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICDesktopEffectSparkle_FWD_DEFINED__
#define __ICDesktopEffectSparkle_FWD_DEFINED__
typedef interface ICDesktopEffectSparkle ICDesktopEffectSparkle;
#endif 	/* __ICDesktopEffectSparkle_FWD_DEFINED__ */


#ifndef __CDesktopEffectSparkle_FWD_DEFINED__
#define __CDesktopEffectSparkle_FWD_DEFINED__

#ifdef __cplusplus
typedef class CDesktopEffectSparkle CDesktopEffectSparkle;
#else
typedef struct CDesktopEffectSparkle CDesktopEffectSparkle;
#endif /* __cplusplus */

#endif 	/* __CDesktopEffectSparkle_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICDesktopEffectSparkle_INTERFACE_DEFINED__
#define __ICDesktopEffectSparkle_INTERFACE_DEFINED__

/* interface ICDesktopEffectSparkle */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICDesktopEffectSparkle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("731CF036-8506-4DE5-9857-98667F9D0D67")
    ICDesktopEffectSparkle : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICDesktopEffectSparkleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICDesktopEffectSparkle * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICDesktopEffectSparkle * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICDesktopEffectSparkle * This);
        
        END_INTERFACE
    } ICDesktopEffectSparkleVtbl;

    interface ICDesktopEffectSparkle
    {
        CONST_VTBL struct ICDesktopEffectSparkleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICDesktopEffectSparkle_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICDesktopEffectSparkle_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICDesktopEffectSparkle_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICDesktopEffectSparkle_INTERFACE_DEFINED__ */



#ifndef __DesktopEffectSparkFollowerLib_LIBRARY_DEFINED__
#define __DesktopEffectSparkFollowerLib_LIBRARY_DEFINED__

/* library DesktopEffectSparkFollowerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DesktopEffectSparkFollowerLib;

EXTERN_C const CLSID CLSID_CDesktopEffectSparkle;

#ifdef __cplusplus

class DECLSPEC_UUID("D9ED8807-9D64-4AC3-B1F2-C0FB05BD63AF")
CDesktopEffectSparkle;
#endif
#endif /* __DesktopEffectSparkFollowerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


