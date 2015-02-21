// AveDesktopEffect.h : Declaration of the CAveDesktopEffect

#pragma once
#include "resource.h"       // main symbols

#include "AveDesktopEffects.h"
#include "SlideShowConfigDlg.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CAveDesktopEffect

class ATL_NO_VTABLE CAveDesktopEffect :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAveDesktopEffect, &CLSID_AveDesktopEffect>,
	public IAveDesktopEffect
{

	friend CSlideShowConfigDlg;

protected:
	IAveDesktopEffectsHost* host;
	IDirect3DDevice9*       directDevice; 
	IDirect3DTexture9*		currentWall;
	IDirect3DTexture9*		nextWall;
	IDirect3DTexture9*		loadedWall;
	WCHAR wallToBeLoaded[MAX_PATH];
	WCHAR wallBaseDir[MAX_PATH];

	std::vector<CString> files;
	int currentFileIndex;

	CRITICAL_SECTION cs;

	POINT ptMouse;
	int counter;
	float fadingProgress;
	bool up;

	HANDLE hThread, hThreadEnumFiles;
	BOOL doStop;

	CSlideShowConfigDlg configDlg;

	int photoInterval;
	int transitionDurationMs;
	DWORD transitionTickStart;
	DWORD transitionCurrentTick;

public:
	CAveDesktopEffect() : configDlg(this)
	{
		loadedWall = NULL;
		InitializeCriticalSection(&cs);
		transitionTickStart = 0;
		transitionCurrentTick = 0;
		fadingProgress = 0.0f;
		transitionDurationMs = 2000;
		photoInterval = 30;
		currentFileIndex = 0;
		doStop = FALSE;
		ZeroMemory(wallToBeLoaded, MAX_PATH*sizeof(WCHAR));
		ZeroMemory(wallBaseDir, MAX_PATH*sizeof(WCHAR));
		hThread = NULL;
		hThreadEnumFiles = NULL;
		up = true;
		currentWall = NULL;
		nextWall = NULL;
		host = NULL;
		directDevice = NULL;
		ptMouse.x = 0;
		ptMouse.y = 0;
		counter = 0;
	}

	~CAveDesktopEffect()
	{
		DeleteCriticalSection(&cs);
	}

	void SetBaseDir(const WCHAR* path);
	void SetPhotoInterval(int sec);
	void SetFadeInterval(int ms);
	void ToNextPhoto();

	DWORD _OnLoader();
	DWORD _OnEnumFiles();
	void _LoadNextWallpaper(const WCHAR* fileName);
	void _StartEnumFiles();

DECLARE_REGISTRY_RESOURCEID(IDR_AVEDESKTOPEFFECT)


BEGIN_COM_MAP(CAveDesktopEffect)
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

OBJECT_ENTRY_AUTO(__uuidof(AveDesktopEffect), CAveDesktopEffect)

