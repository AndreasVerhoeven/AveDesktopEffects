// DesktopEffectWaterWallpaper.cpp : Implementation of CDesktopEffectWaterWallpaper

#include "stdafx.h"
#include "DesktopEffectWaterWallpaper.h"

enum TIMER_COMMAND { TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE,
                     TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME };
//-----------------------------------------------------------------------------
// Name: DXUtil_Timer()
// Desc: Performs timer opertations. Use the following commands:
//          TIMER_RESET           - to reset the timer
//          TIMER_START           - to start the timer
//          TIMER_STOP            - to stop (or pause) the timer
//          TIMER_ADVANCE         - to advance the timer by 0.1 seconds
//          TIMER_GETABSOLUTETIME - to get the absolute system time
//          TIMER_GETAPPTIME      - to get the current time
//          TIMER_GETELAPSEDTIME  - to get the time that elapsed between 
//                                  TIMER_GETELAPSEDTIME calls
//-----------------------------------------------------------------------------
FLOAT __stdcall DXUtil_Timer( TIMER_COMMAND command )
{
    static BOOL     m_bTimerInitialized = FALSE;
    static BOOL     m_bUsingQPF         = FALSE;
    static BOOL     m_bTimerStopped     = TRUE;
    static LONGLONG m_llQPFTicksPerSec  = 0;

    // Initialize the timer
    if( FALSE == m_bTimerInitialized )
    {
        m_bTimerInitialized = TRUE;

        // Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
        // not supported, we will timeGetTime() which returns milliseconds.
        LARGE_INTEGER qwTicksPerSec;
        m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
        if( m_bUsingQPF )
            m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

    if( m_bUsingQPF )
    {
        static LONGLONG m_llStopTime        = 0;
        static LONGLONG m_llLastElapsedTime = 0;
        static LONGLONG m_llBaseTime        = 0;
        double fTime;
        double fElapsedTime;
        LARGE_INTEGER qwTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            qwTime.QuadPart = m_llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {
            fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
            m_llLastElapsedTime = qwTime.QuadPart;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
            return (FLOAT) fAppTime;
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_llBaseTime        = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_llStopTime        = 0;
            m_bTimerStopped     = FALSE;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            if( m_bTimerStopped )
                m_llBaseTime += qwTime.QuadPart - m_llStopTime;
            m_llStopTime = 0;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_bTimerStopped = FALSE;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            if( !m_bTimerStopped )
            {
                m_llStopTime = qwTime.QuadPart;
                m_llLastElapsedTime = qwTime.QuadPart;
                m_bTimerStopped = TRUE;
            }
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_llStopTime += m_llQPFTicksPerSec/10;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
    else
    {
        // Get the time using timeGetTime()
        static double m_fLastElapsedTime  = 0.0;
        static double m_fBaseTime         = 0.0;
        static double m_fStopTime         = 0.0;
        double fTime;
        double fElapsedTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            fTime = m_fStopTime;
        else
            fTime = GetTickCount() * 0.001;
    
        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {   
            fElapsedTime = (double) (fTime - m_fLastElapsedTime);
            m_fLastElapsedTime = fTime;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            return (FLOAT) (fTime - m_fBaseTime);
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_fBaseTime         = fTime;
            m_fLastElapsedTime  = fTime;
            m_fStopTime         = 0;
            m_bTimerStopped     = FALSE;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            if( m_bTimerStopped )
                m_fBaseTime += fTime - m_fStopTime;
            m_fStopTime = 0.0f;
            m_fLastElapsedTime  = fTime;
            m_bTimerStopped = FALSE;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            if( !m_bTimerStopped )
            {
                m_fStopTime = fTime;
                m_fLastElapsedTime  = fTime;
                m_bTimerStopped = TRUE;
            }
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_fStopTime += 0.1f;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
}




inline FLOAT rnd() { return (((FLOAT)rand()-(FLOAT)rand())/(2L*RAND_MAX)); }
inline FLOAT RND() { return (((FLOAT)rand())/RAND_MAX); }
inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

struct BUMPVERTEX
{
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    FLOAT       tu1, tv1;
    FLOAT       tu2, tv2;

    static const DWORD FVF;
};
const DWORD BUMPVERTEX::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;


// Called when the name of this effect is required
STDMETHODIMP CDesktopEffectWaterWallpaper::GetName(BSTR* pName)
{
	if(NULL == pName)
		return E_INVALIDARG;

	CString name;
	name.LoadString(IDS_EFFECTNAME);
	*pName = SysAllocString(name);

	return S_OK;
}

// Called when the author of this effect is required
STDMETHODIMP CDesktopEffectWaterWallpaper::GetAuthor(BSTR* pAuthor)
{
	if(NULL == pAuthor)
		return E_INVALIDARG;

	CString author;
	author.LoadString(IDS_AUTHORNAME);
	*pAuthor = SysAllocString(author);

	return S_OK;
}

// Called when the description of this effect is required
STDMETHODIMP CDesktopEffectWaterWallpaper::GetDescription(BSTR* pDescription)
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
STDMETHODIMP CDesktopEffectWaterWallpaper::GetIcon(HBITMAP* pHBitmap)
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
STDMETHODIMP CDesktopEffectWaterWallpaper::GetCooperationFlags(DWORD* pFlags)
{
	// NOT IMPLEMENTED CURRENTLY

	return S_OK;
}

// Called when this effect is loaded and initialized.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnInitialize(IAveDesktopEffectsHost* host)
{
	// Store a ptr to our host
	this->host = host;
	host->AddRef();

	return S_OK;
}

// Called when this effect is unloaded
STDMETHODIMP CDesktopEffectWaterWallpaper::OnUninitialize(void)
{
	_DestroyScribbleBuffer();

	// Release our ptr to our host
	host->Release();

	return S_OK;
}

HRESULT CDesktopEffectWaterWallpaper::_DestroyScribbleBuffer()
{
	HRESULT hRes = S_OK;

	if(texture != NULL)
	{
		texture->Release();
		texture = NULL;
	}

	/*
	if(bufferBitmap != NULL)
		DeleteObject(bufferBitmap);

	if(bufferDC != NULL)
		DeleteDC(bufferDC);

	bufferDC = NULL;
	bufferBitmap = NULL;
	bufferBits = NULL;
	bufferSize.cx = 0;
	bufferSize.cy = 0;
	*/
	return hRes;
}

HRESULT CDesktopEffectWaterWallpaper::_CreateScribbleBuffer()
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


	CComPtr<IUnknown> unknown;
	host->GetWallpaperTexture(&unknown);
	if(unknown != NULL)
		unknown.QueryInterface(&texture);

	return S_OK;



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
    bi.bV5AlphaMask		= 0x00000000;

	bufferBitmap = CreateDIBSection(bufferDC, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&bufferBits, NULL, 0);
	if(NULL == bufferBitmap)
	{
		_DestroyScribbleBuffer();
		return E_FAIL;
	}

	SelectObject(bufferDC, bufferBitmap);
	RECT rcFill = {0,0,bufferSize.cx, bufferSize.cy};
	HBRUSH white = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(bufferDC, &rcFill, white);
	
	hwnd = NULL;
	host->GetTargetWindow(&hwnd);
	if(hwnd != NULL)
	{
		hwnd = GetParent(hwnd);
		SendMessage(hwnd, WM_PRINT, (WPARAM)bufferDC, PRF_ERASEBKGND | PRF_CLIENT); 
	}

	return S_OK;
}

// Called when DesktopEffects rendering is started
STDMETHODIMP CDesktopEffectWaterWallpaper::OnStart(void)
{
	// D3D example:
	host->StartD3DMode();

	return S_OK;
}

// Called when DesktopEffects rendering is stopped
STDMETHODIMP CDesktopEffectWaterWallpaper::OnStop(void)
{
	//D3D example:
	if(directDevice != NULL)
		host->StopD3DMode();

	return S_OK;
}

// Called when DesktopEffects is paused
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectWaterWallpaper::OnPause(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when DesktopEffects is resumed from a paused state
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectWaterWallpaper::OnResume(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}


HRESULT CDesktopEffectWaterWallpaper::_CreateBumpMap()
{
    UINT iWidth  = 256;//bufferSize.cx;
    UINT iHeight = 256;//bufferSize.cy;

    // Create the bumpmap's surface and texture objects
    if( FAILED( directDevice->CreateTexture( iWidth, iHeight, 1, D3DUSAGE_DYNAMIC,
        D3DFMT_V8U8, D3DPOOL_DEFAULT, &m_pBumpMap, NULL ) ) )
    {
        return E_FAIL;
    }

	_InitBumpMap();

    return S_OK;
}

HRESULT CDesktopEffectWaterWallpaper::_InitBumpMap()
{
	if(NULL == m_pBumpMap)
		return E_FAIL;

	UINT iWidth  = 256;//bufferSize.cx;
    UINT iHeight = 256;//bufferSize.cy;

    // Fill the bumpmap texels to simulate a lens
    D3DLOCKED_RECT lrDst;
    m_pBumpMap->LockRect( 0, &lrDst, 0, 0 );
    BYTE* pDst       = (BYTE*)lrDst.pBits;
	if(!pDst)
		return E_FAIL;

    for( DWORD y=0; y<iHeight; y++ )
    {
        for( DWORD x=0; x<iWidth; x++ )
        {
            FLOAT fx = x/(FLOAT)iWidth  - 0.5f;
            FLOAT fy = y/(FLOAT)iHeight - 0.5f;

            CHAR iDu = (CHAR)(waveHeight*cosf(float(waveLength)*(fx+fy)*D3DX_PI));
            CHAR iDv = (CHAR)(waveHeight*sinf(float(waveLength)*(fx+fy)*D3DX_PI));

			pDst[2*x+0] = iDu;
            pDst[2*x+1] = iDv;
        }
        pDst += lrDst.Pitch;
    }

    m_pBumpMap->UnlockRect(0);

	return S_OK;
}

// Called when D3D mode is started. Gives a Pointer to an IUnknown that implements
// IDirect3DDevice9.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnD3DStart(IUnknown* directDeviceAsUnknown)
{
	// D3D example:
	if(NULL == directDeviceAsUnknown)
		return E_INVALIDARG;
	
	CLSID clsid = {0};
	CLSIDFromString(L"{D0223B96-BF7A-43fd-92BD-A43B0D82B9EB}", &clsid);
	directDeviceAsUnknown->QueryInterface(clsid, (void**)&directDevice);
	if(NULL == directDevice)
		return E_INVALIDARG;

	_CreateScribbleBuffer();

	_CreateBumpMap();
	 if( FAILED( directDevice->CreateVertexBuffer( 4*sizeof(BUMPVERTEX),
                                                  D3DUSAGE_WRITEONLY,
                                                  BUMPVERTEX::FVF,
                                                  D3DPOOL_DEFAULT, &m_pWaterVB, NULL ) ) )
        return E_FAIL;

	float fwidth  = float(bufferSize.cx);
	float fheight = float(bufferSize.cy);

    BUMPVERTEX* v;
    m_pWaterVB->Lock( 0, 0, (void**)&v, 0 );
    v[0].p = D3DXVECTOR3(-fwidth/2.0f,fheight/2.0f, 0.0f ); v[0].n = D3DXVECTOR3( 0, 1, 0 );
    v[1].p = D3DXVECTOR3(-fwidth/2.0f, -fheight/2.0f, 0.0f ); v[1].n = D3DXVECTOR3( 0, 1, 0 );
    v[2].p = D3DXVECTOR3( fwidth/2.0f,fheight/2.0f, 0.0f ); v[2].n = D3DXVECTOR3( 0, 1, 0 );
    v[3].p = D3DXVECTOR3( fwidth/2.0f, -fheight/2.0f, 0.0f ); v[3].n = D3DXVECTOR3( 0, 1, 0 );
    v[0].tu2 = 0.000f; v[0].tv2 = 1.0f;
    v[1].tu2 = 0.000f; v[1].tv2 = 0.0f;
    v[2].tu2 = 1.000f; v[2].tv2 = 1.0f;
    v[3].tu2 = 1.000f; v[3].tv2 = 0.0f;
    m_pWaterVB->Unlock();
	
	// Create the texture using D3DX
	
	/*
    D3DXCreateTextureFromFileEx( directDevice, L"F:\\SDKs\\directx9\\Samples\\Media\\lobbyxpos.JPG", 
                     D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, 
                     D3DPOOL_DEFAULT, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                     D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, &texture );*/

	
	/*D3DXCreateTextureFromFileEx(directDevice, L"c:\\walla.jpg", 0, 0, 1, 0, D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR | D3DX_FILTER_MIRROR, 0, 0, NULL, NULL, &texture);*/
	
/*
	HRESULT hRes = directDevice->CreateTexture(bufferSize.cx, bufferSize.cy,
						1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
	
	if(NULL == texture)
		return S_OK;

	D3DLOCKED_RECT locked = {0};
	RECT rc = {0,0, bufferSize.cx, bufferSize.cy};
	IDirect3DSurface9* surface = NULL;
	texture->GetSurfaceLevel(0, &surface);
	if(surface != NULL)
	{
		if(SUCCEEDED(surface->LockRect(&locked, &rc, 0)))
		{
			GetBitmapBits(bufferBitmap, bufferSize.cx * bufferSize.cy * sizeof(DWORD), locked.pBits);
			surface->UnlockRect();
		}
		surface->Release();
	}	*/
	
	HWND hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
		SetTimer(hwnd, 1001, 20, 0);

	return S_OK;
}

// Called when D3D mode is stopped.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnD3DStop(void)
{
	HWND hwnd = NULL;
	host->GetHelperWindow(this, &hwnd);
	if(hwnd != NULL)
		KillTimer(hwnd, 1001);

	if(m_pWaterVB)m_pWaterVB->Release();m_pWaterVB=NULL;
    if(m_pBumpMap)m_pBumpMap->Release();m_pBumpMap=NULL;

	_DestroyScribbleBuffer();

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
STDMETHODIMP CDesktopEffectWaterWallpaper::OnBeforeGdiBackgroundUpdate(HDC hdc, const RECT* updateRect, DWORD* pWhatToDo)
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
STDMETHODIMP CDesktopEffectWaterWallpaper::OnAfterGdiBackgroundUpdate(HDC hdc, const RECT* updateRect)
{
	if(NULL == updateRect || NULL == hdc)
		return E_FAIL;

	return S_OK;
}

// Called before the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnBeforeIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the internal icon buffer is updated, when in D3D mode.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnAfterIconBufferUpdate(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene needs to be rendered when in D3D mode.
// Set pHasRendered to TRUE if the scene has already been rendered by
// this method. Setting pHasRendered to TRUE allows an Effect to
// modify the complete render-cycle.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnD3DRender(BOOL* pHasRendered)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D render cycle is started, if it has not been taken
// over by OnD3DRender().
// Set pBgColor to the background desired color of the scene.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnBeforeD3DRender(DWORD* pBgColor)
{
	if(NULL == pBgColor)
		return E_INVALIDARG;

	// Clear with solid black.
	//*pBgColor = 0xFF000000;

	return S_OK;
}

// Called before the D3D scene is started.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnBeforeD3DSceneStart(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

static float v = 0.0f;

// Called after the D3D scene is started.
// This is the place for this Effect to draw its own elements into the
// scene.
// By default, the Device is in a 2D render state with a quad VertexBuffer
// that is the size of the desktop, with direct mapping texture coords.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnD3DSceneStart(void)
{
	if(texture != NULL)
	{
		/*
		D3DXVECTOR3 vEyePt    = D3DXVECTOR3( 0.0f, 0.0f, -100.0f );
		D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f,    0.0f );
		D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f,    0.0f );
		D3DXMATRIXA16 matWorld, matView, matProj;

		D3DXMatrixIdentity( &matWorld );
		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		D3DXMatrixPerspectiveFovLH( &matProj, 1.00f, 1.0f, 1.0f, 3000.0f );
		directDevice->SetTransform( D3DTS_WORLD,      &matWorld );
		directDevice->SetTransform( D3DTS_VIEW,       &matView );
		directDevice->SetTransform( D3DTS_PROJECTION, &matProj );*/

		// Set any appropiate state
		directDevice->SetRenderState( D3DRS_AMBIENT,        0xffffffff );
		directDevice->SetRenderState( D3DRS_DITHERENABLE,   TRUE );
		directDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
		directDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		directDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		directDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		directDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		directDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		directDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		directDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

		directDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_fTime = DXUtil_Timer( TIMER_GETAPPTIME );
		directDevice->SetTextureStageState( 0, D3DTSS_BUMPENVMAT00, F2DW(0.01f) );
		directDevice->SetTextureStageState( 0, D3DTSS_BUMPENVMAT01, F2DW(0.00f) );
		directDevice->SetTextureStageState( 0, D3DTSS_BUMPENVMAT10, F2DW(0.00f) );
		directDevice->SetTextureStageState( 0, D3DTSS_BUMPENVMAT11, F2DW(0.01f) );

		if(doRandom)
		{
			if(randomGoingUp)
			{
				if(waveDistortion > waveDistortionMax)
				{
					waveDistortionMin = rand() % waveDistortionMax;
					randomGoingUp = FALSE;
				}
			}
			else
			{
				if(waveDistortion < waveDistortionMin)
				{
					waveDistortionMax = (rand() % (50000 - waveDistortionMin)) + waveDistortionMin;	
					randomGoingUp = TRUE;
				}
			}
			
			if(randomGoingUp)
				waveDistortion += rand() % 30;
			else
				waveDistortion -= rand() % 30;
		}


		float distortion = float(waveDistortion) / 10000.0f;
		float speed = float(waveSpeed) / 100.0f;
		BUMPVERTEX* vWaterVertices;
		m_pWaterVB->Lock( 0, 0, (void**)&vWaterVertices, 0 );
		vWaterVertices[0].tu1 =  0.000f; vWaterVertices[0].tv1 = speed*m_fTime + distortion;
		vWaterVertices[1].tu1 =  0.000f; vWaterVertices[1].tv1 = speed*m_fTime;
		vWaterVertices[2].tu1 =  1.000f; vWaterVertices[2].tv1 = speed*m_fTime;
		vWaterVertices[3].tu1 =  1.000f; vWaterVertices[3].tv1 = speed*m_fTime + distortion;
		m_pWaterVB->Unlock();

		// Render the waves
		directDevice->SetTexture( 0, m_pBumpMap );
		directDevice->SetTexture( 1, texture );

		
		directDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
		directDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_BUMPENVMAP );
		directDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//D3DTA_TEXTURE );
		directDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
		directDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

		directDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
		directDevice->SetTextureStageState( 1, D3DTSS_COLOROP,  D3DTOP_SELECTARG1);// D3DTOP_SELECTARG1 );
		//directDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );

		directDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
		directDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

		directDevice->SetTextureStageState( 2, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		directDevice->SetTextureStageState( 2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

		directDevice->SetFVF( BUMPVERTEX::FVF );
		directDevice->SetStreamSource( 0, m_pWaterVB, 0, sizeof(BUMPVERTEX) );

		directDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		directDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, 0 );
		directDevice->SetTexture( 1, NULL );

	}

	return S_OK;
}

// Called before the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnBeforeIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the icon buffer of the desktop is drawn
// ontop of the D3D scene.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnAfterIconBufferRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when the D3D scene has ended.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnD3DSceneEnd(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called before the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnBeforeD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the D3D scene is present()'d to the device.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnAfterD3DPresent(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called after the whole D3D render loop has been finished.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnAfterD3DRender(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}

// Called when there is a message for the notification window.
// Each effect gets its own notification helper window for use with timers,
// thread marshalling and such.
// By calling IAveDesktopEffectsHost::GetHelperWindow(this, &hwnd) this 
// window will be created and a handle to it will be obtained.
STDMETHODIMP CDesktopEffectWaterWallpaper::OnNotificationWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
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
STDMETHODIMP CDesktopEffectWaterWallpaper::OnTargetWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult, BOOL* bHandled)
{

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
STDMETHODIMP CDesktopEffectWaterWallpaper::GetConfigurationWindow(HWND* hwnd, HWND parent)
{
	if(NULL == hwnd)
		return E_INVALIDARG;

	// TODO: create a window that will be embedded into another window
	// here for configuration.

	return S_OK;
}

// Called when there is a notification for the Effect.
// CURRENTLY NOT IMPLEMENTED
STDMETHODIMP CDesktopEffectWaterWallpaper::OnNotification(DWORD dwNotification)
{
	// TODO: Add your implementation code here
	if(AVE_NOTIFICATION_AFTER_WALLPAPER_CHANGED == dwNotification)
	{
		_CreateScribbleBuffer();
	}

	return S_OK;
}

STDMETHODIMP CDesktopEffectWaterWallpaper::DoesSupport(DWORD* pFlag)
{
	if(NULL == pFlag)
		return E_INVALIDARG;

	*pFlag = AVE_SUPPORTS_D3D_DRAW_OVER_REAL_WALLPAPER;//AVE_SUPPORTS_CONFIG;

	return S_OK;
}
