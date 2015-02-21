// DesktopEffectVideoWallpaper.cpp : Implementation of CDesktopEffectVideoWallpaper

#include "stdafx.h"
#include "DesktopEffectVideoWallpaper.h"
// DesktopEffectWaterWallpaper.cpp : Implementation of CDesktopEffectVideoWallpaper

#define WM_AVE_MEDIA_EVENT		WM_USER + 3001
#define WM_AVE_START_RENDERING	WM_USER + 3002

// Called when the name of this effect is required
STDMETHODIMP CDesktopEffectVideoWallpaper::GetName(BSTR* pName)
{
	if(NULL == pName)
		return E_INVALIDARG;

	CString name;
	name.LoadString(IDS_EFFECTNAME);
	*pName = SysAllocString(name);

	return S_OK;
}

// Called when the author of this effect is required
STDMETHODIMP CDesktopEffectVideoWallpaper::GetAuthor(BSTR* pAuthor)
{
	if(NULL == pAuthor)
		return E_INVALIDARG;

	CString author;
	author.LoadString(IDS_AUTHORNAME);
	*pAuthor = SysAllocString(author);

	return S_OK;
}

// Called when the description of this effect is required
STDMETHODIMP CDesktopEffectVideoWallpaper::GetDescription(BSTR* pDescription)
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
STDMETHODIMP CDesktopEffectVideoWallpaper::GetIcon(HBITMAP* pHBitmap)
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
STDMETHODIMP CDesktopEffectVideoWallpaper::GetCooperationFlags(DWORD* pFlags)
{
	// NOT IMPLEMENTED CURRENTLY

	return S_OK;
}

// Called when this effect is loaded and initialized.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnInitialize(IAveDesktopEffectsHost* host)
{
	// Store a ptr to our host
	this->host = host;
	host->AddRef();

	/*
	DWORD type = REG_SZ;
	DWORD len = MAX_PATH * sizeof(WCHAR);
	
	LONG res = RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\VideoWallpaper",
					L"FileName", RRF_RT_REG_SZ, &type, fileName, &len);
	*/
	CString val = AveRegFuncs::ReadString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\VideoWallpaper",
					L"FileName", fileName);

	wcscpy_s(fileName, MAX_PATH, val);

	return S_OK;
}

// Called when this effect is unloaded
STDMETHODIMP CDesktopEffectVideoWallpaper::OnUninitialize(void)
{

	// Release our ptr to our host
	host->Release();

	return S_OK;
}

// Called when DesktopEffects rendering is started
STDMETHODIMP CDesktopEffectVideoWallpaper::OnStart(void)
{
	// D3D example:
	if(wcslen(fileName) > 0)
		host->StartD3DMode();

	return S_OK;
}

// Called when DesktopEffects rendering is stopped
STDMETHODIMP CDesktopEffectVideoWallpaper::OnStop(void)
{

	//D3D example:
	if(directDevice != NULL)
		host->StopD3DMode();

	return S_OK;
}

void CDesktopEffectVideoWallpaper::SetFileName(const WCHAR* fileName)
{
	if(_wcsicmp(fileName, this->fileName) == 0)
		return;

	wcscpy_s(this->fileName, MAX_PATH, fileName);
	/*
	RegSetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\VideoWallpaper", 
		L"FileName", REG_SZ, 
		fileName, ((DWORD)wcslen(fileName) + 1)*sizeof(WCHAR));
		*/

	AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"SOFTWARE\\AveSoftware\\DesktopEffects\\VideoWallpaper", 
		L"FileName", fileName);

	if(directDevice != NULL)
	{
		StopVMR();
		StartVMR();
	}
	else
	{
		VARIANT_BOOL isRunning = VARIANT_FALSE;
		host->IsRunning(&isRunning);
		if(isRunning != VARIANT_FALSE)
			host->StartD3DMode();
		else
			host->Start();
	}
}

