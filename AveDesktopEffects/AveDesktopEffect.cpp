// AveDesktopEffect.cpp : Implementation of CAveDesktopEffect

#include "stdafx.h"
#include "AveDesktopEffect.h"

#include <algorithm>

#include "AveRegFuncs.h"

#define WM_AVE_ENUMMEDFILES WM_APP + 1999
#define WM_AVE_LOADEDFILE	WM_APP + 2000

#define TIMER_NEXTFADE	3040
#define TIMER_FADE		3041
#define TIMER_EFFECT	3042

#define INTERVAL_FADE		20
#define INTERVAL_EFFECT		20

// CAveDesktopEffect

DWORD WINAPI ThreadProc(LPVOID lpData)
{
	if(lpData != NULL)
	{
		CAveDesktopEffect* effect = (CAveDesktopEffect*)lpData;
		effect->_OnLoader();	
		ExitThread(0);
	}

	return 0;
}

DWORD WINAPI EnumFilesThreadProc(LPVOID lpData)
{
	if(lpData != NULL)
	{
		CAveDesktopEffect* effect = (CAveDesktopEffect*)lpData;
		effect->_OnEnumFiles();	
		ExitThread(0);
	}

	return 0;
}

DWORD CAveDesktopEffect::_OnEnumFiles()
{
	WCHAR searchPath[MAX_PATH] = {0};
	wcscpy_s(searchPath, MAX_PATH, wallBaseDir);
	PathAppendW(searchPath, L"*.jpg");

	WIN32_FIND_DATAW data = {0};
	HANDLE hFileSearch = FindFirstFile(searchPath, &data);
	if(hFileSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			WCHAR filePath[MAX_PATH] = {0};
			wcscpy_s(filePath, MAX_PATH, wallBaseDir);
			PathAppendW(filePath, data.cFileName);
			CString path = filePath;
			files.push_back(path);

		}while(!doStop && FindNextFile(hFileSearch, &data));

		FindClose(hFileSearch);
	}

	srand(GetTickCount());
	std::random_shuffle(files.begin(), files.end());	

	if(!doStop)
	{
		HWND hwnd = NULL;
		host->GetTargetWindow(&hwnd);
		if(hwnd != NULL)
			PostMessage(hwnd, WM_AVE_ENUMMEDFILES, 0, 0);
	}

	CloseHandle(hThreadEnumFiles);
	hThreadEnumFiles = NULL;

	return 0;
}

DWORD CAveDesktopEffect::_OnLoader()
{
	if(directDevice != NULL)
	{		
		IDirect3DTexture9* text = NULL;
		D3DXCreateTextureFromFileEx(directDevice, wallToBeLoaded, 0, 0, 1, 0, D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR, 0, 0, NULL, NULL, &text);	

		EnterCriticalSection(&cs);
		if(loadedWall != NULL)
			loadedWall->Release();
		loadedWall = text;
		LeaveCriticalSection(&cs);
	}

	if(!doStop)
	{
		HWND hwnd = NULL;
		host->GetTargetWindow(&hwnd);
		if(hwnd != NULL)
			PostMessage(hwnd, WM_AVE_LOADEDFILE, 0, 0);
	}

	CloseHandle(hThread);
	hThread = NULL;

	return 0;
}

