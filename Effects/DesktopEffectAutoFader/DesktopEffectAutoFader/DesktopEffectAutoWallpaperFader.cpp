// DesktopEffectAutoWallpaperFader.cpp : Implementation of CDesktopEffectAutoWallpaperFader

#include "stdafx.h"
#include "DesktopEffectAutoWallpaperFader.h"


// CDesktopEffectAutoWallpaperFader


// Called when the name of this effect is required
STDMETHODIMP CDesktopEffectAutoWallpaperFader::GetName(BSTR* pName)
{
	if(NULL == pName)
		return E_INVALIDARG;

	CString name;
	name.LoadString(IDS_EFFECTNAME);
	*pName = SysAllocString(name);

	return S_OK;
}

// Called when the author of this effect is required
STDMETHODIMP CDesktopEffectAutoWallpaperFader::GetAuthor(BSTR* pAuthor)
{
	if(NULL == pAuthor)
		return E_INVALIDARG;

	CString author;
	author.LoadString(IDS_AUTHORNAME);
	*pAuthor = SysAllocString(author);

	return S_OK;
}

// Called when the description of this effect is required
STDMETHODIMP CDesktopEffectAutoWallpaperFader::GetDescription(BSTR* pDescription)
{
	if(NULL == pDescription)
		return E_INVALIDARG;

	CString description;
	description.LoadString(IDS_DESCRIPTION);
	*pDescription = SysAllocString(description);

	return S_OK;
}


Gdiplus::Bitmap* LoadBitmapResource(HINSTANCE instance, LPCTSTR section, LPCTSTR resName) 
{  
	HRSRC res = FindResource(instance, resName, section); 
	if(NULL == res)
		return NULL;

	HGLOBAL hglob = LoadResource(instance, res); 
	DWORD rSize = SizeofResource(instance, res); 
	if(NULL == hglob)
		return NULL;

	LPVOID lockMem = LockResource(hglob); 
	HGLOBAL copyGlob = GlobalAlloc(GMEM_MOVEABLE, rSize); 
	if(NULL == copyGlob)
		return NULL;

	LPVOID copyMem = GlobalLock(copyGlob); 
	memcpy(copyMem, lockMem, rSize); 
	GlobalUnlock(copyGlob); 
	IStream* stream = NULL; 
	CreateStreamOnHGlobal(copyGlob, true, &stream); 
	if(NULL == stream)
		return NULL;

	Bitmap *bmp = Bitmap::FromStream(stream); 
	stream->Release(); 
	GlobalUnlock(copyGlob); 
	GlobalFree(copyGlob); 
	UnlockResource(hglob); 
	return bmp; 
}



// Called when an icon of this effect is required
STDMETHODIMP CDesktopEffectAutoWallpaperFader::GetIcon(HBITMAP* pHBitmap)
{
	// TODO: Load 64x 64 ARGB Bitmap that serves as an icon

	Bitmap* bmp = LoadBitmapResource(ATL::_AtlBaseModule.GetResourceInstance(), L"PNG", MAKEINTRESOURCE(IDR_PNG1));
	if(bmp != NULL)
	{
		bmp->GetHBITMAP(NULL, pHBitmap);
		delete bmp;
	}

	return S_OK;
}

// Called when the cooperation flags are needed.
// NOT IMPLEMENTED CURRENTLY
STDMETHODIMP CDesktopEffectAutoWallpaperFader::GetCooperationFlags(DWORD* pFlags)
{
	// NOT IMPLEMENTED CURRENTLY

	return S_OK;
}

// Called when this effect is loaded and initialized.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnInitialize(IAveDesktopEffectsHost* host)
{
	// Store a ptr to our host
	this->host = host;
	host->AddRef();
	return S_OK;
}

// Called when this effect is unloaded
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnUninitialize(void)
{

	// Release our ptr to our host
	host->Release();

	return S_OK;
}


// Called when DesktopEffects rendering is started
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnStart(void)
{
	// D3D example:
	host->StartD3DMode();

	return S_OK;
}

// Called when DesktopEffects rendering is stopped
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnStop(void)
{
	
	//D3D example:
	if(directDevice != NULL)
		host->StopD3DMode();

	return S_OK;
}

