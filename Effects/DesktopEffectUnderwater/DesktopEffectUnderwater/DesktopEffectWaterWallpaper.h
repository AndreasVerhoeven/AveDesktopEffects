// DesktopEffectWaterWallpaper.h : Declaration of the CDesktopEffectWaterWallpaper

#pragma once
#include "resource.h"       // main symbols

#include "stdafx.h"
#include "DesktopEffectUnderwater.h"
#include "..\..\DesktopFxSDK\AveDesktopEffects.h"
#include "..\..\DesktopFxSDK\AveDesktopEffectDefines.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CDesktopEffectWaterWallpaper

class ATL_NO_VTABLE CDesktopEffectWaterWallpaper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDesktopEffectWaterWallpaper, &CLSID_DesktopEffectWaterWallpaper>,
	public IAveDesktopEffect
{
protected:
	HDC bufferDC;
	HBITMAP bufferBitmap;
	SIZE bufferSize;
	void* bufferBits;
	IAveDesktopEffectsHost* host;
	IDirect3DDevice9*       directDevice;
	IDirect3DTexture9*	texture;

	LPDIRECT3DVERTEXBUFFER9 m_pWaterVB;
    LPDIRECT3DTEXTURE9      m_pBumpMap;
	FLOAT					m_fTime;  

	int waveHeight;
	int waveLength;
	int waveSpeed;
	int waveDistortion;
	int waveDistortionMin;
	int waveDistortionMax;

	BOOL randomGoingUp;
	BOOL doRandom;

public:
	CDesktopEffectWaterWallpaper()
	{
		doRandom = TRUE;
		randomGoingUp = TRUE;
		waveDistortionMin = 5000;
		waveDistortionMax = 50000;
		waveDistortion = 20000;
		waveSpeed = 30;
		waveHeight = 24;
		waveLength = 4;
		m_fTime = 0.0f;
		m_pBumpMap = NULL;
		m_pWaterVB = NULL;
		bufferBits = NULL;
		bufferSize.cx = 0;
		bufferSize.cy = 0;
		bufferBitmap = NULL;
		bufferDC = NULL;
		texture = NULL;
		host = NULL;
		directDevice = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DESKTOPEFFECTWATERWALLPAPER)


BEGIN_COM_MAP(CDesktopEffectWaterWallpaper)
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

	HRESULT _CreateBumpMap();
	HRESULT _DestroyScribbleBuffer();
	HRESULT _CreateScribbleBuffer();
	HRESULT _InitBumpMap();

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
	STDMETHOD(OnNotificationWindowMessage)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled);
};

OBJECT_ENTRY_AUTO(__uuidof(DesktopEffectWaterWallpaper), CDesktopEffectWaterWallpaper)
