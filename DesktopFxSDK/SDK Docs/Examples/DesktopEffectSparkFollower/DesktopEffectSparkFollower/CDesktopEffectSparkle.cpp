// CDesktopEffectSparkle.cpp : Implementation of CCDesktopEffectSparkle

#include "stdafx.h"
#include "CDesktopEffectSparkle.h"


// CCDesktopEffectSparkle


// Called when the name of this effect is required
STDMETHODIMP CCDesktopEffectSparkle::GetName(BSTR* pName)
{
	if(NULL == pName)
		return E_INVALIDARG;

	CString name;
	name.LoadString(IDS_EFFECTNAME);
	*pName = SysAllocString(name);

	return S_OK;
}

// Called when the author of this effect is required
STDMETHODIMP CCDesktopEffectSparkle::GetAuthor(BSTR* pAuthor)
{
	if(NULL == pAuthor)
		return E_INVALIDARG;

	CString author;
	author.LoadString(IDS_AUTHORNAME);
	*pAuthor = SysAllocString(author);

	return S_OK;
}

// Called when the description of this effect is required
STDMETHODIMP CCDesktopEffectSparkle::GetDescription(BSTR* pDescription)
{
	if(NULL == pDescription)
		return E_INVALIDARG;

	CString description;
	description.LoadString(IDS_DESCRIPTION);
	*pDescription = SysAllocString(description);

	return S_OK;
}


Gdiplus::Bitmap* LoadBitmapResource(HINSTANCE instance, LPCTSTR section, LPCTSTR resname) 
{  
 HRSRC res = FindResource(instance, resname, section); 
 DWORD err = GetLastError(); 
 if (res == NULL) { return NULL;} 
 HGLOBAL hglob = LoadResource(instance, res); 
 DWORD rSize = SizeofResource(instance, res); 
 if (hglob == NULL) { return NULL;} 
  LPVOID lockMem = LockResource(hglob); 
 HGLOBAL copyGlob = GlobalAlloc(GMEM_MOVEABLE, rSize); 
 if (copyGlob == NULL) return NULL; 
 LPVOID copyMem = GlobalLock(copyGlob); 
 memcpy(copyMem, lockMem, rSize); 
 GlobalUnlock(copyGlob); 
 IStream *stream; 
 CreateStreamOnHGlobal(copyGlob, true, &stream); 
 if (stream == NULL) {return NULL;} 
 // we are g for go 
 Bitmap *bmp = Bitmap::FromStream(stream); 
 stream->Release(); 
 GlobalUnlock(copyGlob); 
 GlobalFree(copyGlob); 
 UnlockResource(hglob); 
 return bmp; 
}



// Called when an icon of this effect is required
STDMETHODIMP CCDesktopEffectSparkle::GetIcon(HBITMAP* pHBitmap)
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
STDMETHODIMP CCDesktopEffectSparkle::GetCooperationFlags(DWORD* pFlags)
{
	// NOT IMPLEMENTED CURRENTLY

	return S_OK;
}

// Called when this effect is loaded and initialized.
STDMETHODIMP CCDesktopEffectSparkle::OnInitialize(IAveDesktopEffectsHost* host)
{
	// Store a ptr to our host
	this->host = host;
	host->AddRef();

	HWND hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
	{
	}

	return S_OK;
}

// Called when this effect is unloaded
STDMETHODIMP CCDesktopEffectSparkle::OnUninitialize(void)
{

	// Release our ptr to our host
	host->Release();

	return S_OK;
}


// Called when DesktopEffects rendering is started
STDMETHODIMP CCDesktopEffectSparkle::OnStart(void)
{
	// D3D example:
	host->StartD3DMode();

	return S_OK;
}

// Called when DesktopEffects rendering is stopped
STDMETHODIMP CCDesktopEffectSparkle::OnStop(void)
{
	
	//D3D example:
	if(directDevice != NULL)
		host->StopD3DMode();

	return S_OK;
}

