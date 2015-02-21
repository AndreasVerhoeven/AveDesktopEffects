// AveDesktopEffectsHost.cpp : Implementation of CAveDesktopEffectsHost

#include "stdafx.h"
#include "AveDesktopEffectsHost.h"

#include "ConfigureDlg.h"

#include "AveRegFuncs.h"

extern BOOL CheckTimeBomb();

#define D3DFVF_AVEVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct AveVertex
{
    FLOAT x, y, z;
	D3DXVECTOR3 n;
	DWORD color;
	FLOAT u, v;
};

template<typename T> inline void SafeRelease(T*& t)
{
	if(t != NULL)
	{
		t->Release();
		t = NULL;
	}
}

inline void CheckHRES(HRESULT& hRes)
{
	if(FAILED(hRes))
		throw hRes;
}

LRESULT CALLBACK NotificationWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HANDLE ptr = GetProp(hwnd, L"AvePtr");
	if(ptr != NULL)
	{
		BOOL handled = FALSE; 
		CAveDesktopEffectsHost* host = (CAveDesktopEffectsHost*)ptr;
		LRESULT lRes = host->HandleNotificationMessage(hwnd, msg, wParam, lParam, handled);
		if(handled)
			return lRes;
	}
	else if(WM_AVE_FIRE_IT_UP == msg)
	{
		CLSID clsid = {0};
		HRESULT hRes = CLSIDFromString((LPOLESTR)L"{96443122-0A33-4C5B-AEB5-7309276D3C71}", &clsid);
		if(FAILED(hRes))
			return 0;

		CComPtr<IShellExtInit> me;
		hRes = me.CoCreateInstance(clsid);
		if(me != NULL)
		{
			HRESULT hRes = me->Initialize(NULL, NULL, NULL);
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

extern HWND g_NotificationWindow;

HWND CreateNotificationWindow()
{
	if(g_NotificationWindow != NULL)
		return g_NotificationWindow;

	WNDCLASSEXW classex = {0};
	classex.lpszClassName = L"AveDesktopEffectsNotificationWindow";
	classex.hInstance = GetModuleHandle(NULL);
	classex.cbSize = sizeof(WNDCLASSEX);
	classex.lpfnWndProc = NotificationWindowProc;

	RegisterClassEx(&classex);

	g_NotificationWindow = CreateWindowExW(0, classex.lpszClassName, L"", 0, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
	return g_NotificationWindow;
}

LRESULT CALLBACK DefViewSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR dwData)
{
	if(dwData != NULL)
	{
		CAveDesktopEffectsHost* host = (CAveDesktopEffectsHost*)dwData;
		BOOL handled=FALSE;
		LRESULT lRes = host->HandleDefViewMessage(hwnd, msg, wParam, lParam, handled);
		if(handled)
			return lRes;
	}

	return DefSubclassProc(hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK ListViewSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR dwData)
{
	if(dwData != NULL)
	{
		CAveDesktopEffectsHost* host = (CAveDesktopEffectsHost*)dwData;
		BOOL handled=FALSE;
		LRESULT lRes = host->HandleListViewMessage(hwnd, msg, wParam, lParam, handled);
		if(handled)
			return lRes;
	}

	return DefSubclassProc(hwnd, msg, wParam, lParam);
}

HRESULT CAveDesktopEffectsHost::WriteRunningStatusToReg(BOOL status)
{
	WCHAR* isRunningAsString = status ? L"yes" : L"no";
	AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", L"IsRunning", isRunningAsString);
		
		/*
		RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", 				
		L"IsRunning", REG_SZ, 
		isRunningAsString, ((DWORD)wcslen(isRunningAsString) + 1)*sizeof(WCHAR));
		*/

	return S_OK;
}
void CAveDesktopEffectsHost::_CheckOsVersion()
{
	OSVERSIONINFO info = {0};
	info.dwOSVersionInfoSize  = sizeof(info);
	GetVersionEx(&info);

	if(AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", L"ForceXPMode", 0))
	{
		isRunningOnVista = FALSE;
	}
	else
	{
		isRunningOnVista = info.dwMajorVersion >= 6;
	}
}

BOOL CAveDesktopEffectsHost::_IsRunningVista()
{
	return isRunningOnVista;
}

HRESULT CAveDesktopEffectsHost::InternalInitialize()
{
	if(hasInitialized)
		return S_OK;

	AddRef();

	HRESULT hRes = S_OK;

	/*
	WCHAR currentEffectCLSID[MAX_PATH] = {0};
	DWORD type = REG_SZ;
	DWORD len = MAX_PATH * sizeof(WCHAR);
	
	
	LONG res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects",
					L"CurrentEffectCLSID", RRF_RT_REG_SZ, &type, currentEffectCLSID, &len);
	*/

	CString currentEffectCLSID  =AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", L"CurrentEffectCLSID", L"");

	if(wcslen(currentEffectCLSID) > 0)
	{
		hRes = LoadPluginByCLSID(CComBSTR(currentEffectCLSID));
	}

	/*
	WCHAR IsRunningAsString[MAX_PATH] = {0};
	type = REG_SZ;
	len = MAX_PATH * sizeof(WCHAR);
	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects",
					L"IsRunning", RRF_RT_REG_SZ, &type, IsRunningAsString, &len);
	*/
	CString IsRunningAsString = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", L"IsRunning", L"no");

	if(_wcsicmp(IsRunningAsString, L"yes") == 0)
		hRes = Start();

	hasInitialized = TRUE;

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::SetUpVertices(float panelWidth, float panelHeight)
{
	HRESULT hRes = S_OK;
	AveVertex* vertices = NULL;

	hRes = vertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&vertices), 0);
	if(FAILED(hRes))
	{
		return hRes;
	}

	vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = 0xffffffff;

	float z = 0.0f;

	//panelWidth=1.0f;
	//panelHeight=1.0f;
	float fixForBetweenPixels = -0.5f;

	// topleft
	vertices[0].x = - panelWidth / 2.0f + fixForBetweenPixels;
	vertices[0].y =  -panelHeight / 2.0f + fixForBetweenPixels;
	vertices[0].z = z;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	// topright
	vertices[1].x = panelWidth / 2.0f + fixForBetweenPixels;
	vertices[1].y = -panelHeight / 2.0f + fixForBetweenPixels;
	vertices[1].z = z;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	// bottomright
	vertices[2].x = panelWidth / 2.0f + fixForBetweenPixels;
	vertices[2].y = panelHeight / 2.0f + fixForBetweenPixels;
	vertices[2].z = z;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	// bottomleft
	vertices[3].x = - panelWidth / 2.0f + fixForBetweenPixels;
	vertices[3].y = panelHeight / 2.0f + fixForBetweenPixels;
	vertices[3].z = z;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

	vertices[0].n = vertices[1].n = vertices[2].n = vertices[3].n =  D3DXVECTOR3(0,0,-1);

	hRes = vertexBuffer->Unlock();
	if(FAILED(hRes))
	{
		return hRes;
	}

	return S_OK;
}

void DrawWallpaper(HDC dc, int cx, int cy)
{
	RECT rc = {0,0, cx, cy};
	FillRect(dc, &rc, (HBRUSH)(COLOR_DESKTOP+1));

	CString wallpaper = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"Control Panel\\Desktop", L"wallpaper", L"");
	CString tile = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"Control Panel\\Desktop", L"TileWallpaper", L"0");
	CString style = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"Control Panel\\Desktop", L"WallpaperStyle", L"2");
	if(wallpaper.GetLength() == 0)
	{
		return;
	}
	else
	{
		Graphics g(dc);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		Bitmap bmp(wallpaper);
		if(bmp.GetLastStatus() != Ok)
		{
			return;
		}
		

		int w = bmp.GetWidth();
		int h = bmp.GetHeight();

		// centre = 0
		if(tile == L"0" && style == L"0")
		{
			g.DrawImage(&bmp, Rect(cx/2 - w/2, cy/2-h/2, cx, cy), 0, 0, w, h, UnitPixel, 0, 0);
		}
		// tile
		if(tile == L"1" && style == L"0")
		{
			for(int x=0; x < cx / w + 1; ++x)
			{
				for(int y=0; y < cy / h + 1; ++y)
					g.DrawImage(&bmp, Rect(w*x, h*y, w, h), 0, 0, w, h, UnitPixel, 0, 0);
			}
		}
		// stretch
		if(tile == L"0" && style == L"2")
		{
			g.DrawImage(&bmp, Rect(0, 0, cx, cy), 0, 0, w, h, UnitPixel, 0, 0);
		}
		else
		{
			// stretch default
			g.DrawImage(&bmp, Rect(0, 0, cx, cy), 0, 0, w, h, UnitPixel, 0, 0);
		}
	}
}

