

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Tue Jan 15 03:21:35 2008
 */
/* Compiler settings for .\AveDesktopEffects.idl:
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

#ifndef __AveDesktopEffects_h__
#define __AveDesktopEffects_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAveDesktopEffectsHost_FWD_DEFINED__
#define __IAveDesktopEffectsHost_FWD_DEFINED__
typedef interface IAveDesktopEffectsHost IAveDesktopEffectsHost;
#endif 	/* __IAveDesktopEffectsHost_FWD_DEFINED__ */


#ifndef __IAveDesktopEffect_FWD_DEFINED__
#define __IAveDesktopEffect_FWD_DEFINED__
typedef interface IAveDesktopEffect IAveDesktopEffect;
#endif 	/* __IAveDesktopEffect_FWD_DEFINED__ */


#ifndef __AveDesktopEffectsHost_FWD_DEFINED__
#define __AveDesktopEffectsHost_FWD_DEFINED__

#ifdef __cplusplus
typedef class AveDesktopEffectsHost AveDesktopEffectsHost;
#else
typedef struct AveDesktopEffectsHost AveDesktopEffectsHost;
#endif /* __cplusplus */

#endif 	/* __AveDesktopEffectsHost_FWD_DEFINED__ */


#ifndef __AveDesktopEffect_FWD_DEFINED__
#define __AveDesktopEffect_FWD_DEFINED__

#ifdef __cplusplus
typedef class AveDesktopEffect AveDesktopEffect;
#else
typedef struct AveDesktopEffect AveDesktopEffect;
#endif /* __cplusplus */

#endif 	/* __AveDesktopEffect_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAveDesktopEffectsHost_INTERFACE_DEFINED__
#define __IAveDesktopEffectsHost_INTERFACE_DEFINED__

