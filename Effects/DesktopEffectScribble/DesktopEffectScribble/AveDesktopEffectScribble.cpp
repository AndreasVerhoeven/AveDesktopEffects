// AveDesktopEffectScribble.cpp : Implementation of CAveDesktopEffectScribbleScribble

#include "stdafx.h"
#include "AveDesktopEffectScribble.h"


// CAveDesktopEffectScribbleScribble

// AveDesktopEffect.cpp : Implementation of CAveDesktopEffectScribble

#include "stdafx.h"


// Called when the name of this effect is required
STDMETHODIMP CAveDesktopEffectScribble::GetName(BSTR* pName)
{
	if(NULL == pName)
		return E_INVALIDARG;

	CString name;
	name.LoadString(IDS_EFFECTNAME);
	*pName = SysAllocString(name);

	return S_OK;
}

// Called when the author of this effect is required
STDMETHODIMP CAveDesktopEffectScribble::GetAuthor(BSTR* pAuthor)
{
	if(NULL == pAuthor)
		return E_INVALIDARG;

	CString author;
	author.LoadString(IDS_AUTHORNAME);
	*pAuthor = SysAllocString(author);

	return S_OK;
}

// Called when the description of this effect is required
STDMETHODIMP CAveDesktopEffectScribble::GetDescription(BSTR* pDescription)
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
STDMETHODIMP CAveDesktopEffectScribble::GetIcon(HBITMAP* pHBitmap)
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
STDMETHODIMP CAveDesktopEffectScribble::GetCooperationFlags(DWORD* pFlags)
{
	// NOT IMPLEMENTED CURRENTLY

	return S_OK;
}

// Called when this effect is loaded and initialized.
STDMETHODIMP CAveDesktopEffectScribble::OnInitialize(IAveDesktopEffectsHost* host)
{
	// Store a ptr to our host
	this->host = host;
	host->AddRef();

	return S_OK;
}

// Called when this effect is unloaded
STDMETHODIMP CAveDesktopEffectScribble::OnUninitialize(void)
{
	_DestroyScribbleBuffer();

	// Release our ptr to our host
	host->Release();

	return S_OK;
}

HRESULT CAveDesktopEffectScribble::_DestroyScribbleBuffer()
{
	HRESULT hRes = S_OK;

	if(bufferBitmap != NULL)
		DeleteObject(bufferBitmap);

	if(bufferDC != NULL)
		DeleteDC(bufferDC);

	bufferDC = NULL;
	bufferBitmap = NULL;
	bufferBits = NULL;
	bufferSize.cx = 0;
	bufferSize.cy = 0;
	return hRes;
}

HRESULT CAveDesktopEffectScribble::_CreateScribbleBuffer()
{
	HRESULT hRes = S_OK;

	_DestroyScribbleBuffer();

	HWND hwnd = NULL;
	host->GetTargetWindow(&hwnd);
	if(NULL == hwnd)
		return E_FAIL;

	RECT rc = {0};
	GetClientRect(hwnd, &rc);
	bufferSize.cx = rc.right - rc.left;
	bufferSize.cy = rc.bottom - rc.top;

	bufferDC = CreateCompatibleDC(NULL);
	if(NULL == bufferDC)
		return E_FAIL;

	BITMAPV5HEADER bi	= {0};
	bi.bV5Size			= sizeof(BITMAPV5HEADER);
    bi.bV5Width			= bufferSize.cx;
    bi.bV5Height		= -bufferSize.cy;
    bi.bV5Planes		= 1;
    bi.bV5BitCount		= 32;
    bi.bV5Compression	= BI_BITFIELDS;
    bi.bV5RedMask		= 0x00FF0000;
    bi.bV5GreenMask		= 0x0000FF00;
    bi.bV5BlueMask		= 0x000000FF;
    bi.bV5AlphaMask		= 0xFF000000; 

	bufferBitmap = CreateDIBSection(bufferDC, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&bufferBits, NULL, 0);
	if(NULL == bufferBitmap)
	{
		_DestroyScribbleBuffer();
		return E_FAIL;
	}

	SelectObject(bufferDC, bufferBitmap);

	return S_OK;
}

// Called when DesktopEffects rendering is started
STDMETHODIMP CAveDesktopEffectScribble::OnStart(void)
{
	// D3D example:
	//host->StartD3DMode();

	_CreateScribbleBuffer();

	return S_OK;
}