HRESULT CAveDesktopEffectsHost::StartWallpaperBuffering()
{
	StopWallpaperBuffering();

	if(NULL == defView)
		return E_FAIL;



	SIZE bufSize = {0};
	RECT rc = {0};
	GetClientRect(defView, &rc);
	bufSize.cx = rc.right - rc.left;
	bufSize.cy = rc.bottom - rc.top;

	HDC tmpDC = CreateCompatibleDC(NULL);
	if(NULL == tmpDC)
		return E_FAIL;

	BITMAPV5HEADER bi	= {0};
	bi.bV5Size			= sizeof(BITMAPV5HEADER);
    bi.bV5Width			= bufSize.cx;
    bi.bV5Height		= -bufSize.cy;
    bi.bV5Planes		= 1;
    bi.bV5BitCount		= 32;
    bi.bV5Compression	= BI_BITFIELDS;
    bi.bV5RedMask		= 0x00FF0000;
    bi.bV5GreenMask		= 0x0000FF00;
    bi.bV5BlueMask		= 0x000000FF;
    bi.bV5AlphaMask		= 0x00000000;

	LPVOID tmpBits = NULL;
	HBITMAP tmpBmp = CreateDIBSection(tmpDC, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&tmpBits, NULL, 0);
	if(NULL == tmpBmp)
	{
		DeleteDC(tmpDC);
		return E_FAIL;
	}

	SelectObject(tmpDC, tmpBmp);
	if(_IsRunningVista())
	{
		HWND wallpaperWindow = defView;
		SendMessage(wallpaperWindow, WM_PRINT, (WPARAM)tmpDC, PRF_ERASEBKGND | PRF_CLIENT); 
	}
	else
	{
		DrawWallpaper(tmpDC, bufSize.cx, bufSize.cy);
	}

	HRESULT hRes = directDevice->CreateTexture(bufSize.cx, bufSize.cy,
						1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &wallpaper, NULL);

	if(wallpaper != NULL)
	{
		D3DLOCKED_RECT locked = {0};
		RECT rc = {0,0, bufSize.cx, bufSize.cy};
		IDirect3DSurface9* surface = NULL;
		wallpaper->GetSurfaceLevel(0, &surface);
		if(surface != NULL)
		{
			if(SUCCEEDED(surface->LockRect(&locked, &rc, 0)))
			{
				//GetBitmapBits(tmpBmp, bufSize.cx * bufSize.cy * sizeof(DWORD), locked.pBits);
				int bmpPitch = bufSize.cx * sizeof(DWORD);
				void* bmpStart = (void*) (  (BYTE*)tmpBits + rc.top * bmpPitch + (rc.left) * sizeof(DWORD));
					
				BYTE*  bmpLines = (BYTE*)bmpStart;
				DWORD* bmpPtr = (DWORD*)bmpLines;

				BYTE* lines = (BYTE*)locked.pBits;
				DWORD* ptr = (DWORD*)lines;

				int w = rc.right - rc.left;
				int h = rc.bottom - rc.top;
				for(int y= 0; y < h; ++y)
				{
					ptr = (DWORD*)lines;
					bmpPtr = (DWORD*)bmpLines;

					memcpy(ptr, bmpPtr, w*sizeof(DWORD));

					lines += locked.Pitch;
					bmpLines += bmpPitch;
				}
				surface->UnlockRect();
			}
			surface->Release();
		}
	}

	DeleteObject(tmpBmp);
	DeleteDC(tmpDC);

	return hRes;
}

HRESULT CAveDesktopEffectsHost::StopWallpaperBuffering()
{
	SafeRelease(wallpaper);
	return S_OK;
}