/* interface IAveDesktopEffectsHost */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAveDesktopEffectsHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DC830845-6C17-48F9-932A-82979DFB3936")
    IAveDesktopEffectsHost : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Start( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsRunning( 
            /* [out] */ VARIANT_BOOL *isRunning) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetTargetWindow( 
            /* [out] */ HWND *targetWindow) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LoadPluginByCLSID( 
            /* [in] */ BSTR clsid) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsD3DActive( 
            /* [out] */ VARIANT_BOOL *pIsActive) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE D3DRenderIconBuffer( 
            /* [in] */ DWORD mode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StartD3DMode( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StopD3DMode( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE D3DRender( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE MakeSureIsInitialized( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetWallpaperTexture( 
            /* [out] */ IUnknown **texture) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetWallpaperTexture( 
            /* [in] */ IUnknown *texture) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAveDesktopEffectsHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAveDesktopEffectsHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAveDesktopEffectsHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAveDesktopEffectsHost * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Start )( 
            IAveDesktopEffectsHost * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IAveDesktopEffectsHost * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsRunning )( 
            IAveDesktopEffectsHost * This,
            /* [out] */ VARIANT_BOOL *isRunning);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTargetWindow )( 
            IAveDesktopEffectsHost * This,
            /* [out] */ HWND *targetWindow);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *LoadPluginByCLSID )( 
            IAveDesktopEffectsHost * This,
            /* [in] */ BSTR clsid);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsD3DActive )( 
            IAveDesktopEffectsHost * This,
            /* [out] */ VARIANT_BOOL *pIsActive);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *D3DRenderIconBuffer )( 
            IAveDesktopEffectsHost * This,
            /* [in] */ DWORD mode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StartD3DMode )( 
            IAveDesktopEffectsHost * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopD3DMode )( 
            IAveDesktopEffectsHost * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *D3DRender )( 
            IAveDesktopEffectsHost * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *MakeSureIsInitialized )( 
            IAveDesktopEffectsHost * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetWallpaperTexture )( 
            IAveDesktopEffectsHost * This,
            /* [out] */ IUnknown **texture);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetWallpaperTexture )( 
            IAveDesktopEffectsHost * This,
            /* [in] */ IUnknown *texture);
        
        END_INTERFACE
    } IAveDesktopEffectsHostVtbl;

    interface IAveDesktopEffectsHost
    {
        CONST_VTBL struct IAveDesktopEffectsHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAveDesktopEffectsHost_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAveDesktopEffectsHost_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAveDesktopEffectsHost_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAveDesktopEffectsHost_Start(This)	\
    ( (This)->lpVtbl -> Start(This) ) 

#define IAveDesktopEffectsHost_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IAveDesktopEffectsHost_IsRunning(This,isRunning)	\
    ( (This)->lpVtbl -> IsRunning(This,isRunning) ) 

#define IAveDesktopEffectsHost_GetTargetWindow(This,targetWindow)	\
    ( (This)->lpVtbl -> GetTargetWindow(This,targetWindow) ) 

#define IAveDesktopEffectsHost_LoadPluginByCLSID(This,clsid)	\
    ( (This)->lpVtbl -> LoadPluginByCLSID(This,clsid) ) 

#define IAveDesktopEffectsHost_IsD3DActive(This,pIsActive)	\
    ( (This)->lpVtbl -> IsD3DActive(This,pIsActive) ) 

#define IAveDesktopEffectsHost_D3DRenderIconBuffer(This,mode)	\
    ( (This)->lpVtbl -> D3DRenderIconBuffer(This,mode) ) 

#define IAveDesktopEffectsHost_StartD3DMode(This)	\
    ( (This)->lpVtbl -> StartD3DMode(This) ) 

#define IAveDesktopEffectsHost_StopD3DMode(This)	\
    ( (This)->lpVtbl -> StopD3DMode(This) ) 

#define IAveDesktopEffectsHost_D3DRender(This)	\
    ( (This)->lpVtbl -> D3DRender(This) ) 

#define IAveDesktopEffectsHost_MakeSureIsInitialized(This)	\
    ( (This)->lpVtbl -> MakeSureIsInitialized(This) ) 

#define IAveDesktopEffectsHost_GetWallpaperTexture(This,texture)	\
    ( (This)->lpVtbl -> GetWallpaperTexture(This,texture) ) 

#define IAveDesktopEffectsHost_SetWallpaperTexture(This,texture)	\
    ( (This)->lpVtbl -> SetWallpaperTexture(This,texture) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAveDesktopEffectsHost_INTERFACE_DEFINED__ */


#ifndef __IAveDesktopEffect_INTERFACE_DEFINED__
#define __IAveDesktopEffect_INTERFACE_DEFINED__

/* interface IAveDesktopEffect */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IAveDesktopEffect;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("77766B10-7FB8-49D6-895E-2A5B52EAB8E4")
    IAveDesktopEffect : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR *pName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetAuthor( 
            /* [out] */ BSTR *pAuthor) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [out] */ BSTR *pDescription) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetIcon( 
            /* [out] */ HBITMAP *pHBitmap) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetCooperationFlags( 
            /* [out] */ DWORD *pFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnInitialize( 
            /* [in] */ IAveDesktopEffectsHost *host) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUninitialize( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnStart( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnStop( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnPause( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnResume( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnD3DStart( 
            /* [in] */ IUnknown *directDevice) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnD3DStop( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBeforeGdiBackgroundUpdate( 
            /* [in] */ HDC hdc,
            /* [in] */ const RECT *updateRect,
            /* [out] */ DWORD *pWhatToDo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnAfterGdiBackgroundUpdate( 
            /* [in] */ HDC hdc,
            /* [in] */ const RECT *updateRect) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBeforeIconBufferUpdate( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnAfterIconBufferUpdate( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnD3DRender( 
            /* [out] */ BOOL *pHasRendered) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBeforeD3DRender( 
            /* [out] */ DWORD *pBgColor) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBeforeD3DSceneStart( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnD3DSceneStart( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBeforeIconBufferRender( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnAfterIconBufferRender( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnD3DSceneEnd( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBeforeD3DPresent( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnAfterD3DPresent( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnAfterD3DRender( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnTargetWindowMessage( 
            /* [in] */ HWND hwnd,
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam,
            /* [out] */ LRESULT *lResult,
            /* [out] */ BOOL *bHandled) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetConfigurationWindow( 
            /* [out] */ HWND *hwnd,
            /* [in] */ HWND parent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnNotification( 
            DWORD dwNotification) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoesSupport( 
            /* [out] */ DWORD *pFlag) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAveDesktopEffectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAveDesktopEffect * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAveDesktopEffect * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IAveDesktopEffect * This,
            /* [out] */ BSTR *pName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAuthor )( 
            IAveDesktopEffect * This,
            /* [out] */ BSTR *pAuthor);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            IAveDesktopEffect * This,
            /* [out] */ BSTR *pDescription);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetIcon )( 
            IAveDesktopEffect * This,
            /* [out] */ HBITMAP *pHBitmap);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCooperationFlags )( 
            IAveDesktopEffect * This,
            /* [out] */ DWORD *pFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnInitialize )( 
            IAveDesktopEffect * This,
            /* [in] */ IAveDesktopEffectsHost *host);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnUninitialize )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnStart )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnStop )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnPause )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnResume )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnD3DStart )( 
            IAveDesktopEffect * This,
            /* [in] */ IUnknown *directDevice);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnD3DStop )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBeforeGdiBackgroundUpdate )( 
            IAveDesktopEffect * This,
            /* [in] */ HDC hdc,
            /* [in] */ const RECT *updateRect,
            /* [out] */ DWORD *pWhatToDo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnAfterGdiBackgroundUpdate )( 
            IAveDesktopEffect * This,
            /* [in] */ HDC hdc,
            /* [in] */ const RECT *updateRect);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBeforeIconBufferUpdate )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnAfterIconBufferUpdate )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnD3DRender )( 
            IAveDesktopEffect * This,
            /* [out] */ BOOL *pHasRendered);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBeforeD3DRender )( 
            IAveDesktopEffect * This,
            /* [out] */ DWORD *pBgColor);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBeforeD3DSceneStart )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnD3DSceneStart )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBeforeIconBufferRender )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnAfterIconBufferRender )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnD3DSceneEnd )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBeforeD3DPresent )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnAfterD3DPresent )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnAfterD3DRender )( 
            IAveDesktopEffect * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnTargetWindowMessage )( 
            IAveDesktopEffect * This,
            /* [in] */ HWND hwnd,
            /* [in] */ UINT msg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam,
            /* [out] */ LRESULT *lResult,
            /* [out] */ BOOL *bHandled);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetConfigurationWindow )( 
            IAveDesktopEffect * This,
            /* [out] */ HWND *hwnd,
            /* [in] */ HWND parent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnNotification )( 
            IAveDesktopEffect * This,
            DWORD dwNotification);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DoesSupport )( 
            IAveDesktopEffect * This,
            /* [out] */ DWORD *pFlag);
        
        END_INTERFACE
    } IAveDesktopEffectVtbl;

    interface IAveDesktopEffect
    {
        CONST_VTBL struct IAveDesktopEffectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAveDesktopEffect_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAveDesktopEffect_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAveDesktopEffect_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAveDesktopEffect_GetName(This,pName)	\
    ( (This)->lpVtbl -> GetName(This,pName) ) 

#define IAveDesktopEffect_GetAuthor(This,pAuthor)	\
    ( (This)->lpVtbl -> GetAuthor(This,pAuthor) ) 

#define IAveDesktopEffect_GetDescription(This,pDescription)	\
    ( (This)->lpVtbl -> GetDescription(This,pDescription) ) 

#define IAveDesktopEffect_GetIcon(This,pHBitmap)	\
    ( (This)->lpVtbl -> GetIcon(This,pHBitmap) ) 

#define IAveDesktopEffect_GetCooperationFlags(This,pFlags)	\
    ( (This)->lpVtbl -> GetCooperationFlags(This,pFlags) ) 

#define IAveDesktopEffect_OnInitialize(This,host)	\
    ( (This)->lpVtbl -> OnInitialize(This,host) ) 

#define IAveDesktopEffect_OnUninitialize(This)	\
    ( (This)->lpVtbl -> OnUninitialize(This) ) 

#define IAveDesktopEffect_OnStart(This)	\
    ( (This)->lpVtbl -> OnStart(This) ) 

#define IAveDesktopEffect_OnStop(This)	\
    ( (This)->lpVtbl -> OnStop(This) ) 

#define IAveDesktopEffect_OnPause(This)	\
    ( (This)->lpVtbl -> OnPause(This) ) 

#define IAveDesktopEffect_OnResume(This)	\
    ( (This)->lpVtbl -> OnResume(This) ) 

#define IAveDesktopEffect_OnD3DStart(This,directDevice)	\
    ( (This)->lpVtbl -> OnD3DStart(This,directDevice) ) 

#define IAveDesktopEffect_OnD3DStop(This)	\
    ( (This)->lpVtbl -> OnD3DStop(This) ) 

#define IAveDesktopEffect_OnBeforeGdiBackgroundUpdate(This,hdc,updateRect,pWhatToDo)	\
    ( (This)->lpVtbl -> OnBeforeGdiBackgroundUpdate(This,hdc,updateRect,pWhatToDo) ) 

#define IAveDesktopEffect_OnAfterGdiBackgroundUpdate(This,hdc,updateRect)	\
    ( (This)->lpVtbl -> OnAfterGdiBackgroundUpdate(This,hdc,updateRect) ) 

#define IAveDesktopEffect_OnBeforeIconBufferUpdate(This)	\
    ( (This)->lpVtbl -> OnBeforeIconBufferUpdate(This) ) 

#define IAveDesktopEffect_OnAfterIconBufferUpdate(This)	\
    ( (This)->lpVtbl -> OnAfterIconBufferUpdate(This) ) 

#define IAveDesktopEffect_OnD3DRender(This,pHasRendered)	\
    ( (This)->lpVtbl -> OnD3DRender(This,pHasRendered) ) 

#define IAveDesktopEffect_OnBeforeD3DRender(This,pBgColor)	\
    ( (This)->lpVtbl -> OnBeforeD3DRender(This,pBgColor) ) 

#define IAveDesktopEffect_OnBeforeD3DSceneStart(This)	\
    ( (This)->lpVtbl -> OnBeforeD3DSceneStart(This) ) 

#define IAveDesktopEffect_OnD3DSceneStart(This)	\
    ( (This)->lpVtbl -> OnD3DSceneStart(This) ) 

#define IAveDesktopEffect_OnBeforeIconBufferRender(This)	\
    ( (This)->lpVtbl -> OnBeforeIconBufferRender(This) ) 

#define IAveDesktopEffect_OnAfterIconBufferRender(This)	\
    ( (This)->lpVtbl -> OnAfterIconBufferRender(This) ) 

#define IAveDesktopEffect_OnD3DSceneEnd(This)	\
    ( (This)->lpVtbl -> OnD3DSceneEnd(This) ) 

#define IAveDesktopEffect_OnBeforeD3DPresent(This)	\
    ( (This)->lpVtbl -> OnBeforeD3DPresent(This) ) 

#define IAveDesktopEffect_OnAfterD3DPresent(This)	\
    ( (This)->lpVtbl -> OnAfterD3DPresent(This) ) 

#define IAveDesktopEffect_OnAfterD3DRender(This)	\
    ( (This)->lpVtbl -> OnAfterD3DRender(This) ) 

#define IAveDesktopEffect_OnTargetWindowMessage(This,hwnd,msg,wParam,lParam,lResult,bHandled)	\
    ( (This)->lpVtbl -> OnTargetWindowMessage(This,hwnd,msg,wParam,lParam,lResult,bHandled) ) 

#define IAveDesktopEffect_GetConfigurationWindow(This,hwnd,parent)	\
    ( (This)->lpVtbl -> GetConfigurationWindow(This,hwnd,parent) ) 

#define IAveDesktopEffect_OnNotification(This,dwNotification)	\
    ( (This)->lpVtbl -> OnNotification(This,dwNotification) ) 

#define IAveDesktopEffect_DoesSupport(This,pFlag)	\
    ( (This)->lpVtbl -> DoesSupport(This,pFlag) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAveDesktopEffect_INTERFACE_DEFINED__ */



#ifndef __AveDesktopEffectsLib_LIBRARY_DEFINED__
#define __AveDesktopEffectsLib_LIBRARY_DEFINED__

/* library AveDesktopEffectsLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_AveDesktopEffectsLib;

EXTERN_C const CLSID CLSID_AveDesktopEffectsHost;

#ifdef __cplusplus

class DECLSPEC_UUID("96443122-0A33-4C5B-AEB5-7309276D3C71")
AveDesktopEffectsHost;
#endif

EXTERN_C const CLSID CLSID_AveDesktopEffect;

#ifdef __cplusplus

class DECLSPEC_UUID("BB813704-8B48-433E-A44E-E5311661D136")
AveDesktopEffect;
#endif
#endif /* __AveDesktopEffectsLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  HBITMAP_UserSize(     unsigned long *, unsigned long            , HBITMAP * ); 
unsigned char * __RPC_USER  HBITMAP_UserMarshal(  unsigned long *, unsigned char *, HBITMAP * ); 
unsigned char * __RPC_USER  HBITMAP_UserUnmarshal(unsigned long *, unsigned char *, HBITMAP * ); 
void                      __RPC_USER  HBITMAP_UserFree(     unsigned long *, HBITMAP * ); 

unsigned long             __RPC_USER  HDC_UserSize(     unsigned long *, unsigned long            , HDC * ); 
unsigned char * __RPC_USER  HDC_UserMarshal(  unsigned long *, unsigned char *, HDC * ); 
unsigned char * __RPC_USER  HDC_UserUnmarshal(unsigned long *, unsigned char *, HDC * ); 
void                      __RPC_USER  HDC_UserFree(     unsigned long *, HDC * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long *, unsigned long            , HWND * ); 
unsigned char * __RPC_USER  HWND_UserMarshal(  unsigned long *, unsigned char *, HWND * ); 
unsigned char * __RPC_USER  HWND_UserUnmarshal(unsigned long *, unsigned char *, HWND * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long *, HWND * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