// Called when DesktopEffects rendering is stopped
STDMETHODIMP CAveDesktopEffectScribble::OnStop(void)
{
	_DestroyScribbleBuffer();
	
	// D3D example:
	//if(directDevice != NULL)
	//	host->StopD3DMode();

	return S_OK;
}

// Called when DesktopEffects is paused
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CAveDesktopEffectScribble::OnPause(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when DesktopEffects is resumed from a paused state
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CAveDesktopEffectScribble::OnResume(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called when D3D mode is started. Gives a Pointer to an IUnknown that implements
// IDirect3DDevice9.
STDMETHODIMP CAveDesktopEffectScribble::OnD3DStart(IUnknown* directDeviceAsUnknown)
{
	// D3D example:
	//if(NULL == directDeviceAsUnknown)
	//	return E_INVALIDARG;
	//
	//directDeviceAsUnknown->QueryInterface(IID_IDirect3DDevice9, (void**)&directDevice);
	//if(NULL == directDevice)
	//	return E_INVALIDARG;


	return S_OK;
}

// Called when D3D mode is stopped.
STDMETHODIMP CAveDesktopEffectScribble::OnD3DStop(void)
{
	// D3D example:
	//if(directDevice != NULL)
	//	directDevice->Release();
	//directDevice = NULL;

	return S_OK;
}

// Called when the GDI background is updated.
// The GDI hdc and updateRect are passed, and pWhatToDo must be set to one of the following:
//	- AVE_GDI_BACKGROUND_PAINT_DO_NOT_OVERRIDE, let the normal background be painted
//	- AVE_GDI_BACKGROUND_PAINT_DO_NOT_CLEAR, do not clear the background with solid black.
//	- AVE_GDI_BACKGROUND_PAINT_DO_CLEAR, clear the background with solid black.
//	- AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG, let the normal background be painted and call 
//												OnAfterGdiBackgroundUpdate()
STDMETHODIMP CAveDesktopEffectScribble::OnBeforeGdiBackgroundUpdate(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo)
{
	//*pWhatToDo = AVE_GDI_BACKGROUND_PAINT_DO_NOT_CLEAR;
	*pWhatToDo = AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG;

	//HBRUSH brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//FillRect(hdc, updateRect, brush);

	return S_OK;
}

// Will only be called when pWhatToDo was set to AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG in 
// OnBeforeGdiBackgroundUpdate after the normal background has been painted.
// Allows to paint on top of the background.
// The GDI hdc and update are passed.
STDMETHODIMP CAveDesktopEffectScribble::OnAfterGdiBackgroundUpdate(HDC hdc, const RECT* updateRect)
{
	if(NULL == updateRect || NULL == hdc)
		return E_FAIL;

	int l = updateRect->left;
	int t = updateRect->top;
	int w = updateRect->right - updateRect->left;
	int h = updateRect->bottom - updateRect->top;

	SelectObject(bufferDC, bufferBitmap);
	BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
	AlphaBlend(hdc, l, t, w, h, bufferDC, l, t, w, h, bf);
	/*
	RECT rcMouse = {ptMouse.x - 10, ptMouse.y - 10, ptMouse.x + 10, ptMouse.y + 10};
	
	RECT intersection = {0};
	if(IntersectRect(&intersection, &rcMouse, updateRect))
	{
		HBRUSH brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		FillRect(hdc, &intersection, brush);
	}*/

	return S_OK;
}

// Called before the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CAveDesktopEffectScribble::OnBeforeIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CAveDesktopEffectScribble::OnAfterIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene needs to be rendered when in D3D mode.
// Set pHasRendered to TRUE if the scene has already been rendered by
// this method. Setting pHasRendered to TRUE allows an Effect to
// modify the complete render-cycle.
STDMETHODIMP CAveDesktopEffectScribble::OnD3DRender(BOOL* pHasRendered)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D render cycle is started, if it has not been taken
// over by OnD3DRender().
// Set pBgColor to the background desired color of the scene.
STDMETHODIMP CAveDesktopEffectScribble::OnBeforeD3DRender(DWORD* pBgColor)
{
	if(NULL == pBgColor)
		return E_INVALIDARG;

	// Clear with solid black.
	//*pBgColor = 0xFF000000;

	return S_OK;
}

// Called before the D3D scene is started.
STDMETHODIMP CAveDesktopEffectScribble::OnBeforeD3DSceneStart(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is started.
// This is the place for this Effect to draw its own elements into the
// scene.
// By default, the Device is in a 2D render state with a quad VertexBuffer
// that is the size of the desktop, with direct mapping texture coords.
STDMETHODIMP CAveDesktopEffectScribble::OnD3DSceneStart(void)
{
	// TODO: Add your implementation code here
	return S_OK;
}

// Called before the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CAveDesktopEffectScribble::OnBeforeIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CAveDesktopEffectScribble::OnAfterIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene has ended.
STDMETHODIMP CAveDesktopEffectScribble::OnD3DSceneEnd(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D scene is present()'d to the device.
STDMETHODIMP CAveDesktopEffectScribble::OnBeforeD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is present()'d to the device.
STDMETHODIMP CAveDesktopEffectScribble::OnAfterD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the whole D3D render loop has been finished.
STDMETHODIMP CAveDesktopEffectScribble::OnAfterD3DRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the target window (the desktops listview often) receives a message.
// set lResult to the desired return value of the MsgProc and set bHandled to TRUE
// if this message should not be passed to the original MsgProc of the target window.
// If mouse interactivity is needed, this is the place to track mouse movements for example
// by monitorin WM_MOUSEMOVE.
STDMETHODIMP CAveDesktopEffectScribble::OnTargetWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	if(WM_LBUTTONDOWN == msg)
	{
		POINT ptMouse = {0};
		ptMouse.x = GET_X_LPARAM(lParam); 
		ptMouse.y = GET_Y_LPARAM(lParam); 

		LVHITTESTINFO hit = {0};
		hit.pt = ptMouse;
		SendMessage(hwnd, LVM_HITTEST, 0, (LPARAM)&hit);
		if(LVHT_NOWHERE & hit.flags)
		{
			prevPoint = hit.pt;
			*bHandled = TRUE;
			mouseIsDown = TRUE;
			SetCapture(hwnd);
			LPARAM newLParam = MAKELPARAM(hit.pt.x+1, hit.pt.y+1);
			PostMessage(hwnd, WM_MOUSEMOVE, wParam, newLParam);
		}
	}
	else if(WM_CONTEXTMENU == msg)
	{
		if(mouseIsDown)
		{
			*bHandled = TRUE;
			mouseIsDown = FALSE;
		}
	}
	else if(WM_LBUTTONUP == msg)
	{
		if(mouseIsDown)
		{
			*bHandled = TRUE;
			mouseIsDown = FALSE;
			ReleaseCapture();
		}
	}
	else if(WM_MOUSEMOVE == msg && mouseIsDown)
	{
		*bHandled = TRUE;

		POINT ptMouse = {0};
		ptMouse.x = GET_X_LPARAM(lParam); 
		ptMouse.y = GET_Y_LPARAM(lParam); 

		Pen pen(Color(brushColor), (float)brushSize);
		pen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
		Rect pointRect(ptMouse.x - brushSize/2, ptMouse.y - brushSize/2, brushSize, brushSize);
		Graphics g(bufferDC);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		g.DrawLine(&pen, prevPoint.x, prevPoint.y, ptMouse.x, ptMouse.y);


		GraphicsPath path;
		path.AddLine(prevPoint.x, prevPoint.y, ptMouse.x, ptMouse.y);
		Rect pathRect;
		path.GetBounds(&pathRect, NULL, &pen);
		RECT invalRect = {pathRect.X, pathRect.Y, pathRect.X + pathRect.Width, pathRect.Y + pathRect.Height};
		InvalidateRect(hwnd, &invalRect, TRUE);

		prevPoint = ptMouse;
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
STDMETHODIMP CAveDesktopEffectScribble::GetConfigurationWindow(HWND* hwnd, HWND parent)
{
	if(NULL == hwnd)
		return E_INVALIDARG;

	// TODO: create a window that will be embedded into another window
	// here for configuration.

	return S_OK;
}

// Called when there is a notification for the Effect.
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CAveDesktopEffectScribble::OnNotification(DWORD dwNotification)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectScribble::DoesSupport(DWORD* pFlag)
{
	if(NULL == pFlag)
		return E_INVALIDARG;

	*pFlag = 0;//AVE_SUPPORTS_CONFIG;

	return S_OK;
}