HRESULT CAveDesktopEffectsHost::StartDirectX()
{
	if(directDevice != NULL)
		return E_FAIL;

	HRESULT hRes = S_OK;

	
	RECT rcAll = {0};
	rcAll.left	= 0;
	rcAll.top	= 0;
	rcAll.right	= rcAll.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rcAll.bottom= rcAll.top +  GetSystemMetrics(SM_CYVIRTUALSCREEN);

	SetWindowPos(listView, NULL, rcAll.left, rcAll.top, rcAll.right - rcAll.left, rcAll.bottom - rcAll.top, SWP_NOZORDER | SWP_NOACTIVATE);
	SetWindowPos(defView, NULL, rcAll.left, rcAll.top, rcAll.right - rcAll.left, rcAll.bottom - rcAll.top, SWP_NOZORDER | SWP_NOACTIVATE);

	RECT rc = {0};
	GetClientRect(listView, &rc);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	try
	{
		// create direct3D object
		if(_IsRunningVista())
		{
			Direct3DCreate9Ex(D3D_SDK_VERSION, &direct3D);
			if(NULL == direct3D)
				throw E_FAIL;

			direct3D->QueryInterface(IID_IDirect3D9, (void**)&legacyDirect3D);
		}
		else
		{
			legacyDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
			if(NULL == legacyDirect3D)
				throw E_FAIL;
		}

		// get displaymode for the current adapter
		D3DDISPLAYMODE displayMode = {0};
		hRes = legacyDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
		if(FAILED(hRes))
			throw hRes;
		
		D3DPRESENT_PARAMETERS params = {0};
		params.Windowed               = TRUE;
		params.BackBufferCount        = 1L;
		params.EnableAutoDepthStencil = TRUE;
		params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
		params.BackBufferFormat       = displayMode.Format;
		params.BackBufferWidth        = w;
		params.BackBufferHeight       = h;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D16;
		params.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
		params.MultiSampleType        = D3DMULTISAMPLE_NONE;
		params.MultiSampleQuality     = 0L;
		params.hDeviceWindow		  = listView;
		params.Flags				  = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

		DWORD behaviourFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
		D3DDEVTYPE ref = D3DDEVTYPE_HAL;
		D3DCAPS9 caps;
		if(SUCCEEDED(legacyDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, ref, &caps)))
		{
			if(caps.DevCaps &  D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			{
				behaviourFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			}
			else
			{
				behaviourFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			}
		}
		else
		{
			ref = D3DDEVTYPE_REF;
			behaviourFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		if(_IsRunningVista())
		{
			behaviourFlags |= D3DCREATE_MULTITHREADED;
			D3DDISPLAYMODEEX displayModeEx = {0};
			displayModeEx.Size = sizeof(D3DDISPLAYMODEEX);
			hRes = direct3D->GetAdapterDisplayModeEx(D3DADAPTER_DEFAULT, &displayModeEx, NULL);
			hRes = direct3D->CreateDeviceEx(D3DADAPTER_DEFAULT, ref, listView, behaviourFlags, &params, NULL, &directDeviceEx);
			CheckHRES(hRes);
			directDeviceEx->QueryInterface(IID_IDirect3DDevice9, (void**)&directDevice);
		}
		else
		{
			behaviourFlags |= D3DCREATE_MULTITHREADED;
			legacyDirect3D->CreateDevice(D3DADAPTER_DEFAULT, ref, listView, behaviourFlags, &params, &directDevice);
		}

		// four vertices are used, each represent a 
		// corner of an image.
		hRes = directDevice->CreateVertexBuffer(
			sizeof(AveVertex) * 4,
			D3DUSAGE_WRITEONLY,
			D3DFVF_AVEVERTEX,
			D3DPOOL_DEFAULT,
			&vertexBuffer,
			NULL);
		CheckHRES(hRes);

		hRes = SetUpVertices((float)w, (float)h);
		CheckHRES(hRes);

	}
	catch(HRESULT& hRes)
	{
		StopDirectX();
		return hRes;
	}
	
	// Start icon buffering!
	hRes = StartIconBufferCapturing();
	if(FAILED(hRes))
	{
		StopDirectX();
		return E_FAIL;
	}

	SetRect(&sysMemDirtyRect, 0, 0, 0, 0);
	hRes = directDevice->CreateOffscreenPlainSurface(iconBufferSize.cx,iconBufferSize.cy,
							D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM,&sysMemSurface, NULL);
	if(FAILED(hRes))
	{
		StopIconBufferCapturing();
		StopDirectX();
		return E_FAIL;
	}

	hRes = directDevice->CreateTexture(iconBufferSize.cx, iconBufferSize.cy,
						1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL); 
	if(FAILED(hRes))
	{
		StopIconBufferCapturing();
		StopDirectX();
		return E_FAIL;
	}

	BOOL wantsWallpaper = FALSE;
	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		CComPtr<IAveDesktopEffect>& effect = *iter;
		DWORD flags = 0;
		HRESULT hRes = effect->DoesSupport(&flags);
		if(flags & AVE_SUPPORTS_D3D_DRAW_OVER_REAL_WALLPAPER)
			wantsWallpaper = TRUE;
	}

	if(wantsWallpaper)
		StartWallpaperBuffering();

	// Yes, we made it!
	inD3DMode = TRUE;

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnD3DStart(directDevice);
	}

	SendMessage(listView, LVM_SECRET_ALPHABLENDING_MESSAGE, (WPARAM)TRUE, 0);
	InvalidateRect(listView, NULL, TRUE);

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::StartIconBufferCapturing()
{
	StopIconBufferCapturing();

	RECT rc = {0};
	GetClientRect(listView, &rc);
	iconBufferSize.cx = rc.right - rc.left;
	iconBufferSize.cy = rc.bottom - rc.top;

	iconBufferDC = CreateCompatibleDC(NULL);
	if(NULL == iconBufferDC)
		return E_FAIL;

	BITMAPV5HEADER bi	= {0};
	bi.bV5Size			= sizeof(BITMAPV5HEADER);
    bi.bV5Width			= iconBufferSize.cx;
    bi.bV5Height		= -iconBufferSize.cy;
    bi.bV5Planes		= 1;
    bi.bV5BitCount		= 32;
    bi.bV5Compression	= BI_BITFIELDS;
    bi.bV5RedMask		= 0x00FF0000;
    bi.bV5GreenMask		= 0x0000FF00;
    bi.bV5BlueMask		= 0x000000FF;
    bi.bV5AlphaMask		= 0xFF000000; 

	iconBufferBitmap = CreateDIBSection(iconBufferDC, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&iconBufferBits, NULL, 0);
	if(NULL == iconBufferBitmap)
	{
		StopIconBufferCapturing();
		return E_FAIL;
	}

	SelectObject(iconBufferDC, iconBufferBitmap);

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::StopIconBufferCapturing()
{
	if(iconBufferBitmap != NULL)
		DeleteObject(iconBufferBitmap);

	if(iconBufferDC != NULL)
		DeleteDC(iconBufferDC);

	SetRect(&iconBufferUpdateRect, 0, 0, 0, 0);
	iconBufferIsUpdated		= FALSE;
	iconBufferSize.cx		= 0;
	iconBufferSize.cy		= 0;
	iconBufferBitmap		= NULL;
	iconBufferDC			= NULL;
	iconBufferBits			= NULL;

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::StopDirectX()
{
	if(inD3DMode)
	{
		for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
		{
			CComPtr<IAveDesktopEffect>& effect = *iter;
			HRESULT hRes = effect->OnD3DStop();
		}
	}


	d3dModeRefCount = 0;

	StopWallpaperBuffering();
	SetRect(&sysMemDirtyRect, 0, 0, 0, 0);
	SafeRelease(texture);
	SafeRelease(sysMemSurface);
	SafeRelease(vertexBuffer);
	SafeRelease(directDevice);
	SafeRelease(direct3D);
	SafeRelease(directDeviceEx);
	SafeRelease(legacyDirect3D);

	StopIconBufferCapturing();

	inD3DMode = FALSE;
	
	if(_IsRunningVista())
		SendMessage(listView, LVM_SECRET_ALPHABLENDING_MESSAGE, (WPARAM)FALSE, 0);

	InvalidateRect(listView, NULL, TRUE);

	return S_OK;
}

LRESULT CAveDesktopEffectsHost::HandleDefViewMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_WINDOWPOSCHANGING == msg)
	{
		WINDOWPOS* wp = (WINDOWPOS*)lParam;
		if(wp != NULL)
		{
			RECT rcAll = {0};
			rcAll.left	= 0;
			rcAll.top	= 0;
			rcAll.right	= rcAll.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
			rcAll.bottom= rcAll.top +  GetSystemMetrics(SM_CYVIRTUALSCREEN);
			wp->x  = rcAll.left;
			wp->y  = rcAll.top;
			wp->cx = rcAll.right - rcAll.left;
			wp->cy = rcAll.bottom - rcAll.top;
		}
	}
	else if(WM_ERASEBKGND == msg)
	{
		bHandled = TRUE;

		HDC dc = (HDC)wParam;
		if(NULL == dc)
		{
			bHandled = FALSE;
			return 0;
		}

		RECT clipRect = {0};
		GetClipBox(dc, &clipRect);

		if(inD3DMode && _IsRunningVista())
			SendMessage(listView, LVM_SECRET_ALPHABLENDING_MESSAGE, (WPARAM)TRUE, 0);

		BOOL shouldClear = inD3DMode;
		BOOL shouldPassOn = !inD3DMode;
		BOOL wantsAfterRenderEffect = FALSE;
		for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
		{
			CComPtr<IAveDesktopEffect>& effect = *iter;
			DWORD whatToDo = inD3DMode ? AVE_GDI_BACKGROUND_PAINT_DO_CLEAR : AVE_GDI_BACKGROUND_PAINT_DO_NOT_OVERRIDE;
			HRESULT hRes = effect->OnBeforeGdiBackgroundUpdate(dc, &clipRect, &whatToDo);
			if(SUCCEEDED(hRes))
			{
				if(AVE_GDI_BACKGROUND_PAINT_DO_NOT_OVERRIDE == whatToDo)
				{
					shouldPassOn = TRUE;
				}
				else if(AVE_GDI_BACKGROUND_PAINT_DO_NOT_CLEAR == whatToDo)
				{
					shouldClear = FALSE;
					shouldPassOn = FALSE;
				}
				else if(AVE_GDI_BACKGROUND_PAINT_DO_CLEAR == whatToDo)
				{
					shouldClear = TRUE;
					shouldPassOn = FALSE;
				}
				else if(AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG == whatToDo)
				{
					shouldPassOn = FALSE;
					shouldClear = FALSE;
					wantsAfterRenderEffect = TRUE;
				}
			}

		}

		if(shouldClear)
		{
			HBRUSH bgBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
			FillRect(dc, &clipRect, bgBrush);
		}

		if(wantsAfterRenderEffect)
		{
			DefSubclassProc(hwnd, msg, wParam, lParam);
			for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
			{
				CComPtr<IAveDesktopEffect>& effect = *iter;
				HRESULT hRes = effect->OnAfterGdiBackgroundUpdate(dc, &clipRect);
			}
		}

		bHandled = !shouldPassOn;

		return 1;
	}

	return 0;
}


