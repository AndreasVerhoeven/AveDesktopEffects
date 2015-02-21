// DesktopEffectVideoWallpaper.h : Declaration of the CDesktopEffectVideoWallpaper

#pragma once
#include "resource.h"       // main symbols

#include "DesktopEffectVideo.h"
#include "stdafx.h"
#include "X:\coding\DesktopFxSDK\AveDesktopEffects.h"
#include "X:\coding\DesktopFxSDK\AveDesktopEffectDefines.h"
#include "X:\coding\DesktopFxSDK\AveRegFuncs.h"
#include <vector>
#include <dshow.h>
#include <Vmr9.h>
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "dxguid.lib")

#include "ConfigureDlg.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CDesktopEffectVideoWallpaper
struct __declspec(uuid("{73471540-2017-22cf-ae26-0020afdaee67}")) CLSID_AveTextureRenderer;
class ATL_NO_VTABLE CDesktopEffectVideoWallpaper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDesktopEffectVideoWallpaper, &CLSID_DesktopEffectVideoWallpaper>,
	public IAveDesktopEffect,
	public IVMRSurfaceAllocator9,
	public IVMRImagePresenter9
{
protected:
	IAveDesktopEffectsHost*		host;
	IDirect3DDevice9*			directDevice;
	IDirect3DTexture9*			texture;
	IDirect3DTexture9*			videoTexture;
	CComPtr<IDirect3DTexture9>	renderTexture;
	SIZE textureSize;

	CComPtr<IGraphBuilder> graphBuilder;
	CComPtr<IMediaControl> mediaControl;
	CComPtr<IMediaPosition> mediaPosition;
	CComPtr<IMediaEvent> mediaEvent;
	CComPtr<ICaptureGraphBuilder2>	captureGraphBuilder;

	std::vector<IDirect3DSurface9*> surfaces;
	CComPtr<IVMRSurfaceAllocatorNotify9> allocatorNotify;

	HRESULT StartVMR();
	HRESULT StopVMR();

	WCHAR fileName[MAX_PATH];

	CConfigureDlg configDlg;

public:
	CDesktopEffectVideoWallpaper() : configDlg(this)
	{
		memset(fileName, 0, sizeof(fileName));
		videoTexture = NULL;
		textureSize.cx = 0;
		textureSize.cy = 0;
		host = NULL;
		directDevice = NULL;
		texture = NULL;
	}

	const WCHAR* GetFileName() { return fileName; }
	void SetFileName(const WCHAR* fileName);

DECLARE_REGISTRY_RESOURCEID(IDR_DESKTOPEFFECTVIDEOWALLPAPER)


BEGIN_COM_MAP(CDesktopEffectVideoWallpaper)
	COM_INTERFACE_ENTRY(IAveDesktopEffect)
	COM_INTERFACE_ENTRY(IVMRSurfaceAllocator9)
	COM_INTERFACE_ENTRY(IVMRImagePresenter9)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	void ClearSurfaces();

public:
	STDMETHOD(GetName)(BSTR* pName);
	STDMETHOD(GetAuthor)(BSTR* pAuthor);
	STDMETHOD(GetDescription)(BSTR* pDescription);
	STDMETHOD(GetIcon)(HBITMAP* pHBitmap);
	STDMETHOD(GetCooperationFlags)(DWORD* pFlags);
	STDMETHOD(OnInitialize)(IAveDesktopEffectsHost* host);
	STDMETHOD(OnUninitialize)(void);
	STDMETHOD(OnStart)(void);
	STDMETHOD(OnStop)(void);
	STDMETHOD(OnPause)(void);
	STDMETHOD(OnResume)(void);
	STDMETHOD(OnD3DStart)(IUnknown* directDevice);
	STDMETHOD(OnD3DStop)(void);
	STDMETHOD(OnBeforeGdiBackgroundUpdate)(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo);
	
	STDMETHOD(OnAfterGdiBackgroundUpdate)(HDC hdc, const RECT* updateRect);
	STDMETHOD(OnBeforeIconBufferUpdate)(void);
	STDMETHOD(OnAfterIconBufferUpdate)(void);
	STDMETHOD(OnD3DRender)(BOOL* pHasRendered);
	STDMETHOD(OnBeforeD3DRender)(DWORD* pBgColor);
	STDMETHOD(OnBeforeD3DSceneStart)(void);
	STDMETHOD(OnD3DSceneStart)(void);
	STDMETHOD(OnBeforeIconBufferRender)(void);
	STDMETHOD(OnAfterIconBufferRender)(void);
	STDMETHOD(OnD3DSceneEnd)(void);
	STDMETHOD(OnBeforeD3DPresent)(void);
	STDMETHOD(OnAfterD3DPresent)(void);
	STDMETHOD(OnAfterD3DRender)(void);
	STDMETHOD(OnTargetWindowMessage)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled);
	STDMETHOD(GetConfigurationWindow)(HWND* hwnd, HWND parent);
	STDMETHOD(OnNotification)(DWORD dwNotification);
	STDMETHOD(DoesSupport)(DWORD* pFlag);

	STDMETHOD(InitializeDevice)( 
	 DWORD_PTR dwUserID,
		VMR9AllocationInfo *lpAllocInfo,
		DWORD *lpNumBuffers);

	HRESULT CALLBACK GetSurface(
  DWORD_PTR  dwUserID,
  DWORD  SurfaceIndex,
  DWORD  SurfaceFlags,
  IDirect3DSurface9**  lplpSurface
  ) { surfaces[SurfaceIndex]->AddRef(); *lplpSurface = surfaces[SurfaceIndex]; return S_OK; }


	HRESULT CALLBACK TerminateDevice( DWORD_PTR  dwID){return S_OK;};

	HRESULT CALLBACK PresentImage(
    DWORD_PTR dwUserID, 
    VMR9PresentationInfo *lpPresInfo);
	
	HRESULT CALLBACK AdviseNotify(IVMRSurfaceAllocatorNotify9*  lpIVMRSurfAllocNotify) { allocatorNotify=lpIVMRSurfAllocNotify; return S_OK;};
	HRESULT CALLBACK StartPresenting(DWORD_PTR  dwUserID){return S_OK;};
	HRESULT CALLBACK StopPresenting(DWORD_PTR  dwUserID){return S_OK;};


};

OBJECT_ENTRY_AUTO(__uuidof(DesktopEffectVideoWallpaper), CDesktopEffectVideoWallpaper)