// Called when DesktopEffects is paused
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectVideoWallpaper::OnPause(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when DesktopEffects is resumed from a paused state
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectVideoWallpaper::OnResume(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

void CDesktopEffectVideoWallpaper::ClearSurfaces()
{
	for(std::vector<IDirect3DSurface9*>::iterator iter = surfaces.begin(); iter != surfaces.end(); ++iter)
	{
		IDirect3DSurface9* surface = *iter;
		if(surface)
			surface->Release();
	}
	surfaces.clear();
}

STDMETHODIMP CDesktopEffectVideoWallpaper::InitializeDevice( 
    DWORD_PTR dwUserID,
    VMR9AllocationInfo *lpAllocInfo,
    DWORD *lpNumBuffers)
{
	if (lpNumBuffers == NULL)
    {
        return E_POINTER;
    }

    if ( allocatorNotify == NULL )
    {
        return E_FAIL;
    }

	HWND hwnd = NULL;
	host->GetTargetWindow(&hwnd);
    
	HRESULT hr = allocatorNotify->SetD3DDevice( directDevice,
        MonitorFromWindow( hwnd, MONITOR_DEFAULTTOPRIMARY ) );
    if (FAILED(hr))
    {
        return hr;
    }
    // Make sure to create textures. Otherwise, the surface cannot be
    // textured onto our primitive. Therefore, add the "texture" flag.
    lpAllocInfo->dwFlags |= VMR9AllocFlag_TextureSurface;

	ClearSurfaces();

    // Resize the array of surface pointers.
	for(int i = 0; i < (int)(*lpNumBuffers); ++i)
		surfaces.push_back(NULL);

    // Ask the VMR-9 to allocate the surfaces for us.
    hr = allocatorNotify->AllocateSurfaceHelper(
        lpAllocInfo, lpNumBuffers, & surfaces.at(0) );
    
    // If this call failed, create a private texture. We will copy the
    // decoded video frames into the private texture. 
    if (FAILED(hr) && 
        !(lpAllocInfo->dwFlags & VMR9AllocFlag_3DRenderTarget))
    {
        ClearSurfaces();
        // Is the format YUV?
        if (lpAllocInfo->Format > '0000')
        {           
            D3DDISPLAYMODE dm;
            hr = directDevice->GetDisplayMode(NULL,  & dm );
            if (SUCCEEDED(hr))
            {
               // Create the private texture.
               hr = directDevice->CreateTexture(
                   lpAllocInfo->dwWidth, 
                   lpAllocInfo->dwHeight,
                   1, 
                   D3DUSAGE_RENDERTARGET, 
                   dm.Format, 
                   D3DPOOL_DEFAULT, 
                   &videoTexture, NULL );
            }
            if (FAILED(hr))
            {
               return hr;
            }
        }
		for(int i = 0; i < (int)(*lpNumBuffers); ++i)
			surfaces.push_back(NULL);

        lpAllocInfo->dwFlags &= ~ VMR9AllocFlag_TextureSurface;
        lpAllocInfo->dwFlags |= VMR9AllocFlag_OffscreenSurface;
        hr = allocatorNotify->AllocateSurfaceHelper(
            lpAllocInfo, lpNumBuffers, & surfaces.at(0) );
        if (FAILED(hr))
        {
            return hr;
        }
    }
   return S_OK;
}

HRESULT CDesktopEffectVideoWallpaper::StartVMR()
{
	graphBuilder.CoCreateInstance(CLSID_FilterGraph);
	if(NULL == graphBuilder)
		return E_FAIL;

	graphBuilder.QueryInterface(&mediaControl);
	graphBuilder.QueryInterface(&mediaPosition);
	graphBuilder.QueryInterface(&mediaEvent);

	CComPtr<IMediaEventEx> mediaEventEx;
	graphBuilder.QueryInterface(&mediaEventEx);

	HWND hwnd = NULL;
	host->GetTargetWindow(&hwnd);
	if(hwnd != NULL)
		mediaEventEx->SetNotifyWindow((OAHWND)hwnd, WM_AVE_MEDIA_EVENT, 0);

	CComPtr<IBaseFilter> vmr9;
	vmr9.CoCreateInstance(CLSID_VideoMixingRenderer9);
	if(vmr9)
	{
		CComQIPtr<IVMRFilterConfig9> filterConfig(vmr9);
		if (filterConfig)
		{
			filterConfig->SetRenderingMode( VMR9Mode_Renderless );
			filterConfig->SetNumberOfStreams(0);
			filterConfig->SetRenderingPrefs(RenderPrefs_DoNotRenderColorKeyAndBorder);
		}

		graphBuilder->AddFilter(vmr9, L"VMR9");


		CComQIPtr<IVMRSurfaceAllocatorNotify9> lpIVMRSurfAllocNotify(vmr9);
	    if (lpIVMRSurfAllocNotify)
		{
			allocatorNotify = lpIVMRSurfAllocNotify;
			lpIVMRSurfAllocNotify->AdviseSurfaceAllocator(0, this );
		}

		//CComPtr<IBaseFilter> source;
		//graphBuilder->AddSourceFilter(L"C:\\Windows\\Web\\Windows DreamScene\\bliss.wmv", L"Source", &source);
		graphBuilder->RenderFile(fileName, NULL);
		//captureGraphBuilder.CoCreateInstance(CLSID_CaptureGraphBuilder2);
		//if(captureGraphBuilder)
		//{
		//	captureGraphBuilder->SetFiltergraph(graphBuilder);
		//	captureGraphBuilder->RenderStream(0, 0, source, 0, vmr9);
			mediaControl->Run();
		//}
	}

	return S_OK;
}


HRESULT CDesktopEffectVideoWallpaper::PresentImage(
    DWORD_PTR dwUserID, 
    VMR9PresentationInfo *lpPresInfo)
{
	HRESULT hr = S_OK;

	HRESULT hRes = directDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	hr = directDevice->BeginScene();
	host->D3DRenderIconBuffer(AVE_D3D_RENDERICONBUFFER_FULL);

    // If we created a private texture, blit the decoded image onto it.
    if ( videoTexture != NULL )
    {   
		if(lpPresInfo && lpPresInfo->lpSurf)
		{
			CComPtr<IDirect3DSurface9> surface;
			hr = videoTexture->GetSurfaceLevel( 0 , & surface.p );
			if (FAILED(hr))
			{
				return hr;
			}
			// Copy the full surface onto the texture's surface.
			hr = directDevice->StretchRect( lpPresInfo->lpSurf, NULL, surface, NULL, D3DTEXF_NONE );
			if (FAILED(hr))
			{
				return hr;
			}
			renderTexture.Release();
			renderTexture = videoTexture;
		}
		directDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		directDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		directDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        hRes = directDevice->SetTexture(0,videoTexture);
		hRes = directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
        if (FAILED(hr))
        {
            return hr;
        }
    }
    else if(lpPresInfo && lpPresInfo->lpSurf) // The texture was allocated by the VMR-9.
    {
		renderTexture.Release();
        hr = lpPresInfo->lpSurf->GetContainer( IID_IDirect3DTexture9,
           (LPVOID*) &renderTexture );
        if (FAILED(hr))
        {
            return hr;
        }
		directDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		directDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		directDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        hRes = directDevice->SetTexture(0,renderTexture);
		hRes = directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
        if (FAILED(hr))
        {
            return hr;
        }
    }
	

	host->D3DRenderIconBuffer(AVE_D3D_RENDERICONBUFFER_FULL);
	hr  = directDevice->EndScene();
    hr = directDevice->Present( NULL, NULL, NULL, NULL );

    return hr;
}



HRESULT CDesktopEffectVideoWallpaper::StopVMR()
{
	if(mediaControl)
	{
		mediaControl->Stop();
		mediaControl.Release();
	}

	mediaEvent.Release();
	mediaPosition.Release();
	graphBuilder.Release();
	captureGraphBuilder.Release();
	allocatorNotify.Release();
	ClearSurfaces();
	renderTexture.Release();
	if(videoTexture != NULL)
		videoTexture->Release();
	videoTexture = NULL;

	return S_OK;
}


// Called when D3D mode is started. Gives a Pointer to an IUnknown that implements
// IDirect3DDevice9.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnD3DStart(IUnknown* directDeviceAsUnknown)
{
	// D3D example:
	if(NULL == directDeviceAsUnknown)
		return E_INVALIDARG;

	HWND hwnd = NULL;
	host->GetTargetWindow(&hwnd);
	
	CLSID clsid = {0};
	CLSIDFromString(L"{D0223B96-BF7A-43fd-92BD-A43B0D82B9EB}", &clsid);
	directDeviceAsUnknown->QueryInterface(clsid, (void**)&directDevice);
	if(NULL == directDevice)
		return E_INVALIDARG;


	StopVMR();
	StartVMR();

	/*
	HWND hwnd = NULL;
	host->GetTargetWindow(&hwnd);
	if(hwnd != NULL)
		SetTimer(hwnd, 1001, 20, 0);
	*/

	return S_OK;
}

// Called when D3D mode is stopped.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnD3DStop(void)
{
	HWND hwnd = NULL;
	host->GetTargetWindow(&hwnd);
	if(hwnd != NULL)
		KillTimer(hwnd, 1001);

	StopVMR();

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
STDMETHODIMP CDesktopEffectVideoWallpaper::OnBeforeGdiBackgroundUpdate(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo)
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
STDMETHODIMP CDesktopEffectVideoWallpaper::OnAfterGdiBackgroundUpdate(HDC hdc, const RECT* updateRect)
{
	if(NULL == updateRect || NULL == hdc)
		return E_FAIL;

	return S_OK;
}

// Called before the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnBeforeIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnAfterIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene needs to be rendered when in D3D mode.
// Set pHasRendered to TRUE if the scene has already been rendered by
// this method. Setting pHasRendered to TRUE allows an Effect to
// modify the complete render-cycle.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnD3DRender(BOOL* pHasRendered)
{
	// TODO: Add your implementation code here
	*pHasRendered = TRUE;
	HWND hwnd = NULL;
	host->GetTargetWindow(&hwnd);
	if(hwnd != NULL)
		PostMessage(hwnd, WM_AVE_START_RENDERING, 0, 0);

	return S_OK;
}

// Called before the D3D render cycle is started, if it has not been taken
// over by OnD3DRender().
// Set pBgColor to the background desired color of the scene.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnBeforeD3DRender(DWORD* pBgColor)
{
	if(NULL == pBgColor)
		return E_INVALIDARG;

	// Clear with solid black.
	//*pBgColor = 0xFF000000;

	return S_OK;
}

// Called before the D3D scene is started.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnBeforeD3DSceneStart(void)
{

	return S_OK;
}

static float v = 0.0f;

// Called after the D3D scene is started.
// This is the place for this Effect to draw its own elements into the
// scene.
// By default, the Device is in a 2D render state with a quad VertexBuffer
// that is the size of the desktop, with direct mapping texture coords.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnD3DSceneStart(void)
{
	HRESULT hRes = directDevice->SetTexture(0, renderTexture);
	hRes = directDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	return S_OK;
}

// Called before the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnBeforeIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnAfterIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene has ended.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnD3DSceneEnd(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnBeforeD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnAfterD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the whole D3D render loop has been finished.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnAfterD3DRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the target window (the desktops listview often) receives a message.
// set lResult to the desired return value of the MsgProc and set bHandled to TRUE
// if this message should not be passed to the original MsgProc of the target window.
// If mouse interactivity is needed, this is the place to track mouse movements for example
// by monitorin WM_MOUSEMOVE.
STDMETHODIMP CDesktopEffectVideoWallpaper::OnTargetWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{
	if(WM_AVE_START_RENDERING == msg)
	{
		PresentImage(0, NULL);
	}
	else if(WM_AVE_MEDIA_EVENT == msg)
	{
		if(mediaEvent)
		{
			 long lEventCode;
			long lParam1;
			long lParam2;
			// Check for completion events
			HRESULT hr = mediaEvent->GetEvent(&lEventCode, (LONG_PTR *) &lParam1, (LONG_PTR *) &lParam2, 0);
			 if (SUCCEEDED(hr))
			{
				// If we have reached the end of the media file, reset to beginning
				if (EC_COMPLETE == lEventCode) 
				{
					hr = mediaPosition->put_CurrentPosition(0);
				}

			 // Free any memory associated with this event
				hr = mediaEvent->FreeEventParams(lEventCode, lParam1, lParam2);
			}
		}
	}
	else if(WM_TIMER == msg && 1001 == wParam)
	{
		*bHandled = TRUE;
		
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
STDMETHODIMP CDesktopEffectVideoWallpaper::GetConfigurationWindow(HWND* hwnd, HWND parent)
{
	if(NULL == hwnd)
		return E_INVALIDARG;

	configDlg.Create(parent);
	*hwnd = configDlg.m_hWnd;

	return S_OK;
}

// Called when there is a notification for the Effect.
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectVideoWallpaper::OnNotification(DWORD dwNotification)
{
	return S_OK;
}

STDMETHODIMP CDesktopEffectVideoWallpaper::DoesSupport(DWORD* pFlag)
{
	if(NULL == pFlag)
		return E_INVALIDARG;

	*pFlag = AVE_SUPPORTS_CONFIG;

	return S_OK;
}