#define LV_ML_DT_FLAGS  (DT_TOP | DT_NOPREFIX | DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS | DT_END_ELLIPSIS)
#define LV_FL_DT_FLAGS  (DT_TOP | DT_NOPREFIX | DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK | DT_NOCLIP)

void AveDrawAlphaText(HDC dc, const WCHAR* text, RECT rc, DWORD flags)
{
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;

	HDC bufDC = CreateCompatibleDC(dc);

	BITMAPINFO dib = {0};
	HBITMAP hbmp = 0;
	dib.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    dib.bmiHeader.biWidth = w;
    dib.bmiHeader.biHeight = -h;
    dib.bmiHeader.biPlanes = 1;
    dib.bmiHeader.biBitCount = 32;
    dib.bmiHeader.biCompression = BI_RGB;
 
	LPVOID bits = NULL;
    hbmp = CreateDIBSection( bufDC, &dib, DIB_RGB_COLORS, &bits, NULL, 0 );
	SelectObject(bufDC, hbmp);

	LOGFONT lf = {0};
	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), (PVOID)&lf, 0);
	HFONT hFont = CreateFontIndirect(&lf);

	HFONT bufFont = (HFONT)SelectObject(bufDC, hFont);

	int bkMode = GetBkMode(dc);
	SetBkMode(bufDC, bkMode);	
	SetBkColor(bufDC, GetBkColor(dc));
	RECT rcText = {0,0, w, h};
	if(bkMode != TRANSPARENT)
	{
		SetBkMode(bufDC, TRANSPARENT);
		FillRect(bufDC, &rcText, (HBRUSH) (COLOR_HIGHLIGHT+1));
		SetTextColor(bufDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
		DrawText(bufDC, text, -1, &rcText, flags);
	}
	else
	{
		DrawShadowText(bufDC, text, -1, &rcText, flags, RGB(255,255,255), RGB(0,0,0), 2,2);
	}

	DWORD* pixels = (DWORD*)bits;
	DWORD stride = w;
	if(pixels != NULL)
	{
		if(TRANSPARENT == bkMode)
		{
			for(int y = 0; y < h; ++y)
			{
				DWORD* pixel = pixels;
				for(int x = 0; x < w; ++x)
				{
					if((*pixel & 0xFF000000) == 0 && *pixel != 0)
						*pixel = 0xFF000000 | (*pixel & 0x00FFFFFF);
					pixel++;
				}
				pixels += stride;
			}
		}
		else
		{
			for(int y = 0; y < h; ++y)
			{
				DWORD* pixel = pixels;
				for(int x = 0; x < w; ++x)
				{
					*pixel = 0xFF000000 | (*pixel & 0x00FFFFFF);
					pixel++;
				}
				pixels += stride;
			}
		}
	}


	SelectObject(bufDC, bufFont);
	DeleteObject(hFont);

	
	BLENDFUNCTION bf = {0};
	bf.BlendOp = AC_SRC_OVER;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255;
	AlphaBlend(dc, rc.left, rc.top, w, h, bufDC, 0, 0, w, h, bf);

	

	DeleteObject(hbmp);
	DeleteDC(bufDC);
}


void AveDrawText(HDC dc, const WCHAR* text, RECT rc, DWORD flags)
{
	return AveDrawAlphaText(dc, text, rc, flags);
}

