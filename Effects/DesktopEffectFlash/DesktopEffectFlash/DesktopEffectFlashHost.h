// DesktopEffectFlashHost.h : Declaration of the CDesktopEffectFlashHost

#pragma once
#include "resource.h"       // main symbols

#include "DesktopEffectFlash.h"

#include "..\..\DesktopFxSDK\AveDesktopEffects.h"
#include "..\..\DesktopFxSDK\AveDesktopEffectDefines.h"
#include "..\..\DesktopFxSDK\AveRegFuncs.h"

#import "c:\\windows\\system32\\macromed\\flash\\Flash9e.ocx" named_guids


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#include "ConfigureDlg.h"

// CDesktopEffectFlashHost

class ATL_NO_VTABLE CDesktopEffectFlashHost :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDesktopEffectFlashHost, &CLSID_DesktopEffectFlashHost>,
	public IDesktopEffectFlashHost,
	public IAveDesktopEffect,
	public IOleClientSite,
	public IOleInPlaceSiteWindowless,
	public IOleInPlaceFrame,
	public IStorage,
	public ShockwaveFlashObjects::_IShockwaveFlashEvents
{
protected:

	DWORD dwCookie;
	
	CConfigureDlg dlg;

	CComPtr<IAveDesktopEffectsHost> host;
	CComPtr<IDirect3DDevice9>     directDevice; 


	GUID clsid;
	CComPtr<ShockwaveFlashObjects::IShockwaveFlash> flash;
	CComPtr<IOleObject> oleObject;
	CComPtr<IViewObjectEx> viewObject;
	CComPtr<IViewObjectEx> viewObjectEx;
	CComPtr<IOleInPlaceObjectWindowless> inPlaceObjectWindowLess;
	CComPtr<IConnectionPointContainer> connectionPointContainer;
	CComPtr<IConnectionPoint> connectionPoint;

	BOOL noRedraw;

	BOOL hasCapture;
	BOOL hasFocus;

	BOOL isReleasing;

	void doReleaseAll()
	{
		AddRef();

		if(isReleasing)
			return;

		isReleasing = TRUE;

		if(oleObject)
		{
			oleObject->Close(OLECLOSE_NOSAVE);
			oleObject->SetClientSite(NULL);
		}

		viewObject.Release();
		viewObjectEx.Release();

		inPlaceObjectWindowLess.Release();
		oleObject.Release();


		connectionPointContainer.Release();
		connectionPoint.Release();

		AtlUnadvise(flash, __uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents), dwCookie);
		flash.Release();
		dwCookie = 0;

		isReleasing = FALSE;

	}



	HRESULT create(GUID clsid)
	{
		HRESULT hRes = S_OK;

		this->clsid = clsid;

		//CComQIPtr<IStorage> storage((IDesktopEffectFlashHost*)this);

		hRes = oleObject.CoCreateInstance(clsid);

		DWORD dwMiscStatus = 0;
		oleObject->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);

		//HRESULT hRes = S_OK;
		//hRes = OleCreate(clsid, IID_IOleObject, OLERENDER_DRAW, NULL, NULL, NULL, (void**)&oleObject);
		if(FAILED(hRes) || NULL == oleObject)
		{
			MessageBox(0, L"Failed OleCreate", 0, MB_ICONERROR);
			return hRes;
		}

		//hRes = OleSetContainedObject(oleObject, TRUE);
		//if(FAILED(hRes))
		//	return hRes;

		CComQIPtr<IOleClientSite> clientSite((IDesktopEffectFlashHost*)this);
		hRes = oleObject->SetClientSite(clientSite);

		hRes = oleObject->QueryInterface(&flash);
		hRes = oleObject->QueryInterface(&viewObjectEx);
		hRes = oleObject->QueryInterface(&viewObject);

		hRes = oleObject->QueryInterface(&inPlaceObjectWindowLess);

		AtlAdvise(flash, (ShockwaveFlashObjects::_IShockwaveFlashEvents*)this, __uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents), &dwCookie );

	
		//AddRef();

		return hRes;
	}

	BOOL IsMouseMessage(UINT msg)
	{
		return msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST || WM_SETCURSOR == msg;
		
		/*
		return msg == WM_MOUSEMOVE || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_LBUTTONDBLCLK
			|| msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP || msg == WM_RBUTTONDBLCLK
			|| msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP || msg == WM_MBUTTONDBLCLK
			|| msg == WM_MOUSEWHEEL 
			|| msg == WM_SETCURSOR;
		*/
	}

	BOOL IsKeyBoardMessage(UINT msg)
	{
		return msg >= WM_KEYFIRST && msg <= WM_KEYLAST || WM_CANCELMODE == msg;
	}

