// DesktopEffectSnow.cpp : Implementation of CDesktopEffectSnow

#include "stdafx.h"
#include "DesktopEffectSnow.h"

void CDesktopEffectSnow::SetDoRenderGround(BOOL val)
{
	BOOL old = doRenderGround;

	/*
	doRenderGround = val;
	DWORD v = doRenderGround;
	RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Ground", REG_DWORD,
					&v, sizeof(DWORD));	
	*/

	AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Ground",
					doRenderGround);

	if(directDevice != NULL && old != doRenderGround)
	{
		if(doRenderGround)
			system.InitBuckets(250);
		else
			system.ClearBuckets();
	}
}

void CDesktopEffectSnow::SetWind(int val)
{
	wind = val;

	/*
	DWORD v = wind;
	RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Wind", REG_DWORD,
					&v, sizeof(DWORD));	
					*/

	AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Wind\\", L"Ground",
					wind);
}

void CDesktopEffectSnow::SetSpeed(int val)
{
	speed = val;

	/*
	DWORD v = speed;
	RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Speed", REG_DWORD,
					&v, sizeof(DWORD));
	*/
	AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Wind\\", L"Speed",
					speed);

}

void CDesktopEffectSnow::SetMaxCounter(int val)
{
	maxCounter = val;

	/*
	DWORD v = maxCounter;
	RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"MaxCounter", REG_DWORD,
					&v, sizeof(DWORD));
	*/
	AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Wind\\", L"MaxCounter",
					maxCounter);
}

void CDesktopEffectSnow::SetSize(int val)
{
	size = val;
	
	/*
	DWORD v = size;
	RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Size", REG_DWORD,
					&v, sizeof(DWORD));	*/
	AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Wind\\", L"Size",
					size);
}

void CDesktopEffectSnow::SetDoRandomFlakes(BOOL val)
{
	doRandomFlakes = val;
	/*
	DWORD v = doRandomFlakes;
	RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"DoRandomFlakes", REG_DWORD,
					&v, sizeof(DWORD));	
					*/
	AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\Wind\\", L"DoRandomFlakes",
					doRandomFlakes);

	HWND hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
	{
		flakesPeriod = 0;
		if(doRandomFlakes)
			SetTimer(hwnd, 444, 1500, NULL);
		else
			KillTimer(hwnd, 444);
	}
}

// CDesktopEffectSnow

// Called when the name of this effect is required
STDMETHODIMP CDesktopEffectSnow::GetName(BSTR* pName)
{
	if(NULL == pName)
		return E_INVALIDARG;

	CString name;
	name.LoadString(IDS_EFFECTNAME);
	*pName = SysAllocString(name);

	return S_OK;
}

// Called when the author of this effect is required
STDMETHODIMP CDesktopEffectSnow::GetAuthor(BSTR* pAuthor)
{
	if(NULL == pAuthor)
		return E_INVALIDARG;

	CString author;
	author.LoadString(IDS_AUTHORNAME);
	*pAuthor = SysAllocString(author);

	return S_OK;
}

// Called when the description of this effect is required
STDMETHODIMP CDesktopEffectSnow::GetDescription(BSTR* pDescription)
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
STDMETHODIMP CDesktopEffectSnow::GetIcon(HBITMAP* pHBitmap)
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
STDMETHODIMP CDesktopEffectSnow::GetCooperationFlags(DWORD* pFlags)
{
	// NOT IMPLEMENTED CURRENTLY

	return S_OK;
}

// Called when this effect is loaded and initialized.
STDMETHODIMP CDesktopEffectSnow::OnInitialize(IAveDesktopEffectsHost* host)
{
	// Store a ptr to our host
	this->host = host;
	host->AddRef();

	maxCounter = AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"MaxCounter", maxCounter);
	size = AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Size", size);
	wind = AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Wind", wind);
	speed = AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Speed", speed);
	doRandomFlakes = AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"DoRandomFlakes", doRandomFlakes);
	doRenderGround = AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Ground", doRenderGround);

	/*
	DWORD type = REG_DWORD;
	DWORD val = 0;
	DWORD len = sizeof(DWORD);
	LSTATUS res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"MaxCounter",
					RRF_RT_REG_DWORD, &type, &val, &len);
	if(ERROR_SUCCESS == res)
		maxCounter = val;

	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Size",
					RRF_RT_DWORD, &type, &val, &len);
	if(ERROR_SUCCESS == res)
		size = val;

	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Size",
					RRF_RT_REG_DWORD, &type, &val, &len);
	if(ERROR_SUCCESS == res)
		size = val;

	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Wind",
					RRF_RT_REG_DWORD, &type, &val, &len);
	if(ERROR_SUCCESS == res)
		wind = val;

	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Speed",
					RRF_RT_REG_DWORD, &type, &val, &len);
	if(ERROR_SUCCESS == res)
		speed = val;

	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"DoRandomFlakes",
					RRF_RT_REG_DWORD, &type, &val, &len);
	if(ERROR_SUCCESS == res)
		doRandomFlakes = val;

	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\WinterSnow\\", L"Ground",
					RRF_RT_REG_DWORD, &type, &val, &len);
	if(ERROR_SUCCESS == res)
		doRenderGround = val;
		*/


	return S_OK;
}