void CAveDesktopEffectsHost::_DrawListViewIntoBufferManually(HWND hwnd, HDC dc)
{
	RECT rc = {0};
	GetClipBox(dc, &rc);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
	FillRect(iconBufferDC, &rc, brush);
	DeleteObject(brush);

	int count = ListView_GetItemCount(hwnd);
	for(int i = 0; i < count; ++i)
	{
		RECT rcIcon = {0}, rcLabel = {0};
		ListView_GetItemRect(hwnd, i, &rcIcon, LVIR_ICON);
		RECT rcIntersect = {0};
		if(::IntersectRect(&rcIntersect, &rc, &rcIcon))
		{
			LVITEM item = {0};
			item.mask = LVIF_IMAGE | LVIF_STATE;
			item.iItem = i;
			ListView_GetItem(hwnd, &item);
			HIMAGELIST imgList = ListView_GetImageList(hwnd, LVSIL_NORMAL);

			DWORD flags = 0;
			UINT state = ListView_GetItemState(hwnd, i, LVIS_CUT | LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED | LVIS_OVERLAYMASK);
				
			DWORD fgColor = CLR_DEFAULT;
			if(state & LVIS_CUT)
			{
				flags |= ILD_BLEND50;
				fgColor = CLR_NONE;
			}

			if(state & LVIS_DROPHILITED)
				flags |= ILD_SELECTED;
			/*if(state & LVIS_FOCUSED)
				flags |= ILD_SELECTED;*/
			if(state & LVIS_SELECTED)
				flags |= ILD_SELECTED;

			int cx = 0;
			int cy = 0;
			ImageList_GetIconSize(imgList, &cx, &cy);
			int w = rcIcon.right - rcIcon.left;
			int h = rcIcon.bottom - rcIcon.top;
			int l = rcIcon.left + w/2 - cx/2;
			int t = rcIcon.top + h/2 - cy/2;
			
			ImageList_DrawEx(imgList, item.iImage, iconBufferDC, l, t, cx, cy, CLR_NONE, fgColor, ILD_NORMAL | ILD_TRANSPARENT | flags);
			UINT nOvlImageMask = state & LVIS_OVERLAYMASK;
            ImageList_DrawEx(imgList, item.iImage, iconBufferDC, l, t, cx, cy, CLR_NONE, fgColor, ILD_TRANSPARENT|nOvlImageMask | flags);
		}

		ListView_GetItemRect(hwnd, i, &rcLabel, LVIR_LABEL);
		if(::IntersectRect(&rcIntersect, &rc, &rcLabel))
		{
			WCHAR wText[1024] = {0};
			ListView_GetItemText(hwnd, i, 0, wText, 1024);
				
			HFONT font = (HFONT)SelectObject(dc, GetStockObject(DEFAULT_GUI_FONT));
			HFONT backFont = (HFONT)SelectObject(iconBufferDC, GetStockObject(DEFAULT_GUI_FONT));

			DWORD flags = 0;
			UINT state = ListView_GetItemState(hwnd, i, LVIS_FOCUSED | LVIS_SELECTED);
			if((state & LVIS_FOCUSED) | (state & LVIS_SELECTED))
				flags = LV_FL_DT_FLAGS;
			else
				flags = LV_ML_DT_FLAGS;
			
			if((state & LVIS_SELECTED))
			{
				SetBkMode(iconBufferDC, OPAQUE);
				SetBkColor(iconBufferDC, GetSysColor(COLOR_HIGHLIGHT));
			}
			else
			{
				SetBkMode(iconBufferDC, TRANSPARENT);
			}
			AveDrawText(iconBufferDC, wText, rcLabel, flags);

			SelectObject(iconBufferDC, backFont);
			SelectObject(dc, font);
		}
	}
}

LRESULT CAveDesktopEffectsHost::HandleListViewMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_WINDOWPOSCHANGING == msg)
	{
		WINDOWPOS* wp = (WINDOWPOS*)lParam;
		if(wp != NULL)
		{
			RECT rcAll = {0};
			rcAll.left	= 0;
			rcAll.top	= 0;
			rcAll.right	= rcAll.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
			rcAll.bottom= rcAll.top +  GetSystemMetrics(SM_CYVIRTUALSCREEN);

			wp->x  = rcAll.left;
			wp->y  = rcAll.top;
			wp->cx = rcAll.right - rcAll.left;
			wp->cy = rcAll.bottom - rcAll.top;
		}
	}
	else if(WM_ERASEBKGND == msg && inD3DMode)
	{
		if(!_IsRunningVista())
		{
			bHandled = TRUE;
			_DrawListViewIntoBufferManually(hwnd, (HDC)wParam);
		}
	}
	else if(WM_PAINT == msg && inD3DMode)
	{
		bHandled = TRUE;
		PAINTSTRUCT ps = {0};
		HDC dc = BeginPaint(hwnd, &ps);

		if(_IsRunningVista())
		{
			HRGN region = CreateRectRgn(ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
			SelectClipRgn(iconBufferDC, region);

			SelectObject(iconBufferDC, (HGDIOBJ)iconBufferBitmap);
			DefSubclassProc(hwnd, WM_PAINT, (WPARAM)iconBufferDC, 0);
			DeleteObject(region);
		}

		EndPaint(hwnd, &ps);
		GdiFlush();
		

		UnionRect(&iconBufferUpdateRect, &iconBufferUpdateRect, &ps.rcPaint);
		iconBufferIsUpdated = TRUE;

		InternalD3DRender();
	}

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnTargetWindowMessage(hwnd, msg, wParam, lParam, &lResult, &hasHandled);
		bHandled |= hasHandled;
	}

	return 0;
}

HRESULT CAveDesktopEffectsHost::CopyIconBufferToSysMemSurface(const RECT& rc)
{
	if(sysMemSurface != NULL)
	{
		D3DLOCKED_RECT locked = {0};
		if(SUCCEEDED(sysMemSurface->LockRect(&locked, &rc, 0)))
		{	
			int bmpPitch = iconBufferSize.cx * sizeof(DWORD);
			void* bmpStart = (void*) (  (BYTE*)iconBufferBits + rc.top * bmpPitch + (rc.left) * sizeof(DWORD));
				
			BYTE*  bmpLines = (BYTE*)bmpStart;
			DWORD* bmpPtr = (DWORD*)bmpLines;

			BYTE* lines = (BYTE*)locked.pBits;
			DWORD* ptr = (DWORD*)lines;

			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;
			for(int y= 0; y < h; ++y)
			{
				ptr = (DWORD*)lines;
				bmpPtr = (DWORD*)bmpLines;

				memcpy(ptr, bmpPtr, w*sizeof(DWORD));

				lines += locked.Pitch;
				bmpLines += bmpPitch;
			}

			UnionRect(&sysMemDirtyRect, &sysMemDirtyRect, &rc);	
			sysMemSurface->UnlockRect();
		}
	}

	return E_FAIL;
}