public:
	CDesktopEffectFlashHost() : dlg(this)
	{
		dwCookie = 0;
		noRedraw = FALSE;
		hasCapture = FALSE;
		hasFocus = FALSE;
		isReleasing = FALSE;
		scaling = 2;
	}

	BOOL IsOnListView(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL IsOnObject(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

DECLARE_REGISTRY_RESOURCEID(IDR_DESKTOPEFFECTFLASHHOST)


BEGIN_COM_MAP(CDesktopEffectFlashHost)
	COM_INTERFACE_ENTRY(IDesktopEffectFlashHost)
	COM_INTERFACE_ENTRY(IAveDesktopEffect)
	COM_INTERFACE_ENTRY(IOleInPlaceSiteWindowless)
	COM_INTERFACE_ENTRY(IOleClientSite)
	COM_INTERFACE_ENTRY(IOleInPlaceSiteEx)
	COM_INTERFACE_ENTRY(IOleInPlaceFrame)
	COM_INTERFACE_ENTRY(IStorage)
	COM_INTERFACE_ENTRY(ShockwaveFlashObjects::_IShockwaveFlashEvents)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	HWND getWindow()
	{
		HWND hwnd = NULL;
		if(host != NULL)
			host->GetTargetWindow(&hwnd);

		return hwnd;
	}

	RECT getOutputRect()
	{
		RECT r = {0};
		::GetClientRect(getWindow(), &r);
		

		CComQIPtr<IAveMultiMonitorHelper> mmHelper(host);
		if(mmHelper != NULL)
		{
			mmHelper->GetMonitorRect(0, &r, TRUE);
		}	
		
		return r;
	}

	// settings
	int scaling;
	CComBSTR movieUrl;
	CComBSTR flashVars;

public:

	void setScaling(int scalingId);
	int  getScaling();
	const WCHAR* mapScalingToString(int scalingId);

	void setMovieUrl(const WCHAR* url);
	const WCHAR* getMovieUrl();

	void setFlashVars(const WCHAR* vars);
	const WCHAR* getFlashVars();


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


	//IOleClientSite
	virtual HRESULT STDMETHODCALLTYPE SaveObject() { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk) { *ppmk = NULL; return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE GetContainer(LPOLECONTAINER FAR* ppContainer) { *ppContainer = NULL; return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE ShowObject() {return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout() { return S_OK; }

	//IOleInPlaceSite
	virtual HRESULT STDMETHODCALLTYPE GetWindow(HWND FAR* lphwnd)
	{ 
		*lphwnd = getWindow();
		return S_OK;

		//host->GetHelperWindow((IUnknown*)(IAveDesktopEffect*)this, lphwnd); 
		//return S_OK; 
	}

	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate() { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate() { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE OnUIActivate() { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame,LPOLEINPLACEUIWINDOW FAR* lplpDoc,LPRECT lprcPosRect,LPRECT lprcClipRect,LPOLEINPLACEFRAMEINFO lpFrameInfo)
	{
		if(isReleasing)
			return E_FAIL;

		*lplpFrame = (LPOLEINPLACEFRAME)this;
		*lplpDoc = NULL;

		lpFrameInfo->fMDIApp = FALSE;
		lpFrameInfo->hwndFrame = getWindow(); // hwnd
		lpFrameInfo->haccel = 0;
		lpFrameInfo->cAccelEntries = 0;
	
		RECT r = getOutputRect();
		
		*lprcPosRect = r;
		*lprcClipRect = r;

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtent)  { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate() { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState() { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo() { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect) { return S_OK; }

	//IOleInPlaceSiteEx
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivateEx(BOOL __RPC_FAR *pfNoRedraw, DWORD dwFlags)
	{
		if(*pfNoRedraw)
			*pfNoRedraw = noRedraw;

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivateEx(BOOL fNoRedraw) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE RequestUIActivate(void) { return S_OK; }



	//IOleInPlaceSiteWindowless
	virtual HRESULT STDMETHODCALLTYPE CanWindowlessActivate( void) 
	{ 
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetCapture( void) 
	{
		return hasCapture ? S_OK : S_FALSE;
	}

    virtual HRESULT STDMETHODCALLTYPE SetCapture(BOOL fCapture)
	{
		if(fCapture)
		{
			hasCapture = TRUE;
			::SetCapture(getWindow());
		}
		else
		{
			hasCapture = FALSE;
			::SetCapture(NULL);
		}

		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE GetFocus( void)
	{
		return hasFocus ? S_OK : S_FALSE;

	}

    virtual HRESULT STDMETHODCALLTYPE SetFocus(BOOL fFocus)
	{
		if(fFocus)
		{
			hasFocus = TRUE;
			::SetFocus(getWindow());
		}
		else
		{
			hasFocus = FALSE;
			::SetFocus(NULL);
		}

		return S_OK;
	}

    virtual HRESULT STDMETHODCALLTYPE GetDC( 
        /* [in] */ LPCRECT pRect,
        /* [in] */ DWORD grfFlags,
        /* [out] */ HDC __RPC_FAR *phDC)
	{
		return S_FALSE;
	}

    virtual HRESULT STDMETHODCALLTYPE ReleaseDC( 
        /* [in] */ HDC hDC)
	{
		return S_FALSE;
	}


    virtual HRESULT STDMETHODCALLTYPE InvalidateRect( 
        /* [in] */ LPCRECT pRect,
        /* [in] */ BOOL fErase)
	{

		HWND hwnd = getWindow();

		WCHAR className[1024] = {0};
		GetClassName(hwnd, className, 1024);
		BOOL isListView = _wcsicmp(className, L"SysListView32") == 0;

		// we hook into the WM_ERASEBKGND when no listview is shown!
		if(!isListView)
			fErase = TRUE;

		// TODO draw
		
		::InvalidateRect(hwnd, pRect, fErase);

		return S_OK;
	}

    virtual HRESULT STDMETHODCALLTYPE InvalidateRgn( 
        /* [in] */ HRGN hRGN,
        /* [in] */ BOOL fErase)
	{
		return S_FALSE;
	}

    virtual HRESULT STDMETHODCALLTYPE ScrollRect( 
        /* [in] */ INT dx,
        /* [in] */ INT dy,
        /* [in] */ LPCRECT pRectScroll,
        /* [in] */ LPCRECT pRectClip)
	{
		return E_NOTIMPL;
	}

    virtual HRESULT STDMETHODCALLTYPE AdjustRect( 
        /* [out][in] */ LPRECT prc)
	{
		if(prc != NULL)
		{
			prc = prc;
		}

		return S_FALSE;
	}

    virtual HRESULT STDMETHODCALLTYPE OnDefWindowMessage( 
        /* [in] */ UINT msg,
        /* [in] */ WPARAM wParam,
        /* [in] */ LPARAM lParam,
        /* [out] */ LRESULT __RPC_FAR *plResult)
	{
		return S_FALSE;
	}

	//IOleInPlaceFrame
//	virtual HRESULT STDMETHODCALLTYPE GetWindow(HWND FAR* lphwnd);
//	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
	virtual HRESULT STDMETHODCALLTYPE GetBorder(LPRECT lprectBorder) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS pborderwidths) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS pborderwidths) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU hmenuShared) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR pszStatusText) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpmsg, WORD wID) { return S_OK; }

	//IStorage
	virtual HRESULT STDMETHODCALLTYPE CreateStream(const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE OpenStream(const WCHAR * pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE CreateStorage(const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE OpenStorage(const WCHAR * pwcsName, IStorage * pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE CopyTo(DWORD ciidExclude, IID const *rgiidExclude, SNB snbExclude,IStorage *pstgDest) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(const OLECHAR *pwcsName,IStorage * pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE Revert() { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE EnumElements(DWORD reserved1, void * reserved2, DWORD reserved3, IEnumSTATSTG ** ppenum) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE DestroyElement(const OLECHAR *pwcsName) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE RenameElement(const WCHAR *pwcsOldName, const WCHAR *pwcsNewName) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(const WCHAR *pwcsName, FILETIME const *pctime, FILETIME const *patime, FILETIME const *pmtime) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE SetClass(REFCLSID clsid) { return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE SetStateBits(DWORD grfStateBits, DWORD grfMask) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG * pstatstg, DWORD grfStatFlag) { return E_NOTIMPL; }

	// IDispatch
	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo){return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT , LCID , ITypeInfo **){return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID , LPOLESTR *, UINT , LCID , DISPID *){return E_NOTIMPL;}

	HRESULT STDMETHODCALLTYPE Invoke( 
    /* [in] */ DISPID dispIdMember,
    /* [in] */ REFIID riid,
    /* [in] */ LCID lcid,
    /* [in] */ WORD wFlags,
    /* [out][in] */ DISPPARAMS *pDispParams,
    /* [out] */ VARIANT *pVarResult,
    /* [out] */ EXCEPINFO *pExcepInfo,
    /* [out] */ UINT *puArgErr)
	{
		switch(dispIdMember)
		{
			case 0x96: 
			if ((pDispParams->cArgs == 2) &&
			(pDispParams->rgvarg[0].vt == VT_BSTR) &&
			(pDispParams->rgvarg[1].vt == VT_BSTR))
			{
				return FSCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
			}
			break;
		} 
		return S_OK;
	}

	// _IShockWaveFlashEvents
	virtual HRESULT STDMETHODCALLTYPE OnReadyStateChange (long newState ) { return E_NOTIMPL;};
    virtual HRESULT STDMETHODCALLTYPE OnProgress (long percentDone ){ return E_NOTIMPL;};
    virtual HRESULT STDMETHODCALLTYPE FSCommand (_bstr_t command,_bstr_t args )
	{
		if(command.GetBSTR() == NULL)
			return S_OK;

		if(_wcsicmp(command, L"msgbox") == 0)
		{
			MessageBox(NULL, args, 0, MB_ICONINFORMATION);
		}
		else if(_wcsicmp(command, L"exec") == 0)
		{
			WCHAR path[4096] = {0};
			ExpandEnvironmentStrings(args, path, _countof(path));
			ShellExecute(NULL, NULL, path, NULL, NULL, SW_SHOW);
		}
		else if(_wcsicmp(command, L"desktopeffects") == 0)
		{
			if(args.GetBSTR() == NULL)
				return S_OK;

			if(NULL == this->host)
				return S_OK;

			if(_wcsicmp(args, L"stop") == 0)
			{
				this->host->Stop();
			}
		}

		return S_OK;
	}

    virtual HRESULT STDMETHODCALLTYPE FlashCall (_bstr_t request ){ return E_NOTIMPL;};

};

OBJECT_ENTRY_AUTO(__uuidof(DesktopEffectFlashHost), CDesktopEffectFlashHost)
