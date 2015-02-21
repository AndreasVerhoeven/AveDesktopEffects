// DesktopEffectFlashHost.cpp : Implementation of CDesktopEffectFlashHost

#include "stdafx.h"
#include "DesktopEffectFlashHost.h"


// CDesktopEffectFlashHost

// Called when the name of this effect is required
STDMETHODIMP CDesktopEffectFlashHost::GetName(BSTR* pName)
{
	if(NULL == pName)
		return E_INVALIDARG;

	CString name;
	name.LoadString(IDS_EFFECTNAME);
	*pName = SysAllocString(name);

	return S_OK;
}

// Called when the author of this effect is required
STDMETHODIMP CDesktopEffectFlashHost::GetAuthor(BSTR* pAuthor)
{
	if(NULL == pAuthor)
		return E_INVALIDARG;

	*pAuthor = SysAllocString(L"Andreas Verhoeven");

	return S_OK;
}

// Called when the description of this effect is required
STDMETHODIMP CDesktopEffectFlashHost::GetDescription(BSTR* pDescription)
{
	if(NULL == pDescription)
		return E_INVALIDARG;

	CString description;
	description.LoadString(IDS_DESCRIPTION);
	*pDescription = SysAllocString(description);

	return S_OK;
}

// Called when an icon of this effect is required
STDMETHODIMP CDesktopEffectFlashHost::GetIcon(HBITMAP* pHBitmap)
{
	// TODO: Load 64x 64 ARGB Bitmap that serves as an icon

	return S_OK;
}

// Called when the cooperation flags are needed.
// NOT IMPLEMENTED CURRENTLY
STDMETHODIMP CDesktopEffectFlashHost::GetCooperationFlags(DWORD* pFlags)
{
	// NOT IMPLEMENTED CURRENTLY

	return S_OK;
}

// Called when this effect is loaded and initialized.
STDMETHODIMP CDesktopEffectFlashHost::OnInitialize(IAveDesktopEffectsHost* host)
{
	// Store a ptr to our host
	this->host = host;


	CString url = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Flash", L"MovieUrl", L"");
	movieUrl = CComBSTR(url);

	CString vars = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\FlashVars", L"MovieUrl", L"");
	flashVars = CComBSTR(vars);

	scaling = AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Flash", L"Scaling", scaling);

	return S_OK;
}

// Called when this effect is unloaded
STDMETHODIMP CDesktopEffectFlashHost::OnUninitialize(void)
{
	// Release our ptr to our host
	host.Release();

	return S_OK;
}

// Called when DesktopEffects rendering is started
STDMETHODIMP CDesktopEffectFlashHost::OnStart(void)
{
	// D3D example:
	//host->StartD3DMode();

	create(ShockwaveFlashObjects::CLSID_ShockwaveFlash);
	if(flash != NULL)
	{
		flash->put_WMode(CComBSTR(L"transparent"));
		flash->put_Scale(CComBSTR(mapScalingToString(scaling)));
		flash->put_BackgroundColor(0xFFFFFFFF);
		flash->put_EmbedMovie(VARIANT_FALSE);
		flash->put_AllowNetworking(CComBSTR(L"yes"));
		flash->DisableLocalSecurity();


		flash->put_FlashVars(flashVars);

		CComQIPtr<IOleClientSite> clientSite((IDesktopEffectFlashHost*)this);
		HRESULT hRes = oleObject->DoVerb(OLEIVERB_SHOW, NULL, clientSite, 0, NULL, NULL);

		flash->put_EmbedMovie(VARIANT_TRUE);
		flash->put_Movie(movieUrl);
		//flash->raw_LoadMovie(0, movieUrl);
		flash->Play();

		flash.Release();
	}

	return S_OK;
}


void CDesktopEffectFlashHost::setScaling(int newScaling)
{
	if(scaling != newScaling)
	{
		scaling = newScaling;
		CComQIPtr<ShockwaveFlashObjects::IShockwaveFlash> flash(oleObject);
		if(flash != NULL)
		{
			const WCHAR* scaleString = mapScalingToString(scaling);
			flash->put_Scale(CComBSTR(scaleString));

			AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Flash", L"Scaling", scaling);
		}
	}
}

int  CDesktopEffectFlashHost::getScaling()
{
	return scaling;
}

const WCHAR* CDesktopEffectFlashHost::mapScalingToString(int scalingId)
{
	switch(scalingId)
	{
		case 0:  return L"exactFit";
		case 1:  return L"showAll";
		case 2:  return L"noScale";
		default: return L"noScale";
	}
}

void CDesktopEffectFlashHost::setMovieUrl(const WCHAR* url)
{
	if(movieUrl != CComBSTR(url))
	{
		movieUrl = CComBSTR(url);
		CComQIPtr<ShockwaveFlashObjects::IShockwaveFlash> flash(oleObject);
		flash->raw_Stop();
		flash->put_Movie(movieUrl);
		flash->raw_LoadMovie(0, movieUrl);
		flash->Rewind();
		flash->Play();

		AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Flash", L"MovieUrl", url);

		doReleaseAll();
		OnStart();
	}
}