HRESULT CAveDesktopEffectsHost::InternalD3DRender()
{
	if(!CheckTimeBomb())
		return E_FAIL;

	HRESULT hRes = S_OK;

	hRes = UpdateIconBufferToD3D();

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		CComPtr<IAveDesktopEffect>& effect = *iter;
		BOOL hasRendered=FALSE;
		HRESULT hRes = effect->OnD3DRender(&hasRendered);
		if(hasRendered)
			return S_OK;
	}

	// OUR RENDER LOOP:
	// 0) BEFORE RENDER
	// 1) CLEAR DEVICE
	// 2) BEGIN SCENE
	// 3) DRAW BUFFER
	// 4) END SCENE
	// 5) PRESENT
	// 6) AFTER RENDER

	// 0) BEFORE RENDER
	DWORD bgColor = 0x00000000;

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnBeforeD3DRender(&bgColor);
	}

	// 1) CLEAR DEVICE
	hRes = directDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColor, 1.0f, 0);

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnBeforeD3DSceneStart();
	}

	// 2) BEGIN SCENE
	hRes = directDevice->BeginScene();

	// 2.b) DRAW WALLPAPER, if set
	if(wallpaper != NULL)
	{
		UpdateD3DForSimple2DDrawing();
		HRESULT hRes = S_OK;
		hRes = directDevice->SetTexture(0,wallpaper);
		hRes = directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	}

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnD3DSceneStart();
	}

	// (optional)

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnBeforeIconBufferRender();
	}

	// 3) DRAW ICON BUFFER

	hRes = DrawIconBufferToD3DScene();

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnAfterIconBufferRender();
	}


	// (optional)

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnD3DSceneEnd();
	}

	// 4) END SCENE

	hRes = directDevice->EndScene();

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnBeforeD3DPresent();
	}

	// 5) PRESENT

	if(_IsRunningVista())
		directDeviceEx->PresentEx(NULL, NULL, NULL, NULL, 0);
	else
		directDevice->Present(NULL, NULL, NULL, NULL);

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnAfterD3DPresent();
	}

	// (optional)

	// 6) AFTER RENDER

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnAfterD3DRender();
	}

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::UpdateIconBufferToD3D()
{
	// Update the SysMemSurface from the icon buffer
	if(iconBufferIsUpdated)
	{
		CopyIconBufferToSysMemSurface(iconBufferUpdateRect);
		iconBufferIsUpdated = FALSE;
		SetRect(&iconBufferUpdateRect, 0, 0, 0, 0);
	}

	// Update Texture from SysMemSurface
	if(texture != NULL && !IsRectEmpty(&sysMemDirtyRect))
	{
		IDirect3DSurface9* surface = NULL;
		texture->GetSurfaceLevel(0, &surface);
		if(surface != NULL)
		{
			POINT pt = {sysMemDirtyRect.left, sysMemDirtyRect.top};
			if(SUCCEEDED(directDevice->UpdateSurface(sysMemSurface, &sysMemDirtyRect, surface, &pt)))
				SetRect(&sysMemDirtyRect, 0, 0, 0, 0);

			surface->Release();
		}
	}

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::UpdateD3DForSimple2DDrawing()
{
	HRESULT hRes = S_OK;

	D3DXMATRIX Projection, View, World, OrigWorld;
	D3DXMATRIX Scaling, Translation;

	/*D3DXMatrixIdentity(&Projection);
	D3DXMatrixOrthoRH(&Projection, 1.0f, 1.0f, 0.0f, 1.0f);
	D3DXMatrixIdentity(&View);
	D3DXMatrixIdentity(&World);
	D3DXMatrixIdentity(&OrigWorld);*/

	//D3DXMATRIX View, World,  Projection;
	D3DXMatrixOrthoOffCenterLH(&Projection, 0.0f, (float)iconBufferSize.cx, (float)iconBufferSize.cy, 0.0f, 0.0f, 1.0f);
	D3DXMatrixIdentity(&View);
	D3DXMatrixIdentity(&World);
	D3DXMatrixTranslation(&World, float(iconBufferSize.cx)/2.0f, float(iconBufferSize.cy)/2.0f, 0.0f);

	
	directDevice->SetTransform(D3DTS_PROJECTION, &Projection);
	directDevice->SetTransform(D3DTS_WORLD, &World);
	directDevice->SetTransform(D3DTS_VIEW, &View);

	directDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	directDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	directDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	hRes = directDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(AveVertex));
	hRes = directDevice->SetFVF(D3DFVF_AVEVERTEX);
	hRes = directDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, (DWORD)-1);

	hRes = directDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  TRUE);
	hRes = directDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	hRes = directDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	hRes = directDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	hRes = directDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	hRes = directDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	hRes = directDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	directDevice->SetRenderState( D3DRS_AMBIENT,        0 );
    directDevice->SetRenderState( D3DRS_DITHERENABLE,   0 );
    directDevice->SetRenderState( D3DRS_SPECULARENABLE, 0 );
	directDevice->LightEnable(0,0);

	hRes = directDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    hRes = directDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	 directDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, 0 );

	return hRes;
}

HRESULT CAveDesktopEffectsHost::DrawIconBufferToD3DScene()
{
	UpdateD3DForSimple2DDrawing();

	HRESULT hRes = S_OK;
	hRes = directDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
	hRes = directDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	hRes = directDevice->SetTexture(0,texture);
	hRes = directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	hRes = directDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	hRes = directDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return hRes;
}

LRESULT CAveDesktopEffectsHost::HandleNotificationMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_AVE_REBUFFER_WALLPAPER == msg)
	{
		bHandled = TRUE;
		if(wallpaper && inD3DMode)
		{
			for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
			{
				CComPtr<IAveDesktopEffect>& effect = *iter;
				HRESULT hRes = effect->OnNotification(AVE_NOTIFICATION_BEFORE_WALLPAPER_CHANGED);
			}

			inD3DMode = FALSE;
			StopWallpaperBuffering();
			StartWallpaperBuffering();

			for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
			{
				CComPtr<IAveDesktopEffect>& effect = *iter;
				HRESULT hRes = effect->OnNotification(AVE_NOTIFICATION_AFTER_WALLPAPER_CHANGED);
			}

			inD3DMode = TRUE;
		}
	}
	else if(WM_WININICHANGE == msg && SPI_SETDESKWALLPAPER == wParam)
	{
		PostMessage(hwnd, WM_AVE_REBUFFER_WALLPAPER, 0L, 0L);
	}
	else if(WM_AVE_START == msg)
	{
		bHandled = TRUE;
		HRESULT hRes = InternalStart();
		/*
		if(FAILED(hRes))
		{
			CString title, error;
			title.LoadStringW(IDS_DESKTOPEFFECTSTITLE);
			error.LoadStringW(IDS_ERRORSTARTING);
			MessageBoxW(NULL, error, title, MB_ICONERROR);
		}*/
	}
	else if(WM_AVE_STOP == msg)
	{
		bHandled = TRUE;
		HRESULT hRes = InternalStop();
	}
	else if(WM_DEVMODECHANGE == msg)
	{
		PostMessage(hwnd, WM_AVE_RESTART, 0, 0);
	}
	else if(WM_AVE_RESTART == msg)
	{
		if(isRunning && inD3DMode)
		{
			InternalStop();
			InternalStart();
		}
	}

	return 0;
}