// Called when DesktopEffects is paused
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnPause(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when DesktopEffects is resumed from a paused state
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnResume(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called when D3D mode is started. Gives a Pointer to an IUnknown that implements
// IDirect3DDevice9.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnD3DStart(IUnknown* directDeviceAsUnknown)
{
	// D3D example:
	if(NULL == directDeviceAsUnknown)
		return E_INVALIDARG;
	
	HWND hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);

	//CLSID clsid = {0};
	//CLSIDFromString(L"{D0223B96-BF7A-43fd-92BD-A43B0D82B9EB}", &clsid);
	directDeviceAsUnknown->QueryInterface(IID_IDirect3DDevice9, (void**)&directDevice);
	if(NULL == directDevice)
		return E_INVALIDARG;

	return S_OK;
}

// Called when D3D mode is stopped.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnD3DStop(void)
{
	HWND hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
	{
		KillTimer(hwnd, 555);
		KillTimer(hwnd, 777);
	}

	oldWallpaper.Release();
	newWallpaper.Release();



	// D3D example:
	if(directDevice != NULL)
		directDevice->Release();
	directDevice = NULL;

	return S_OK;
}

// Called when the GDI background is updated.
// The GDI hdc and updateRect are passed, and pWhatToDo must be set to one of the following:
//	- AVE_GDI_BACKGROUND_PAINT_DO_NOT_OVERRIDE, let the normal background be painted
//	- AVE_GDI_BACKGROUND_PAINT_DO_NOT_CLEAR, do not clear the background with solid black.
//	- AVE_GDI_BACKGROUND_PAINT_DO_CLEAR, clear the background with solid black.
//	- AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG, let the normal background be painted and call 
//												OnAfterGdiBackgroundUpdate()
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnBeforeGdiBackgroundUpdate(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo)
{
	//*pWhatToDo = AVE_GDI_BACKGROUND_PAINT_DO_NOT_CLEAR;
	//*pWhatToDo = AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG;

	//HBRUSH brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//FillRect(hdc, updateRect, brush);

	return S_OK;
}

// Will only be called when pWhatToDo was set to AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG in 
// OnBeforeGdiBackgroundUpdate after the normal background has been painted.
// Allows to paint on top of the background.
// The GDI hdc and update are passed.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnAfterGdiBackgroundUpdate(HDC hdc, const RECT* updateRect)
{
	if(NULL == updateRect || NULL == hdc)
		return E_FAIL;

	return S_OK;
}

// Called before the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnBeforeIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnAfterIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene needs to be rendered when in D3D mode.
// Set pHasRendered to TRUE if the scene has already been rendered by
// this method. Setting pHasRendered to TRUE allows an Effect to
// modify the complete render-cycle.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnD3DRender(BOOL* pHasRendered)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D render cycle is started, if it has not been taken
// over by OnD3DRender().
// Set pBgColor to the background desired color of the scene.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnBeforeD3DRender(DWORD* pBgColor)
{
	if(NULL == pBgColor)
		return E_INVALIDARG;

	// Clear with solid black.
	//*pBgColor = 0xFF000000;

	return S_OK;
}

// Called before the D3D scene is started.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnBeforeD3DSceneStart(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called after the D3D scene is started.
// This is the place for this Effect to draw its own elements into the
// scene.
// By default, the Device is in a 2D render state with a quad VertexBuffer
// that is the size of the desktop, with direct mapping texture coords.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnD3DSceneStart(void)
{
	if(newWallpaper != NULL)
	{
		directDevice->SetRenderState(D3DRS_LIGHTING, 1);
		D3DMATERIAL9 mtrl;
		ZeroMemory( &mtrl, sizeof(mtrl) );
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;

		directDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
		directDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		mtrl.Diffuse.a = mtrl.Ambient.a = sin(D3DX_PI /2.0f * fadingProgress);
		directDevice->SetMaterial( &mtrl );

		directDevice->SetTexture(0,newWallpaper);
		directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	}

	return S_OK;
}

// Called before the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnBeforeIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnAfterIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene has ended.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnD3DSceneEnd(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnBeforeD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnAfterD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the whole D3D render loop has been finished.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnAfterD3DRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the target window (the desktops listview often) receives a message.
// set lResult to the desired return value of the MsgProc and set bHandled to TRUE
// if this message should not be passed to the original MsgProc of the target window.
// If mouse interactivity is needed, this is the place to track mouse movements for example
// by monitorin WM_MOUSEMOVE.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnTargetWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	return S_OK;
}