STDMETHODIMP CAveDesktopEffect::GetName(BSTR* pName)
{
	if(NULL == pName)
		return E_INVALIDARG;

	CString name;
	name.LoadStringW(IDS_SAMPLEEFFECTNAME);
	*pName = SysAllocString(name);

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::GetAuthor(BSTR* pAuthor)
{
	if(NULL == pAuthor)
		return E_INVALIDARG;

	*pAuthor = SysAllocString(L"Andreas Verhoeven");

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::GetDescription(BSTR* pDescription)
{
	if(NULL == pDescription)
		return E_INVALIDARG;

	CString description;
	description.LoadStringW(IDS_SAMPLEEFFECTDESCRIPTION);
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

STDMETHODIMP CAveDesktopEffect::GetIcon(HBITMAP* pHBitmap)
{
	Bitmap* bmp = LoadBitmapResource(_AtlBaseModule.GetResourceInstance(), L"PNG", MAKEINTRESOURCE(IDR_PNG2));
	if(bmp != NULL)
	{
		bmp->GetHBITMAP(NULL, pHBitmap);
		delete bmp;
	}

	return S_OK;
}


STDMETHODIMP CAveDesktopEffect::GetCooperationFlags(DWORD* pFlags)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnInitialize(IAveDesktopEffectsHost* host)
{
	this->host = host;
	host->AddRef();

	SHGetSpecialFolderPath(NULL, wallBaseDir, CSIDL_MYPICTURES, FALSE);
	CString str = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow", L"PhotoDirectory", L"");
	wcscpy_s(wallBaseDir, MAX_PATH, str);
	/*
	DWORD type = REG_SZ;
	DWORD len = MAX_PATH * sizeof(WCHAR);
	LONG res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow",
					L"PhotoDirectory", RRF_RT_REG_SZ, &type, wallBaseDir, &len);
	if(res != ERROR_SUCCESS)
		SHGetSpecialFolderPath(NULL, wallBaseDir, CSIDL_MYPICTURES, FALSE);
	*/


	
	str = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow",
					L"PhotoIntervalSec", L"30");
	/*
	WCHAR str[200] = {0};
	type = REG_SZ;
	len = sizeof(str);
	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow",
					L"PhotoIntervalSec", RRF_RT_REG_SZ, &type, str, &len);
	if(ERROR_SUCCESS == res)
	{*/
	int val = _wtoi(str);
	if(val > 0)
		photoInterval = val;



	str = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow",
					L"TransitionDurationMs", L"2000");
	/*
	memset(str, 0, sizeof(str));
	type = REG_SZ;
	len = sizeof(str);
	res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow",
					L"TransitionDurationMs", RRF_RT_REG_SZ, &type, str, &len);

	if(ERROR_SUCCESS == res)
	{*/
	val = _wtoi(str);
	if(val > 0)
		transitionDurationMs = val;
	

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnUninitialize(void)
{
	host->Release();

	return S_OK;
}

void CAveDesktopEffect::_StartEnumFiles()
{
	if(hThreadEnumFiles != NULL)
		WaitForSingleObject(hThreadEnumFiles, INFINITE);

	files.clear();
	
	DWORD id = 0;
	hThreadEnumFiles = CreateThread(NULL, 0, EnumFilesThreadProc, (LPVOID)this, 0, &id);
}

STDMETHODIMP CAveDesktopEffect::OnStart(void)
{
	doStop = FALSE;

	host->StartD3DMode();

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnStop(void)
{
	doStop = TRUE;

	if(directDevice != NULL)
		host->StopD3DMode();

	if(hThreadEnumFiles != NULL)
		WaitForSingleObject(hThreadEnumFiles, INFINITE);

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnPause(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnResume(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

void CAveDesktopEffect::_LoadNextWallpaper(const WCHAR* fileName)
{
	if(hThread != NULL)
		WaitForSingleObject(hThread, INFINITE);

	wcscpy_s(wallToBeLoaded, MAX_PATH, fileName);

	DWORD id = 0;
	hThread = CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, 0, &id);
}

STDMETHODIMP CAveDesktopEffect::OnD3DStart(IUnknown* directDeviceAsUnknown)
{
	if(NULL == directDeviceAsUnknown)
		return E_INVALIDARG;

	CLSID clsid = {0};
	CLSIDFromString(L"{D0223B96-BF7A-43fd-92BD-A43B0D82B9EB}", &clsid);

	directDeviceAsUnknown->QueryInterface(clsid, (void**)&directDevice);
	if(NULL == directDevice)
		return E_INVALIDARG;

	//_LoadNextWallpaper(L"C:\\windows\\web\\wallpaper\\img20.jpg");
	//D3DXCreateTextureFromFile(directDevice, L"C:\\windows\\web\\wallpaper\\img20.jpg", &wallA);
	//D3DXCreateTextureFromFile(directDevice, L"C:\\windows\\web\\wallpaper\\img19.jpg", &wallB);

	// Set light 0 to be a pure white directional light
	D3DLIGHT9 light0;
	ZeroMemory( &light0, sizeof(D3DLIGHT9) );
	light0.Type = D3DLIGHT_DIRECTIONAL;
	light0.Direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	light0.Diffuse.r = 1.0f;
	light0.Diffuse.g = 1.0f;
	light0.Diffuse.b = 1.0f;
	light0.Diffuse.a = 1.0f;
	light0.Range = 1000.0f;
	directDevice->SetLight( 0, &light0 );
	directDevice->LightEnable( 0, TRUE );

	currentFileIndex = 0;
	_StartEnumFiles();

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnD3DStop(void)
{
	if(hThread != NULL)
		WaitForSingleObject(hThread, INFINITE);

	HWND hwnd=NULL;
	host->GetTargetWindow(&hwnd);
	if(hwnd != NULL)
	{
		KillTimer(hwnd, TIMER_NEXTFADE);
		KillTimer(hwnd, TIMER_FADE);
		//KillTimer(hwnd, TIMER_EFFECT);
	}

	EnterCriticalSection(&cs);

	if(loadedWall != NULL)
	{
		loadedWall->Release();
		loadedWall = NULL;
	}

	LeaveCriticalSection(&cs);

	if(nextWall != NULL)
		nextWall->Release();

	if(currentWall != NULL)
		currentWall->Release();

	if(directDevice != NULL)
		directDevice->Release();

	loadedWall = NULL;
	currentWall = NULL;
	nextWall = NULL;
	directDevice = NULL;

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnBeforeGdiBackgroundUpdate(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo)
{
	//*pWhatToDo = AVE_GDI_BACKGROUND_PAINT_DO_NOT_CLEAR;
	//*pWhatToDo = AVE_GDI_WANTS_TO_RENDER_OVER_DEFAULT_BG;

	//HBRUSH brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//FillRect(hdc, updateRect, brush);

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnAfterGdiBackgroundUpdate(HDC hdc, const RECT* updateRect)
{
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

STDMETHODIMP CAveDesktopEffect::OnBeforeIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnAfterIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnD3DRender(BOOL* pHasRendered)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnBeforeD3DRender(DWORD* pBgColor)
{
	if(NULL == pBgColor)
		return E_INVALIDARG;

	*pBgColor = 0xFF000000;

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnBeforeD3DSceneStart(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnD3DSceneStart(void)
{
	directDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	directDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	directDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);

	directDevice->SetRenderState(D3DRS_LIGHTING, 1);
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(mtrl) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;

	directDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
	directDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	if(currentWall)
	{
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;//1.0f - float(fadingInCounter) / 255.0f;
		directDevice->SetMaterial( &mtrl );

		directDevice->SetTexture(0, currentWall);
		directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	}

	if(nextWall)
	{
		mtrl.Diffuse.a = mtrl.Ambient.a = sin(D3DX_PI /2.0f * fadingProgress);
		directDevice->SetMaterial( &mtrl );

		directDevice->SetTexture(0, nextWall);
		directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	}
	

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnBeforeIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnAfterIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnD3DSceneEnd(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnBeforeD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnAfterD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnAfterD3DRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

void CAveDesktopEffect::SetBaseDir(const WCHAR* path)
{
	wcscpy_s(wallBaseDir, MAX_PATH, path);
	if(directDevice != NULL && !doStop)
	{
		currentFileIndex = 0;
		_StartEnumFiles();
	}

	AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow", 
		L"PhotoDirectory", wallBaseDir);
	/*RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow", 
		L"PhotoDirectory", REG_SZ, 
		wallBaseDir, ((DWORD)wcslen(wallBaseDir) + 1)*sizeof(WCHAR));
	*/
}

void CAveDesktopEffect::SetPhotoInterval(int sec)
{
	photoInterval = sec;
	if(directDevice != NULL && !doStop && NULL == nextWall)
	{
		HWND hwnd = NULL;
		host->GetTargetWindow(&hwnd);
		if(hwnd != NULL)
			SetTimer(hwnd, TIMER_NEXTFADE, photoInterval * 1000, NULL);
	}

	WCHAR intervalAsString[100] = {0};
	_itow_s(photoInterval, intervalAsString, 100, 10);
	AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow", 
		L"PhotoIntervalSec", intervalAsString);
	/*RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow", 
		L"PhotoIntervalSec", REG_SZ, 
		intervalAsString, ((DWORD)wcslen(intervalAsString) + 1)*sizeof(WCHAR));
	*/
}

void CAveDesktopEffect::SetFadeInterval(int ms)
{
	transitionDurationMs = ms;

	WCHAR durationAsString[100] = {0};
	_itow_s(transitionDurationMs, durationAsString, 100, 10);
	AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow", 
		L"TransitionDurationMs", durationAsString);
	/*
	RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\SlideShow", 
		L"TransitionDurationMs", REG_SZ, 
		durationAsString, ((DWORD)wcslen(durationAsString) + 1)*sizeof(WCHAR));
	*/
}

void CAveDesktopEffect::ToNextPhoto()
{
	if(directDevice != NULL && !doStop && NULL == nextWall)
	{
		HWND hwnd = NULL;
		host->GetTargetWindow(&hwnd);
		if(hwnd != NULL)
		{
			KillTimer(hwnd, TIMER_NEXTFADE);
			if(!doStop && files.size() > 0)
			{
				currentFileIndex = (currentFileIndex+1) % (int)files.size();
				_LoadNextWallpaper(files[currentFileIndex]);
			}
		}
	}
}

STDMETHODIMP CAveDesktopEffect::OnTargetWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	if(WM_AVE_ENUMMEDFILES == msg)
	{
		*bHandled = TRUE;
		*lResult = 0;

		if(!doStop && files.size() > 0)
			_LoadNextWallpaper(files[currentFileIndex]);
	}
	else if(WM_AVE_LOADEDFILE == msg)
	{
		*bHandled = TRUE;
		*lResult = 0;

		if(nextWall != NULL)
			nextWall->Release();

		EnterCriticalSection(&cs);
		nextWall = loadedWall;
		loadedWall = NULL;
		LeaveCriticalSection(&cs);

		if(!doStop)
		{
			fadingProgress = 0.0f;
			transitionTickStart = GetTickCount();
			//KillTimer(hwnd, TIMER_EFFECT);
			KillTimer(hwnd, TIMER_NEXTFADE);
			SetTimer(hwnd, TIMER_FADE, INTERVAL_FADE, NULL);
		}
	}
	else if(WM_TIMER == msg)
	{
		if(TIMER_FADE == wParam)
		{
			*bHandled = TRUE;
			*lResult = 0;

			transitionCurrentTick = GetTickCount();

			DWORD length = transitionCurrentTick - transitionTickStart;
			if(length < 0)
			{
				transitionTickStart = transitionCurrentTick;
				length = 0;
			}

			fadingProgress = float(length) / float(transitionDurationMs);

			if(fadingProgress >= 1.0f)
			{
				fadingProgress = 0.0f;
				if(currentWall != NULL)
					currentWall->Release();

				currentWall = nextWall;
				nextWall = NULL;

				KillTimer(hwnd, TIMER_FADE);
				if(!doStop)
				{
					SetTimer(hwnd, TIMER_NEXTFADE, photoInterval * 1000, NULL);
					//SetTimer(hwnd, TIMER_EFFECT, INTERVAL_EFFECT, NULL);
				}
			}

			if(host != NULL)
				host->D3DRender();
		}
		else if(TIMER_EFFECT == wParam)
		{
			*bHandled = TRUE;
			*lResult = 0;

			if(host != NULL)
				host->D3DRender();
		}
		else if(TIMER_NEXTFADE == wParam)
		{
			*bHandled = TRUE;
			*lResult = 0;

			ToNextPhoto();
		}
	}

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::GetConfigurationWindow(HWND* hwnd, HWND parent)
{
	if(NULL == hwnd)
		return E_INVALIDARG;

	configDlg.Create(parent);
	*hwnd = configDlg.m_hWnd;

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::OnNotification(DWORD dwNotification)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CAveDesktopEffect::DoesSupport(DWORD* pFlag)
{
	if(NULL == pFlag)
		return E_INVALIDARG;

	*pFlag = AVE_SUPPORTS_CONFIG;

	return S_OK;
}