HRESULT CAveDesktopEffectsHost::InternalStart()
{
	if(isRunning)
		return E_FAIL;

	if(!CheckTimeBomb())
	{
		const WCHAR* msg = L"This version of Desktop Effects has expired!";
		MessageBox(NULL, msg, L"Desktop Effects", MB_ICONEXCLAMATION);
		return E_FAIL;
	}

	if(!_IsRunningVista())
	{
		GdiplusStartupInput input;
		GdiplusStartup(&gdiplusToken, &input, NULL);
	}

	HRESULT hRes = SubclassDesktop();
	if(FAILED(hRes))
		return hRes;

	isRunning = TRUE;

	RECT rcAll = {0};
	rcAll.left	= 0;
	rcAll.top	= 0;
	rcAll.right	= rcAll.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rcAll.bottom= rcAll.top +  GetSystemMetrics(SM_CYVIRTUALSCREEN);

	SetWindowPos(listView, NULL, rcAll.left, rcAll.top, rcAll.right - rcAll.left, rcAll.bottom - rcAll.top, SWP_NOZORDER | SWP_NOACTIVATE);
	SetWindowPos(defView, NULL, rcAll.left, rcAll.top, rcAll.right - rcAll.left, rcAll.bottom - rcAll.top, SWP_NOZORDER | SWP_NOACTIVATE);

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnStart();
	}

	SetClassLongPtr(listView, GCLP_HBRBACKGROUND, NULL);
	SetClassLongPtr(defView, GCLP_HBRBACKGROUND, NULL);

	InvalidateRect(listView, NULL, TRUE);

	if(configureDlg.m_hWnd != NULL)
		PostMessage(configureDlg.m_hWnd, WM_AVE_STATUSCHANGED, (WPARAM)TRUE, 0L);
	
	return hRes;
}

HRESULT CAveDesktopEffectsHost::FillEffectInfos()
{
	ClearEffectInfos();

	const WCHAR* keyName = L"SOFTWARE\\AveSoftware\\AveDesktopEffects\\Effects";

	HKEY key = NULL;
	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0L, KEY_ENUMERATE_SUB_KEYS,  &key);
	if(key != NULL)
	{
		
		DWORD index = 0;
		do
		{
			WCHAR subKeyName[255] = {0};
			DWORD subKeyBufferLength = sizeof(subKeyName) / sizeof(subKeyName[0]);
			result = RegEnumKeyEx(key, index, subKeyName, &subKeyBufferLength, 0, NULL, NULL, NULL);
			index++;
			if(result != ERROR_NO_MORE_ITEMS)
			{
				HKEY subKey = NULL;
				RegOpenKeyEx(key, subKeyName, 0L, KEY_QUERY_VALUE,  &subKey);
				if(subKey != NULL)
				{
					WCHAR keyValue[255] = {0};
					DWORD keyValueBufferLength = sizeof(keyValue);
					DWORD type=0;
					LONG res = RegQueryValueEx(subKey, L"classid", NULL, &type, (LPBYTE)keyValue, &keyValueBufferLength);
					if(ERROR_SUCCESS == res)
					{
						AveDesktopEffectInfo info = {0};
						if(SUCCEEDED(info.InitializeFromCLSID(keyValue)))
							effectInfos.push_back(info);
					}

					RegCloseKey(subKey);
				}

				AveDesktopEffectInfo info = {0};
			}

		}while(result != ERROR_NO_MORE_ITEMS);

		RegCloseKey(key);
	}

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::ClearEffectInfos()
{
	for(EffectInfoIterator iter = effectInfos.begin(); iter != effectInfos.end(); ++iter)
	{
		AveDesktopEffectInfo& info = *iter;
		info.Free();
	}

	effectInfos.clear();

	return S_OK;	
}


HRESULT CAveDesktopEffectsHost::InternalStop()
{
	HRESULT hRes = UnsubclassDesktop();
	if(FAILED(hRes))
		return hRes;

	if(inD3DMode)
		StopDirectX();

	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		HRESULT hRes = effect->OnStop();
	}

	isRunning = FALSE;
	InvalidateRect(listView, NULL, TRUE);

	if(!_IsRunningVista())
	{
		GdiplusShutdown(gdiplusToken);
		gdiplusToken = NULL;
	}

	if(configureDlg.m_hWnd != NULL)
		PostMessage(configureDlg.m_hWnd, WM_AVE_STATUSCHANGED, (WPARAM)FALSE, 0L);

	return hRes;
}

// CAveDesktopEffectsHost

STDMETHODIMP CAveDesktopEffectsHost::Initialize ( 
  LPCITEMIDLIST pidlFolder,
  LPDATAOBJECT pDataObj,
  HKEY hProgID )
{
	if(!hasInitialized)
		InternalInitialize();

	if(NULL == pidlFolder)
		return E_INVALIDARG;

	if(0 == pidlFolder->mkid.cb)
		return S_OK;

	return E_INVALIDARG;
}

HRESULT CAveDesktopEffectsHost::QueryContextMenu (
  HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd,
  UINT uidLastCmd, UINT uFlags )
{
  // our item is not the default one, thus do not display when only the default item is requested.
  if(uFlags & CMF_DEFAULTONLY)
    return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
 
  if(effects.size() > 0 || isRunning)
  {
	CString toggleStartStopMenuText;
	toggleStartStopMenuText.LoadString(isRunning ? IDS_MENUSTOP : IDS_MENUSTART);
	InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION, uidFirstCmd, toggleStartStopMenuText);
  }

  CString configureMenuText;
  configureMenuText.LoadString(IDS_MENUCONFIGURE);
  InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION, uidFirstCmd+1, configureMenuText);

 
  return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 2 );
}


HRESULT CAveDesktopEffectsHost::GetCommandString (
  UINT_PTR idCmd, UINT uFlags, UINT* pwReserved,
  LPSTR pszName, UINT cchMax )
{
	return E_NOTIMPL;
}

HRESULT CAveDesktopEffectsHost::InvokeCommand (
  LPCMINVOKECOMMANDINFO pCmdInfo)
{
// If lpVerb really points to a string, ignore this function call and bail out.
	if(0 != HIWORD(pCmdInfo->lpVerb))
		return E_INVALIDARG;
 
	switch(LOWORD(pCmdInfo->lpVerb))
	{
		case 1:
			if(NULL == configureDlg.m_hWnd)
			{
				configureDlg.Create(NULL);
				configureDlg.ShowWindow(SW_SHOW);
			}
			else
			{
				configureDlg.BringWindowToTop();
			}

			return S_OK;

		case 0:
			if(isRunning)
			{
				Stop();
			}
			else
			{
				HRESULT hRes = Start();
				if(FAILED(hRes))
				{
					CString title, error;
					title.LoadStringW(IDS_DESKTOPEFFECTSTITLE);
					error.LoadStringW(IDS_ERRORSTARTING);
					MessageBox(pCmdInfo->hwnd, error, title, MB_ICONERROR);
				}
			}
			
			return S_OK;

		default:
			return E_INVALIDARG;
	}
}