const WCHAR* CDesktopEffectFlashHost::getMovieUrl()
{
	return movieUrl != NULL ? (const WCHAR*)movieUrl : NULL;
}

void CDesktopEffectFlashHost::setFlashVars(const WCHAR* vars)
{
	if(flashVars != CComBSTR(vars))
	{
		flashVars = CComBSTR(vars);
		AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Flash", L"FlashVars", vars);

		//doReleaseAll();
		//OnStart();
	
	}
}

const WCHAR* CDesktopEffectFlashHost::getFlashVars()
{
	return flashVars != NULL ? (const WCHAR*)flashVars : NULL;
}

// Called when DesktopEffects rendering is stopped
STDMETHODIMP CDesktopEffectFlashHost::OnStop(void)
{
	
	// D3D example:
	//if(directDevice != NULL)
	//	host->StopD3DMode();

	doReleaseAll();

	return S_OK;
}

// Called when DesktopEffects is paused
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectFlashHost::OnPause(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when DesktopEffects is resumed from a paused state
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectFlashHost::OnResume(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called when D3D mode is started. Gives a Pointer to an IUnknown that implements
// IDirect3DDevice9.
STDMETHODIMP CDesktopEffectFlashHost::OnD3DStart(IUnknown* directDeviceAsUnknown)
{
	// D3D example:
	if(NULL == directDeviceAsUnknown)
		return E_INVALIDARG;

	CComQIPtr<IDirect3DDevice9> castedDevice(directDeviceAsUnknown);
	directDevice = castedDevice;

	if(NULL == directDevice)
		return E_INVALIDARG;

	return S_OK;
}

// Called when D3D mode is stopped.
STDMETHODIMP CDesktopEffectFlashHost::OnD3DStop(void)
{
	directDevice.Release();

	return S_OK;
}

// Called when the GDI background is updated.
// The GDI hdc and updateRect are passed, and pWhatToDo must be set to one of the following:
//	- AVE_GDI_BACKGROUND_PAINT_DO_NOT_OVERRIDE, let the normal background be painted
//	- AVE_GDI_BACKGROUND_PAINT_DO_NOT_CLEAR, do not clear the background with solid black.
//	- AVE_GDI_BACKGROUND_PAINT_DO_CLEAR, clear the background with solid black.
//	- AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG, let the normal background be painted and call 
//												OnAfterGdiBackgroundUpdate()
STDMETHODIMP CDesktopEffectFlashHost::OnBeforeGdiBackgroundUpdate(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo)
{
	*pWhatToDo = AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG;

	return S_OK;
}

// Will only be called when pWhatToDo was set to AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG in 
// OnBeforeGdiBackgroundUpdate after the normal background has been painted.
// Allows to paint on top of the background.
// The GDI hdc and update are passed.
STDMETHODIMP CDesktopEffectFlashHost::OnAfterGdiBackgroundUpdate(HDC hdc, const RECT* updateRect)
{
	if(!isReleasing)
	{
		CComPtr<IViewObject> view = viewObjectEx != NULL ? viewObjectEx : viewObject;
	
		RECT r = {0};
		GetClientRect(getWindow(), &r);
	
		CComQIPtr<IAveMultiMonitorHelper> mmHelper(host);
		if(mmHelper != NULL)
		{
			mmHelper->GetMonitorRect(0, &r, TRUE);
		}
	
		if(view != NULL)
			HRESULT hRes = OleDraw(view, DVASPECT_CONTENT, hdc, &r);
	}

	return S_OK;
}

// Called before the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectFlashHost::OnBeforeIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectFlashHost::OnAfterIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene needs to be rendered when in D3D mode.
// Set pHasRendered to TRUE if the scene has already been rendered by
// this method. Setting pHasRendered to TRUE allows an Effect to
// modify the complete render-cycle.
STDMETHODIMP CDesktopEffectFlashHost::OnD3DRender(BOOL* pHasRendered)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D render cycle is started, if it has not been taken
// over by OnD3DRender().
// Set pBgColor to the background desired color of the scene.
STDMETHODIMP CDesktopEffectFlashHost::OnBeforeD3DRender(DWORD* pBgColor)
{
	if(NULL == pBgColor)
		return E_INVALIDARG;

	// Clear with solid red.
	*pBgColor = 0xFFFF0000;

	return S_OK;
}

// Called before the D3D scene is started.
STDMETHODIMP CDesktopEffectFlashHost::OnBeforeD3DSceneStart(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is started.
// This is the place for this Effect to draw its own elements into the
// scene.
// By default, the Device is in a 2D render state with a quad VertexBuffer
// that is the size of the desktop, with direct mapping texture coords.
STDMETHODIMP CDesktopEffectFlashHost::OnD3DSceneStart(void)
{
	// TODO: Add your implementation code here
	return S_OK;
}

// Called before the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectFlashHost::OnBeforeIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectFlashHost::OnAfterIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene has ended.
STDMETHODIMP CDesktopEffectFlashHost::OnD3DSceneEnd(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectFlashHost::OnBeforeD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectFlashHost::OnAfterD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the whole D3D render loop has been finished.
STDMETHODIMP CDesktopEffectFlashHost::OnAfterD3DRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

BOOL CDesktopEffectFlashHost::IsOnListView(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WCHAR className[1024] = {0};
	GetClassName(hwnd, className, 1024);
	BOOL isListView = _wcsicmp(className, L"SysListView32") == 0;
	if(!isListView)
		return FALSE;


	POINT ptMouse = {0};
	if(msg != WM_SETCURSOR)
	{
		ptMouse.x = GET_X_LPARAM(lParam); 
		ptMouse.y = GET_Y_LPARAM(lParam); 
	}
	else
	{
		DWORD pos = GetMessagePos();
		ptMouse.x = GET_X_LPARAM(pos); 
		ptMouse.y = GET_Y_LPARAM(pos); 
		ScreenToClient(hwnd, &ptMouse);
	}

	LVHITTESTINFO hit = {0};
	hit.pt = ptMouse;
	SendMessage(hwnd, LVM_HITTEST, 0, (LPARAM)&hit);

	return !(hit.flags & LVHT_NOWHERE);
}

BOOL CDesktopEffectFlashHost::IsOnObject(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(NULL == viewObject)
		return FALSE;

	POINT ptMouse = {0};
	if(msg != WM_SETCURSOR)
	{
		ptMouse.x = GET_X_LPARAM(lParam); 
		ptMouse.y = GET_Y_LPARAM(lParam); 
	}
	else
	{
		DWORD pos = GetMessagePos();
		ptMouse.x = GET_X_LPARAM(pos); 
		ptMouse.y = GET_Y_LPARAM(pos); 
		ScreenToClient(hwnd, &ptMouse);
	}

	RECT r = getOutputRect();
	DWORD dwRes = 0;
	HRESULT hRes = viewObject->QueryHitPoint(DVASPECT_CONTENT, &r, ptMouse, 1, &dwRes);
	return SUCCEEDED(hRes) && HITRESULT_HIT == dwRes;
}

// Called when the target window (the desktops listview often) receives a message.
// set lResult to the desired return value of the MsgProc and set bHandled to TRUE
// if this message should not be passed to the original MsgProc of the target window.
// If mouse interactivity is needed, this is the place to track mouse movements for example
// by monitorin WM_MOUSEMOVE.
STDMETHODIMP CDesktopEffectFlashHost::OnTargetWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	if(isReleasing)
		return S_OK;

	BOOL routeToObject=FALSE;

	if(IsMouseMessage(msg))
	{
		if(hasCapture)
			routeToObject = TRUE;
		else if(IsOnListView(hwnd, msg, wParam, lParam))
			routeToObject = FALSE;
		else if(IsOnObject(hwnd, msg, wParam, lParam))
			routeToObject = TRUE;
		else
			routeToObject = FALSE;
	}
	else if(IsKeyBoardMessage(msg))
	{
		if(hasFocus)
			routeToObject = TRUE;
		else
			routeToObject = FALSE;
	}

	if(routeToObject)
	{
		LRESULT res;
		if(inPlaceObjectWindowLess != NULL)
		{
			if(msg == WM_SETCURSOR)
				msg = msg;

			HRESULT hRes = inPlaceObjectWindowLess->OnWindowMessage(msg, wParam, lParam, &res);
			if(hRes == S_OK || msg == WM_SETCURSOR)
				*bHandled = TRUE;

			// FIX: TEMP FIX for Release() called on WM_RBUTTONDOWN
			if(msg == WM_RBUTTONDOWN && hRes == S_OK)
				AddRef();
		}
	}

	return S_OK;
}

// Called when a configuration window is needed for this effect.
// Set hwnd to the configuration window, parent should be set as the parent of the
// the configuration window.
// 
// DoesSupport() must signal AVE_SUPPORTS_CONFIG to the host in order for this
// method to get called.
//
// The configuration window must be prepared to run inside another window, thus
// should not have borders.
//
// WM_AVE_EFFECTCONFIG_OK will be sent to this window when an OK button is pressed
// in the parent of this configuration window.
STDMETHODIMP CDesktopEffectFlashHost::GetConfigurationWindow(HWND* hwnd, HWND parent)
{
	if(NULL == hwnd)
		return E_INVALIDARG;

	dlg.Create(parent);
	*hwnd = dlg.m_hWnd;

	return S_OK;
}

// Called when there is a notification for the Effect.
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectFlashHost::OnNotification(DWORD dwNotification)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the host needs to find out what the Effect Supports.
// Set pFlag to a combination of the following:
//	+ AVE_SUPPORTS_CONFIG	if a config window is supported
STDMETHODIMP CDesktopEffectFlashHost::DoesSupport(DWORD* pFlag)
{
	if(NULL == pFlag)
		return E_INVALIDARG;

	// Todo: add your supported flags here
	// (AVE_SUPPORTS_CONFIG for Config window)
	*pFlag = AVE_SUPPORTS_D3D_DRAW_OVER_REAL_WALLPAPER | AVE_SUPPORTS_CONFIG;

	return S_OK;
}

STDMETHODIMP CDesktopEffectFlashHost::OnNotificationWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	return S_OK;
}