// Called when this effect is unloaded
STDMETHODIMP CDesktopEffectSnow::OnUninitialize(void)
{

	// Release our ptr to our host
	host->Release();

	return S_OK;
}


// Called when DesktopEffects rendering is started
STDMETHODIMP CDesktopEffectSnow::OnStart(void)
{
	// D3D example:
	host->StartD3DMode();

	return S_OK;
}

// Called when DesktopEffects rendering is stopped
STDMETHODIMP CDesktopEffectSnow::OnStop(void)
{
	
	//D3D example:
	if(directDevice != NULL)
		host->StopD3DMode();

	return S_OK;
}

// Called when DesktopEffects is paused
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectSnow::OnPause(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when DesktopEffects is resumed from a paused state
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectSnow::OnResume(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called when D3D mode is started. Gives a Pointer to an IUnknown that implements
// IDirect3DDevice9.
STDMETHODIMP CDesktopEffectSnow::OnD3DStart(IUnknown* directDeviceAsUnknown)
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

	WCHAR searchPath[MAX_PATH] = {0};
	GetModuleFileName(ATL::_AtlBaseModule.GetResourceInstance(), searchPath, MAX_PATH);
	PathRemoveFileSpec(searchPath);
	PathAppend(searchPath, L"snow");
	//wcscpy_s(searchPath, MAX_PATH, L"c:\\snow");

	WCHAR groundPath[MAX_PATH] = {0};
	wcscpy_s(groundPath, MAX_PATH, searchPath);
	PathAppendW(groundPath, L"ground\\1.png");
	D3DXCreateTextureFromFileEx(directDevice, groundPath, 0, 0, 1, 0, D3DFMT_UNKNOWN,
					D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR, 0, 0, NULL, NULL, &ground);	


	WCHAR search[MAX_PATH] = {0};
	wcscpy_s(search, MAX_PATH, searchPath);
	PathAppend(search, L"*.png");

	WIN32_FIND_DATA data = {0};
	HANDLE h = FindFirstFile(search, &data);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			WCHAR file[MAX_PATH] = {0};
			wcscpy_s(file, MAX_PATH, searchPath);
			PathAppend(file, data.cFileName);
			IDirect3DTexture9* texture = NULL;
			D3DXCreateTextureFromFileEx(directDevice, file, 0, 0, 1, 0, D3DFMT_UNKNOWN,
					D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR, 0, 0, NULL, NULL, &texture);	
			if(texture != NULL)
				particles.push_back(texture);

		}while(FindNextFile(h, &data));
		FindClose(h);
	}
		
	system.SetPos(300.0f, 0.0f);
	system.Begin(sceneSize.cx / 2, sceneSize.cy / 2, (int)particles.size());
	if(doRenderGround)
		system.InitBuckets(250);

	
	hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
		SetTimer(hwnd, 1001, 20, 0);

	return S_OK;
}

