#pragma once


typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, WS_EX_APPWINDOW> CAveWindowTraits;


class ATL_NO_VTABLE CAveFakeDesktop :
	public CWindowImpl<CAveFakeDesktop, CWindow, CAveWindowTraits>,
	public CComObjectRoot,
	public IShellBrowser
{
public:
	CAveFakeDesktop(void);
	~CAveFakeDesktop(void);

	CDC dc;
	CBitmap bmp;

	ULONG_PTR gdiplusToken;

	CWindow defView;
	CComPtr<IShellView> view;

	BOOL loadHost;


	BEGIN_COM_MAP(CAveFakeDesktop)
		COM_INTERFACE_ENTRY(IShellBrowser)
		COM_INTERFACE_ENTRY(IOleWindow)
		//COM_INTERFACE_ENTRY(ICommDlgBrowser)
	END_COM_MAP()

	DECLARE_WND_CLASS(_T("Progman"))

	BEGIN_MSG_MAP(CAveFakeDesktop)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PRINT, OnPrintClient)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPrintClient)
		MESSAGE_HANDLER(WM_WININICHANGE, OnWinIniChange) 
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowposChanged)
	END_MSG_MAP()

	LRESULT OnWinIniChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnWindowposChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPrintClient(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void _FixWallpaper();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode){return E_NOTIMPL;}	
	// *** ICommDlgBrowser methods ***
    STDMETHOD(OnDefaultCommand) (THIS_ struct IShellView * ppshv)
	{	//handle double click and ENTER key if needed
		return E_NOTIMPL; 
	}
    STDMETHOD(OnStateChange) (THIS_ struct IShellView * ppshv,ULONG uChange)
	{	//handle selection, rename, focus if needed
		return S_OK; 
	}
    STDMETHOD(IncludeObject) (THIS_ struct IShellView * ppshv,LPCITEMIDLIST pidl)
	{	//filter files if needed
		return S_OK;
	}
    // *** IShellBrowser methods *** (same as IOleInPlaceFrame)
    STDMETHOD(InsertMenusSB) (HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths) {return E_NOTIMPL;}
    STDMETHOD(SetMenuSB) (HMENU hmenuShared, HOLEMENU holemenuReserved,HWND hwndActiveObject){return E_NOTIMPL;}
    STDMETHOD(RemoveMenusSB) (HMENU hmenuShared){return E_NOTIMPL;}
    STDMETHOD(SetStatusTextSB) (LPCOLESTR lpszStatusText){return E_NOTIMPL;}
    STDMETHOD(EnableModelessSB) (BOOL fEnable){return E_NOTIMPL;}
	STDMETHOD(BrowseObject)(LPCITEMIDLIST pidl, UINT wFlags)
	{
		return E_NOTIMPL;	
	}

	STDMETHOD(GetViewStateStream)(DWORD grfMode,LPSTREAM  *ppStrm)
	{
		if(NULL == ppStrm)
			return E_INVALIDARG;
		
		return E_NOTIMPL;
	}

	STDMETHOD(OnViewWindowActive)(struct IShellView *ppshv){return E_NOTIMPL;}
	STDMETHOD(SetToolbarItems)(LPTBBUTTON lpButtons, UINT nButtons,UINT uFlags){return E_NOTIMPL;}
	STDMETHOD(TranslateAcceleratorSB) (LPMSG lpmsg, WORD wID)
	{

		return E_NOTIMPL; 
	}

	//IOleWindow
	STDMETHOD(GetWindow)(HWND * lphwnd)
	{ 
		*lphwnd = m_hWnd; 
		return S_OK; 
	}
	//IShellBrowser
	STDMETHOD(QueryActiveShellView)(struct IShellView ** ppshv)
	{
		view.CopyTo(ppshv);
		//*ppshv = view;
		return S_OK; 
	}
	STDMETHOD(GetControlWindow)(UINT id, HWND * lphwnd)
	{
		if(lphwnd == NULL)
			return E_POINTER;


		return E_NOTIMPL;
	}

	STDMETHOD(SendControlMsg)(UINT id, UINT uMsg, WPARAM wParam,LPARAM lParam, LRESULT *pret)
	{
		if(pret == NULL)
			return E_POINTER;
	
		return E_NOTIMPL;
	}

	STDMETHOD(OnMessage)(MSG* pMsg) { return E_NOTIMPL; }
	STDMETHOD(Stop)(void) { return E_NOTIMPL; }
	STDMETHOD(SaveState)(void) { return E_NOTIMPL; }
	STDMETHOD(GetShellViewWindow)(HWND* pHwnd)
	{
		if(NULL == pHwnd)
			return E_POINTER;

		*pHwnd = defView;

		return S_OK;
	}
};
