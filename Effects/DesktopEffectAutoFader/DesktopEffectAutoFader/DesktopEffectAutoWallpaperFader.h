// DesktopEffectAutoWallpaperFader.h : Declaration of the CDesktopEffectAutoWallpaperFader

#pragma once
#include "resource.h"       // main symbols

#include "DesktopEffectAutoFader.h"
#include "X:\coding\DesktopFxSDK\AveDesktopEffects.h"
#include "X:\coding\DesktopFxSDK\AveDesktopEffectDefines.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CDesktopEffectAutoWallpaperFader

class ATL_NO_VTABLE CDesktopEffectAutoWallpaperFader :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDesktopEffectAutoWallpaperFader, &CLSID_DesktopEffectAutoWallpaperFader>,
	public IAveDesktopEffect
{
protected:
	IAveDesktopEffectsHost*		host;
	IDirect3DDevice9*			directDevice;
	CComPtr<IDirect3DTexture9>	oldWallpaper;
	CComPtr<IDirect3DTexture9>	newWallpaper;
	DWORD startTick;
	DWORD fadingDuration;
	float fadingProgress;

public:
	CDesktopEffectAutoWallpaperFader()
	{
		host = NULL;
		directDevice = NULL;
		oldWallpaper = NULL;
		newWallpaper = NULL;
		fadingDuration = 4000;
		startTick = 0;
		fadingProgress = 0.0f;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DESKTOPEFFECTAUTOWALLPAPERFADER)


BEGIN_COM_MAP(CDesktopEffectAutoWallpaperFader)
	COM_INTERFACE_ENTRY(IAveDesktopEffect)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

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
};

OBJECT_ENTRY_AUTO(__uuidof(DesktopEffectAutoWallpaperFader), CDesktopEffectAutoWallpaperFader)