// Called when DesktopEffects is paused
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CCDesktopEffectSparkle::OnPause(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when DesktopEffects is resumed from a paused state
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CCDesktopEffectSparkle::OnResume(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called when D3D mode is started. Gives a Pointer to an IUnknown that implements
// IDirect3DDevice9.
STDMETHODIMP CCDesktopEffectSparkle::OnD3DStart(IUnknown* directDeviceAsUnknown)
{
	// D3D example:
	if(NULL == directDeviceAsUnknown)
		return E_INVALIDARG;
	
	HWND hwnd = NULL;
	host->GetTargetWindow(&hwnd);
	if(hwnd != NULL)
	{
		RECT rc = {0};
		GetClientRect(hwnd, &rc);
		sceneSize.cx = rc.right - rc.left;
		sceneSize.cy = rc.bottom - rc.top;
	}

	CLSID clsid = {0};
	CLSIDFromString(L"{D0223B96-BF7A-43fd-92BD-A43B0D82B9EB}", &clsid);
	directDeviceAsUnknown->QueryInterface(clsid, (void**)&directDevice);
	if(NULL == directDevice)
		return E_INVALIDARG;

	D3DXCreateTextureFromResourceEx(directDevice, ATL::_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP1), 0, 0, 1, 0, D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR, 0, 0, NULL, NULL, &particle);	

	if(NULL == particle)
		MessageBox(0, L"err", 0, 0);
		
	system.textureIsAlphaBlended = FALSE;
	system.SetPos(300.0f, 0.0f);
	system.Begin(250);

	hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
		SetTimer(hwnd, 1001, 20, 0);

	return S_OK;
}

// Called when D3D mode is stopped.
STDMETHODIMP CCDesktopEffectSparkle::OnD3DStop(void)
{
	HWND hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
		KillTimer(hwnd, 1001);

	if(particle != NULL)
		particle->Release();
	particle = NULL;


	// D3D example:
	if(directDevice != NULL)
		directDevice->Release();
	directDevice = NULL;

	sceneSize.cx = 0;
	sceneSize.cy = 0;

	return S_OK;
}

// Called when the GDI background is updated.
// The GDI hdc and updateRect are passed, and pWhatToDo must be set to one of the following:
//	- AVE_GDI_BACKGROUND_PAINT_DO_NOT_OVERRIDE, let the normal background be painted
//	- AVE_GDI_BACKGROUND_PAINT_DO_NOT_CLEAR, do not clear the background with solid black.
//	- AVE_GDI_BACKGROUND_PAINT_DO_CLEAR, clear the background with solid black.
//	- AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG, let the normal background be painted and call 
//												OnAfterGdiBackgroundUpdate()
STDMETHODIMP CCDesktopEffectSparkle::OnBeforeGdiBackgroundUpdate(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo)
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
STDMETHODIMP CCDesktopEffectSparkle::OnAfterGdiBackgroundUpdate(HDC hdc, const RECT* updateRect)
{
	if(NULL == updateRect || NULL == hdc)
		return E_FAIL;

	return S_OK;
}

// Called before the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CCDesktopEffectSparkle::OnBeforeIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CCDesktopEffectSparkle::OnAfterIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene needs to be rendered when in D3D mode.
// Set pHasRendered to TRUE if the scene has already been rendered by
// this method. Setting pHasRendered to TRUE allows an Effect to
// modify the complete render-cycle.
STDMETHODIMP CCDesktopEffectSparkle::OnD3DRender(BOOL* pHasRendered)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D render cycle is started, if it has not been taken
// over by OnD3DRender().
// Set pBgColor to the background desired color of the scene.
STDMETHODIMP CCDesktopEffectSparkle::OnBeforeD3DRender(DWORD* pBgColor)
{
	if(NULL == pBgColor)
		return E_INVALIDARG;

	// Clear with solid black.
	//*pBgColor = 0xFF000000;

	return S_OK;
}

// Called before the D3D scene is started.
STDMETHODIMP CCDesktopEffectSparkle::OnBeforeD3DSceneStart(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called after the D3D scene is started.
// This is the place for this Effect to draw its own elements into the
// scene.
// By default, the Device is in a 2D render state with a quad VertexBuffer
// that is the size of the desktop, with direct mapping texture coords.
STDMETHODIMP CCDesktopEffectSparkle::OnD3DSceneStart(void)
{

	D3DXMATRIX Projection, View, World;
	D3DXMatrixOrthoLH(&Projection, float(sceneSize.cx), -float(sceneSize.cy), 0.0f, 1.0f);
	D3DXMatrixIdentity(&View);
	D3DXMatrixIdentity(&World);

	directDevice->SetTransform(D3DTS_PROJECTION, &Projection);
	directDevice->SetTransform(D3DTS_WORLD, &World);
	directDevice->SetTransform(D3DTS_VIEW, &View);


	system.Update();
	directDevice->SetTexture( 0, particle );
	system.Render(directDevice);

	return S_OK;
}

// Called before the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CCDesktopEffectSparkle::OnBeforeIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CCDesktopEffectSparkle::OnAfterIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene has ended.
STDMETHODIMP CCDesktopEffectSparkle::OnD3DSceneEnd(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D scene is present()'d to the device.
STDMETHODIMP CCDesktopEffectSparkle::OnBeforeD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is present()'d to the device.
STDMETHODIMP CCDesktopEffectSparkle::OnAfterD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the whole D3D render loop has been finished.
STDMETHODIMP CCDesktopEffectSparkle::OnAfterD3DRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called when there is a message for the notification window.
// Each effect gets its own notification helper window for use with timers,
// thread marshalling and such.
// By calling IAveDesktopEffectsHost::GetHelperWindow(this, &hwnd) this 
// window will be created and a handle to it will be obtained.
STDMETHODIMP CCDesktopEffectSparkle::OnNotificationWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	if(WM_TIMER == msg && 1001 == wParam)
	{
		*bHandled = TRUE;
		
		host->D3DRender();	
	}

	return S_OK;
}

// Called when the target window (the desktops listview often) receives a message.
// set lResult to the desired return value of the MsgProc and set bHandled to TRUE
// if this message should not be passed to the original MsgProc of the target window.
// If mouse interactivity is needed, this is the place to track mouse movements for example
// by monitorin WM_MOUSEMOVE.
STDMETHODIMP CCDesktopEffectSparkle::OnTargetWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	
	if(WM_MOUSEMOVE == msg && directDevice != NULL)
	{
		POINT ptPrev = ptMouse;
		ptMouse.x = GET_X_LPARAM(lParam);
		ptMouse.y = GET_Y_LPARAM(lParam);

		float fx = float(ptMouse.x - sceneSize.cx  / 2);
		float fy = float(ptMouse.y - sceneSize.cy  / 2);
		
		int xdiff = ptMouse.x - ptPrev.x;
		int ydiff = ptMouse.y - ptPrev.y;
		ptPrev.x -= sceneSize.cx  / 2;
		ptPrev.y -= sceneSize.cy  / 2;
		int diff = max(abs(xdiff), abs(ydiff));
		int numNeeded = diff / 2;
		float fax = float(ptPrev.x);
		float fay = float(ptPrev.y);
		for(int i=0; i < numNeeded-1; ++i)
		{
			fax += float(xdiff) / float(numNeeded);
			fay += float(ydiff) / float(numNeeded);

			system.AddSpark(fax, fay, -0.5f - (system.rnd() * 0.4f));
			system.AddSpark(fax, fay, -0.5f - (system.rnd() * 0.4f), 0.15f);
			system.AddSpark(fax, fay, -0.2f - (system.rnd() * 0.4f), 0.15f);
		}
		
		//system.AddSpark(fx, fy);
		//system.AddSpark(fx, fy);

		system.SetPos(fx, fy);

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
STDMETHODIMP CCDesktopEffectSparkle::GetConfigurationWindow(HWND* hwnd, HWND parent)
{
	if(NULL == hwnd)
		return E_INVALIDARG;

	// TODO: create a window that will be embedded into another window
	// here for configuration.

	return S_OK;
}

// Called when there is a notification for the Effect.
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CCDesktopEffectSparkle::OnNotification(DWORD dwNotification)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CCDesktopEffectSparkle::DoesSupport(DWORD* pFlag)
{
	if(NULL == pFlag)
		return E_INVALIDARG;

	*pFlag = AVE_SUPPORTS_D3D_DRAW_OVER_REAL_WALLPAPER;//AVE_SUPPORTS_CONFIG;

	return S_OK;
}
