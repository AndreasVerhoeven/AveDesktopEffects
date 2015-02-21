// DesktopEffectSnow.h : Declaration of the CDesktopEffectSnow

#pragma once
#include "resource.h"       // main symbols

#include "DesktopEffectWinter.h"
#include "X:\coding\DesktopFxSDK\AveDesktopEffects.h"
#include "X:\coding\DesktopFxSDK\AveDesktopEffectDefines.h"
#include "X:\coding\DesktopFxSDK\AveRegFuncs.h"
#include "ParticleSystem.h"
#include "ConfigureDlg.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CDesktopEffectSnow

class ATL_NO_VTABLE CDesktopEffectSnow :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDesktopEffectSnow, &CLSID_DesktopEffectSnow>,
	public IAveDesktopEffect
{
protected:
	friend CConfigureDlg;

	IAveDesktopEffectsHost* host;
	IDirect3DDevice9*       directDevice;
	std::vector<IDirect3DTexture9*>		particles;
	std::vector<IDirect3DTexture9*>		icons;
	IDirect3DTexture9*		ground;
	SIZE sceneSize;

	CParticleSystem system;
	int counter;

	int maxCounter;
	int speed;
	int wind;
	int size;
	BOOL doRandomFlakes;
	int flakesPeriod;
	int nextMaxTarget;
	float targetAdd;
	float fMaxCounter;

	CConfigureDlg dlg;

	BOOL doRenderGround;

public:
	CDesktopEffectSnow() : dlg(this)
	{
		doRenderGround = TRUE;
		ground = NULL;
		fMaxCounter = 0.0f;
		targetAdd = 0.0f;
		nextMaxTarget = 0;
		flakesPeriod = 0;
		doRandomFlakes = FALSE;
		size = 20;
		wind = 0;
		speed = 100;
		maxCounter = 2;
		counter = 0;
		sceneSize.cx = 0;
		sceneSize.cy = 0;
		host = NULL;
		directDevice = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DESKTOPEFFECTSNOW)


BEGIN_COM_MAP(CDesktopEffectSnow)
	COM_INTERFACE_ENTRY(IAveDesktopEffect)
END_COM_MAP()

	void SetWind(int val);
	void SetSpeed(int val);
	void SetMaxCounter(int val);
	void SetSize(int val);
	void SetDoRandomFlakes(BOOL val);
	void SetDoRenderGround(BOOL renderGround);

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

OBJECT_ENTRY_AUTO(__uuidof(DesktopEffectSnow), CDesktopEffectSnow)
