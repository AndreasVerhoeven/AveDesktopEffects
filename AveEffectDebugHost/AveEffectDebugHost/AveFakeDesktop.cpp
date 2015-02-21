#include "StdAfx.h"
#include "AveFakeDesktop.h"

#include "F:\\Coding\\DesktopProjects\\DesktopEffects\\DesktopFxSDK\\AveRegFuncs.h"

void DrawWallpaper(HDC dc, int cx, int cy)
{
	
	RECT rc = {0,0, cx, cy};
	FillRect(dc, &rc, (HBRUSH)(COLOR_DESKTOP+1));

	//return;

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

CAveFakeDesktop::CAveFakeDesktop(void)
{
	loadHost = TRUE;

	GdiplusStartupInput input;
	GdiplusStartup(&gdiplusToken, &input, NULL);

	OleInitialize(NULL);
}

CAveFakeDesktop::~CAveFakeDesktop(void)
{
	GdiplusShutdown(gdiplusToken);
	gdiplusToken = NULL;
}

LRESULT CAveFakeDesktop::OnPrintClient(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hwnd = FindWindowEx(defView.m_hWnd, NULL, L"SysListView32", NULL);

	HDC dc = ::GetDC(hwnd);

	RECT rcClip = {0};
	GetClipBox(dc, &rcClip);

	::ReleaseDC(hwnd, dc);

	CDCHandle hdc = (HDC)wParam;
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcClip);
	hdc.SelectClipRgn(rgn);

	RedrawWindow(&rcClip, NULL, RDW_ERASE | RDW_INTERNALPAINT);
	UpdateWindow();

	::SendMessage(m_hWnd, WM_ERASEBKGND, wParam, lParam);

	return 1;
}

LRESULT CAveFakeDesktop::OnWinIniChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(SPI_SETDESKWALLPAPER == wParam)
	{
		_FixWallpaper();
		defView.InvalidateRect(NULL, FALSE);
	}

	return 1;
}

LRESULT CAveFakeDesktop::OnWindowposChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	WINDOWPOS* wp = (WINDOWPOS*)lParam;
	if(wp != NULL)
	{
		if(!(wp->flags & SWP_NOSIZE))
		{
			_FixWallpaper();
			if(::IsWindow(defView.m_hWnd))
			{
				defView.SetWindowPos(NULL, wp->x, wp->y, wp->cx, wp->cy, SWP_NOMOVE | SWP_NOZORDER);
			}
		}
	}

	return 0;
}

void CAveFakeDesktop::_FixWallpaper()
{
	CRect rc;
	GetClientRect(&rc);
	CSize size = rc.Size();

	if(dc.IsNull())
		dc.CreateCompatibleDC(NULL);

	if(!bmp.IsNull())
	{
		CSize bmpSize;
		bmp.GetSize(bmpSize);
		if(bmpSize != size)
			bmp.DeleteObject();
	}

	if(bmp.IsNull())
	{
		bmp.CreateCompatibleBitmap(CWindowDC(NULL), size.cx, size.cy);
	}
	
	dc.SelectBitmap(bmp);
	DrawWallpaper(dc, size.cx, size.cy);	
}

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

MIDL_DEFINE_GUID(CLSID, CLSID_AveDesktopEffectsHost,0x96443122,0x0A33,0x4C5B,0xAE,0xB5,0x73,0x09,0x27,0x6D,0x3C,0x71);

LRESULT CAveFakeDesktop::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(L"FakeDesktop");

	_FixWallpaper();

	HRESULT hRes = S_OK;
	CComPtr<IShellFolder> desktop;
	SHGetDesktopFolder(&desktop);
	hRes = desktop->CreateViewObject(NULL, IID_IShellView, (void**)&view);
	if(view != NULL)
	{
		RECT rcFinal = {0};
		GetClientRect(&rcFinal);

		FOLDERSETTINGS folderSettings = {0};
		folderSettings.ViewMode = FVM_ICON;
		folderSettings.fFlags = FWF_DESKTOP;
		
		hRes = view->CreateViewWindow(NULL, &folderSettings, (IShellBrowser*)this, &rcFinal, &defView.m_hWnd);
		view->UIActivate(SVUIA_INPLACEACTIVATE);
		::SetWindowPos(defView, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	if(loadHost)
	{
		CComPtr<IUnknown> host;
		host.CoCreateInstance(CLSID_AveDesktopEffectsHost);
	}


	return 0;
}

LRESULT CAveFakeDesktop::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	defView.Detach();
	if(view != NULL)
	{
		view->UIActivate(SVUIA_DEACTIVATE);
		view->DestroyViewWindow();
		view.Release();
	}

	return 0;
}

LRESULT CAveFakeDesktop::OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CRect rc;
	GetClientRect(&rc);

	if(!bmp.IsNull())
	{
		CSize bmpSize;
		bmp.GetSize(bmpSize);
		if(rc.Size() != bmpSize)
			_FixWallpaper();
	}

	CDCHandle dc((HDC)wParam);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), this->dc, 0, 0, SRCCOPY);

	return 1;
}