// Called when there is a message for the notification window.
// Each effect gets its own notification helper window for use with timers,
// thread marshalling and such.
// By calling IAveDesktopEffectsHost::GetHelperWindow(this, &hwnd) this 
// window will be created and a handle to it will be obtained.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnNotificationWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	if(WM_TIMER == msg && 777 == wParam)
	{
		*bHandled = TRUE;
		KillTimer(hwnd, (UINT_PTR)wParam);
		SetTimer(hwnd, 555, 20, NULL);
		startTick = GetTickCount();
	}
	else if(WM_TIMER == msg && 555 == wParam)
	{
		*bHandled = TRUE;

		if(0 == startTick)
			startTick = GetTickCount();

		DWORD curTick = GetTickCount();
		DWORD elapsed = curTick - startTick;

		// GetTickCount() can wrap over after 49.7 days!
		if(elapsed < 0)
		{
			elapsed = 0;
			startTick = curTick;
		}

		fadingProgress = float(elapsed) / float(fadingDuration);
		if(fadingProgress > 1.0f)
		{
			KillTimer(hwnd, (UINT_PTR)wParam);
			fadingProgress = 0.0f;

			CComPtr<IUnknown> unknown;
			newWallpaper.QueryInterface(&unknown);
			host->SetWallpaperTexture(unknown);
			oldWallpaper.Release();
			newWallpaper.Release();

			oldWallpaper = NULL;
			newWallpaper = NULL;
		}

		
		
		host->D3DRender();	
	}
	return S_OK;
}

// Called when a configuration window is needed for this effect.
// Set hwnd to the configuration window, parent should be set as the parent of the
// the configuration window.
// 
// The configuration window must be prepared to run inside another window, thus
// should not have borders.
//
// WM_AVE_EFFECTCONFIG_OK will be sent to this window when an OK button is pressed
// in the parent of this configuration window.
STDMETHODIMP CDesktopEffectAutoWallpaperFader::GetConfigurationWindow(HWND* hwnd, HWND parent)
{
	if(NULL == hwnd)
		return E_INVALIDARG;

	// TODO: create a window that will be embedded into another window
	// here for configuration.

	return S_OK;
}

// Called when there is a notification for the Effect.
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectAutoWallpaperFader::OnNotification(DWORD dwNotification)
{
	LONG lRes = 0;
	if(AVE_NOTIFICATION_BEFORE_WALLPAPER_CHANGED == dwNotification)
	{
		oldWallpaper.Release();
		newWallpaper.Release();

		CComPtr<IUnknown> unknown;
		host->GetWallpaperTexture(&unknown);
		if(unknown != NULL)
			unknown.QueryInterface(&oldWallpaper);
	}
	else if(AVE_NOTIFICATION_AFTER_WALLPAPER_CHANGED == dwNotification)
	{
		CComPtr<IUnknown> unknown;
		host->GetWallpaperTexture(&unknown);
		if(unknown != NULL)
			unknown.QueryInterface(&newWallpaper);


		if(oldWallpaper != NULL)
		{
			CComPtr<IUnknown> unknown;
			oldWallpaper.QueryInterface(&unknown);
			host->SetWallpaperTexture(unknown);
		}

		HWND hwnd = NULL;
		host->GetHelperWindow(this,&hwnd);
		if(hwnd != NULL)
		{
			SetTimer(hwnd, 777, 1000, NULL);
		}
	}


	return S_OK;
}

STDMETHODIMP CDesktopEffectAutoWallpaperFader::DoesSupport(DWORD* pFlag)
{
	if(NULL == pFlag)
		return E_INVALIDARG;

	*pFlag = AVE_SUPPORTS_D3D_DRAW_OVER_REAL_WALLPAPER;//AVE_SUPPORTS_CONFIG;

	return S_OK;
}