HRESULT CAveDesktopEffectsHost::SubclassDesktop()
{
	if(listView != NULL || defView != NULL)
		return E_FAIL;

	HWND progman = FindWindow(L"progman", NULL);
	if(NULL == progman)
		return FALSE;

	defView = FindWindowEx(progman, NULL, L"SHELLDLL_DefView", NULL);
	if(NULL == defView)
		return E_FAIL;

	listView = FindWindowEx(defView, NULL, L"SysListView32", NULL);
	if(NULL == listView)
	{
		defView = NULL;
		return E_FAIL;
	}
	
	BOOL hasSubclassed = SetWindowSubclass(listView, ListViewSubclassProc, 1, (DWORD_PTR)this);
	if(!hasSubclassed)
	{
		defView = NULL;
		listView = NULL;
		return E_FAIL;
	}

	hasSubclassed = SetWindowSubclass(defView, DefViewSubclassProc, 1, (DWORD_PTR)this);
	if(!hasSubclassed)
	{
		RemoveWindowSubclass(listView, ListViewSubclassProc, 1);
		defView = NULL;
		listView = NULL;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::CreateNotificationWindow()
{
	if(NULL == g_NotificationWindow)
		g_NotificationWindow = ::CreateNotificationWindow();

	
	notificationWindow = g_NotificationWindow;
	if(notificationWindow != NULL)
		SetProp(notificationWindow, L"AvePtr", (HANDLE)this);

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::UnsubclassDesktop()
{
	if(NULL == listView || NULL == defView)
		return E_FAIL;

	RemoveWindowSubclass(listView, ListViewSubclassProc, 1);
	RemoveWindowSubclass(defView, DefViewSubclassProc, 1);

	listView = NULL;
	defView = NULL;

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::Start(void)
{
	if(NULL == notificationWindow)
	{
		HRESULT hRes = CreateNotificationWindow();
		if(FAILED(hRes))
			return E_FAIL;
	}

	::PostMessage(notificationWindow, WM_AVE_START, 0, 0);
	WriteRunningStatusToReg(TRUE);

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::Stop(void)
{
	::PostMessage(notificationWindow, WM_AVE_STOP, 0, 0);
	WriteRunningStatusToReg(FALSE);

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::IsRunning(VARIANT_BOOL* pIsRunning)
{
	if(NULL == pIsRunning)
		return E_INVALIDARG;

	if(isRunning)
		*pIsRunning = VARIANT_TRUE;
	else
		*pIsRunning = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::GetTargetWindow(HWND* pTargetWindow)
{
	if(NULL == pTargetWindow)
		return E_INVALIDARG;

	*pTargetWindow = listView;

	return S_OK;
}

HRESULT CAveDesktopEffectsHost::UnloadEffects()
{
	HRESULT hRes = S_OK;
	for(EffectIterator iter = effects.begin(); iter != effects.end(); ++iter)
	{
		LRESULT lResult = 0;
		BOOL hasHandled = FALSE;
		CComPtr<IAveDesktopEffect>& effect = *iter;
		if(isRunning)
			hRes = effect->OnStop();

		hRes = effect->OnUninitialize();

		effect.Release();
	}

	effects.clear();
	effectCLSIDs.clear();
	CoFreeUnusedLibraries();

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::LoadPluginByCLSID(BSTR clsidAsString)
{
	if(NULL == clsidAsString)
		return E_INVALIDARG;

	// No effect!
	if(NULL == clsidAsString || SysStringLen(clsidAsString) == 0)
	{
		Stop();
		UnloadEffects();

		// Save this instance CLSID
		AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", 
		L"CurrentEffectCLSID", clsidAsString);
		/*
		RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", 
		L"CurrentEffectCLSID", REG_SZ, 
		clsidAsString, ((DWORD)wcslen((const WCHAR*)clsidAsString) + 1)*sizeof(WCHAR));
		*/
	}

	CLSID clsid = {0};
	HRESULT hRes = CLSIDFromString(clsidAsString, &clsid);
	if(FAILED(hRes))
		return E_INVALIDARG;

	CComPtr<IAveDesktopEffect> effect;
	hRes = effect.CoCreateInstance(clsid);
	if(FAILED(hRes))
		return E_FAIL;

	// disable all other effects first!
	// DESIGN DECISION: ONLY ONE Plugin can be active at the same time
	UnloadEffects();


	effects.push_back(effect);
	effectCLSIDs.push_back(CString((const WCHAR*)clsidAsString));
	hRes = effect->OnInitialize(this);
	if(SUCCEEDED(hRes))
	{
		// Save this instance CLSID
		AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", 
		L"CurrentEffectCLSID", clsidAsString);
		/*
		RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects", 
		L"CurrentEffectCLSID", REG_SZ, 
		clsidAsString, ((DWORD)wcslen((const WCHAR*)clsidAsString) + 1)*sizeof(WCHAR));
		*/
	}
	
	if(isRunning)
		hRes = effect->OnStart();

	if(inD3DMode)
		hRes = effect->OnD3DStart(directDevice);

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::IsD3DActive(VARIANT_BOOL* pIsActive)
{
	if(NULL == pIsActive)
		return E_INVALIDARG;

	if(inD3DMode)
		*pIsActive = VARIANT_TRUE;
	else
		*pIsActive = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::D3DRenderIconBuffer(DWORD mode)
{
	if(!inD3DMode)
		return E_FAIL;

	if(AVE_D3D_RENDERICONBUFFER_FULL == mode)
	{
		return this->DrawIconBufferToD3DScene();
	}
	else if(AVE_D3D_RENDERICONBUFFER_ONLY_DRAW == mode)
	{
		HRESULT hRes = S_OK;
		hRes = directDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(AveVertex));
		hRes = directDevice->SetFVF(D3DFVF_AVEVERTEX);
		hRes = directDevice->SetTexture(0,texture);
		hRes = directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		return hRes;
	}
	else if(AVE_D3D_RENDERICONBUFFER_ONLY_SET_TEXTURE == mode)
	{
		HRESULT hRes = directDevice->SetTexture(0,texture);
		return hRes;
	}
	else
	{
		return E_INVALIDARG;
	}
}

STDMETHODIMP CAveDesktopEffectsHost::StartD3DMode(void)
{
	d3dModeRefCount++;

	if(1 == d3dModeRefCount)
		return StartDirectX();

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::StopD3DMode(void)
{
	d3dModeRefCount--;

	if(0 == d3dModeRefCount)
		return StopDirectX();

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::D3DRender(void)
{
	if(!inD3DMode)
		return E_FAIL;

	this->InternalD3DRender();

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::MakeSureIsInitialized(void)
{
	InternalInitialize();

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::GetWallpaperTexture(IUnknown** texture)
{
	if(NULL == texture)
		return E_INVALIDARG;

	*texture = NULL;
	if(wallpaper != NULL)
		return wallpaper->QueryInterface(IID_IUnknown, (void**)texture);

	return S_OK;
}

STDMETHODIMP CAveDesktopEffectsHost::SetWallpaperTexture(IUnknown* texture)
{
	if(NULL == texture)
	{
		SafeRelease(wallpaper);
		return S_OK;
	}
	IDirect3DTexture9* newWallpaper = NULL;
	texture->QueryInterface(IID_IDirect3DTexture9, (void**)&newWallpaper);
	if(NULL == newWallpaper)
		return E_INVALIDARG;

	SafeRelease(wallpaper);
	wallpaper = newWallpaper;

	return S_OK;
}
