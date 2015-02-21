// AveDesktopEffectsHost.h : Declaration of the CAveDesktopEffectsHost

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols

#include "AveDesktopEffects.h"
#include "ConfigureDlg.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

struct AveDesktopEffectInfo
{
	BSTR	clsid;
	BSTR	name;
	BSTR	author;
	BSTR	description;
	HBITMAP		image;

	HRESULT InitializeFromCLSID(const WCHAR* clsidAsString)
	{
		CLSID clsid = {0};
		HRESULT hRes = CLSIDFromString((LPOLESTR)clsidAsString, &clsid);
		if(FAILED(hRes))
			return E_INVALIDARG;

		CComPtr<IAveDesktopEffect> effect;
		hRes = effect.CoCreateInstance(clsid);
		if(FAILED(hRes))
			return E_FAIL;

		this->clsid = SysAllocString(clsidAsString);

		effect->GetName(&this->name);
		effect->GetAuthor(&this->author);
		effect->GetDescription(&this->description);
		effect->GetIcon(&this->image);

		return S_OK;
	}

	void Free()
	{
		SysFreeString(clsid);
		SysFreeString(name);
		SysFreeString(author);
		SysFreeString(description);
		if(image != NULL)
			DeleteObject(image);

		clsid		= NULL;
		name		= NULL;
		author		= NULL;
		description	= NULL;
		image		= NULL;
	}
};


// CAveDesktopEffectsHost

class ATL_NO_VTABLE CAveDesktopEffectsHost :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAveDesktopEffectsHost, &CLSID_AveDesktopEffectsHost>,
	public IAveDesktopEffectsHost,
	public IShellExtInit,
	public IContextMenu
{
	friend class CConfigureDlg;

	// GDI Icon Buffer Variables
	BOOL	iconBufferIsUpdated;
	RECT	iconBufferUpdateRect;
	HDC		iconBufferDC;
	HBITMAP iconBufferBitmap;
	void*	iconBufferBits;
	SIZE	iconBufferSize;

	// Status Variables
	HWND notificationWindow;

	BOOL inD3DMode;
	BOOL isRunning;
	HWND listView;
	HWND defView;
	int d3dModeRefCount;

	// DirectX Variables
	IDirect3D9*				legacyDirect3D;  // DirectX legacy (XP) main variable
	IDirect3DDevice9*		directDevice; // DirectX legacy (XP) device

	IDirect3D9Ex*           direct3D;       // DirectX main variable
	IDirect3DDevice9Ex*     directDeviceEx; // DirectX device
	IDirect3DVertexBuffer9* vertexBuffer;   // DirectX vertex buffer
	IDirect3DTexture9*		texture;		   // DirectX front texture
	IDirect3DSurface9*		sysMemSurface;
	IDirect3DTexture9*		wallpaper;
	RECT sysMemDirtyRect;

	ULONG_PTR gdiplusToken;

	HWND configureDialog;

	// Plugins Variables
	typedef std::vector<CComPtr<IAveDesktopEffect> > EffectList;
	typedef EffectList::iterator EffectIterator;
	EffectList effects;
	std::vector<CString> effectCLSIDs;

	typedef std::vector<AveDesktopEffectInfo> EffectInfoList;
	typedef EffectInfoList::iterator EffectInfoIterator;
	EffectInfoList effectInfos;

	CConfigureDlg configureDlg;

	BOOL hasInitialized;

	BOOL isRunningOnVista;

public:
	CAveDesktopEffectsHost() : configureDlg(this)
	{
		gdiplusToken = NULL;
		wallpaper = NULL;
		hasInitialized = FALSE;
		d3dModeRefCount = 0;
		direct3D = NULL;
		legacyDirect3D = NULL;
		directDeviceEx = NULL;
		directDevice = NULL;
		vertexBuffer = NULL;
		texture = NULL;
		sysMemSurface = NULL;
		SetRect(&sysMemDirtyRect, 0, 0, 0, 0);

		iconBufferSize.cx = 0;
		iconBufferSize.cy = 0;
		iconBufferBits = NULL;
		iconBufferIsUpdated = FALSE;
		SetRect(&iconBufferUpdateRect, 0, 0, 0, 0);
		iconBufferDC = NULL;
		iconBufferBitmap = NULL;

		inD3DMode = FALSE;
		isRunning = FALSE;
		listView = NULL;
		defView = NULL;
		notificationWindow = NULL;

		configureDialog = NULL;

		isRunningOnVista = FALSE;
		_CheckOsVersion();
	}


BEGIN_COM_MAP(CAveDesktopEffectsHost)
	COM_INTERFACE_ENTRY(IAveDesktopEffectsHost)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

protected:
	HRESULT CreateNotificationWindow();
	HRESULT SubclassDesktop();
	HRESULT UnsubclassDesktop();
	HRESULT InternalStart();
	HRESULT InternalStop();
	HRESULT InternalD3DRender();
	HRESULT StartDirectX();
	HRESULT StartIconBufferCapturing();
	HRESULT StopIconBufferCapturing();
	HRESULT StopDirectX();
	HRESULT CopyIconBufferToSysMemSurface(const RECT& rc);
	HRESULT UpdateIconBufferToD3D();
	HRESULT UpdateD3DForSimple2DDrawing();
	HRESULT DrawIconBufferToD3DScene();
	HRESULT UnloadEffects();
	HRESULT InternalInitialize();
	HRESULT WriteRunningStatusToReg(BOOL status);
	HRESULT StartWallpaperBuffering();
	HRESULT StopWallpaperBuffering();

	HRESULT SetUpVertices(float panelWidth, float panelHeight);

	HRESULT FillEffectInfos();
	HRESULT ClearEffectInfos();

	void _CheckOsVersion();
	BOOL _IsRunningVista();

	void _DrawListViewIntoBufferManually(HWND hwnd, HDC dc);

public:

	LRESULT HandleNotificationMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleListViewMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleDefViewMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	DECLARE_REGISTRY_RESOURCEID(IDR_AVEDESKTOPEFFECTSHOST)
	DECLARE_CLASSFACTORY_SINGLETON(CAveDesktopEffectsHost)

	// IShellExtInit
	STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

	// IContextMenu
	STDMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT);
	STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
	STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);

	// IAveDesktopEffectsHost
	STDMETHOD(Start)(void);
	STDMETHOD(Stop)(void);
	STDMETHOD(IsRunning)(VARIANT_BOOL* isRunning);
	STDMETHOD(GetTargetWindow)(HWND* targetWindow);
	STDMETHOD(LoadPluginByCLSID)(BSTR clsid);
	STDMETHOD(IsD3DActive)(VARIANT_BOOL* pIsActive);
	STDMETHOD(D3DRenderIconBuffer)(DWORD mode);
	STDMETHOD(StartD3DMode)(void);
	STDMETHOD(StopD3DMode)(void);
	STDMETHOD(D3DRender)(void);
	STDMETHOD(MakeSureIsInitialized)(void);
	STDMETHOD(GetWallpaperTexture)(IUnknown** texture);
	STDMETHOD(SetWallpaperTexture)(IUnknown* texture);
};

OBJECT_ENTRY_AUTO(__uuidof(AveDesktopEffectsHost), CAveDesktopEffectsHost)