// Called when D3D mode is stopped.
STDMETHODIMP CDesktopEffectSnow::OnD3DStop(void)
{
	HWND hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
	{
		KillTimer(hwnd, 1001);
		KillTimer(hwnd, 444);
	}

	for(size_t i = 0; i < particles.size(); ++i)
	{
		if(particles[i] != NULL)
			particles[i]->Release();
	}
	particles.clear();

	for(size_t i = 0; i < icons.size(); ++i)
	{
		if(icons[i] != NULL)
			icons[i]->Release();
	}
	icons.clear();

	if(ground != NULL)
		ground->Release();

	ground = NULL;

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
STDMETHODIMP CDesktopEffectSnow::OnBeforeGdiBackgroundUpdate(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo)
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
STDMETHODIMP CDesktopEffectSnow::OnAfterGdiBackgroundUpdate(HDC hdc, const RECT* updateRect)
{
	if(NULL == updateRect || NULL == hdc)
		return E_FAIL;

	return S_OK;
}

// Called before the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectSnow::OnBeforeIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectSnow::OnAfterIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene needs to be rendered when in D3D mode.
// Set pHasRendered to TRUE if the scene has already been rendered by
// this method. Setting pHasRendered to TRUE allows an Effect to
// modify the complete render-cycle.
STDMETHODIMP CDesktopEffectSnow::OnD3DRender(BOOL* pHasRendered)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D render cycle is started, if it has not been taken
// over by OnD3DRender().
// Set pBgColor to the background desired color of the scene.
STDMETHODIMP CDesktopEffectSnow::OnBeforeD3DRender(DWORD* pBgColor)
{
	if(NULL == pBgColor)
		return E_INVALIDARG;

	// Clear with solid black.
	//*pBgColor = 0xFF000000;

	return S_OK;
}

// Called before the D3D scene is started.
STDMETHODIMP CDesktopEffectSnow::OnBeforeD3DSceneStart(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


// Called after the D3D scene is started.
// This is the place for this Effect to draw its own elements into the
// scene.
// By default, the Device is in a 2D render state with a quad VertexBuffer
// that is the size of the desktop, with direct mapping texture coords.
STDMETHODIMP CDesktopEffectSnow::OnD3DSceneStart(void)
{
	//HWND hwnd = NULL;
	//host->GetTargetWindow(&hwnd);
	system.Update(NULL, float(size) / 3.0f);
	system.Render(directDevice, (float)sceneSize.cx, (float)sceneSize.cy, &particles[0]);
	if(doRenderGround)
		system.RenderBuckets(directDevice, ground);

	return S_OK;
}

// Called before the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectSnow::OnBeforeIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectSnow::OnAfterIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene has ended.
STDMETHODIMP CDesktopEffectSnow::OnD3DSceneEnd(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectSnow::OnBeforeD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectSnow::OnAfterD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the whole D3D render loop has been finished.
STDMETHODIMP CDesktopEffectSnow::OnAfterD3DRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the target window (the desktops listview often) receives a message.
// set lResult to the desired return value of the MsgProc and set bHandled to TRUE
// if this message should not be passed to the original MsgProc of the target window.
// If mouse interactivity is needed, this is the place to track mouse movements for example
// by monitorin WM_MOUSEMOVE.
STDMETHODIMP CDesktopEffectSnow::OnTargetWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	//return OnNotificationWindowMessage(hwnd, msg, wParam, lParam, lResult, bHandled);
	return S_OK;
}

// Called when there is a message for the notification window.
// Each effect gets its own notification helper window for use with timers,
// thread marshalling and such.
// By calling IAveDesktopEffectsHost::GetHelperWindow(this, &hwnd) this 
// window will be created and a handle to it will be obtained.
STDMETHODIMP CDesktopEffectSnow::OnNotificationWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	if(WM_TIMER == msg && 444 == wParam)
	{
		*bHandled = TRUE;
		if(flakesPeriod <= 0)
		{
			flakesPeriod = rand() % 120;
			nextMaxTarget = rand() % 30;
			float fdiff = float(nextMaxTarget - maxCounter);
			targetAdd = fdiff / float(flakesPeriod);
			fMaxCounter = float(maxCounter);
		}
		else
		{
			--flakesPeriod;
			fMaxCounter += targetAdd;
			maxCounter = int(fMaxCounter);
		}
	}
	else if(WM_TIMER == msg && 1001 == wParam)
	{
		*bHandled = TRUE;
		
		if(++counter >= maxCounter)
		{
			counter = 0;
			float x = float(sceneSize.cx+40) * system.rnd() - float(sceneSize.cx / 2) - 20.0f;
			float y = 0.0f - float(sceneSize.cy / 2) - 20.0f;

			float fwind  = float(wind) / 10.0f;
			float fspeed = float(speed) / 100.0f;
			float fsize = float(size);
			system.AddSpark(x,y, fwind, fspeed, fsize);
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
STDMETHODIMP CDesktopEffectSnow::GetConfigurationWindow(HWND* hwnd, HWND parent)
{
	if(NULL == hwnd)
		return E_INVALIDARG;

	dlg.Create(parent);
	*hwnd = dlg.m_hWnd;

	return S_OK;
}

// Called when there is a notification for the Effect.
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectSnow::OnNotification(DWORD dwNotification)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CDesktopEffectSnow::DoesSupport(DWORD* pFlag)
{
	if(NULL == pFlag)
		return E_INVALIDARG;

	*pFlag = AVE_SUPPORTS_D3D_DRAW_OVER_REAL_WALLPAPER | AVE_SUPPORTS_CONFIG;

	return S_OK;
}
