// AveDesktopEffects.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "AveDesktopEffects.h"
#include "dlldatax.h"

// data shared between all instances of this DLL
#pragma data_seg(".AVEDESKTOPEFFECTS")
BOOL g_HasBeenInitialized=FALSE;
HWND g_NotificationWindow = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:.AVEDESKTOPEFFECTS,rws")


BOOL IsRunningInExplorer()
{
	WCHAR path[MAX_PATH] = {0};
	GetModuleFileName(NULL, path, MAX_PATH);
	const WCHAR* fileName = PathFindFileName(path);
	return _wcsicmp(fileName, L"explorer.exe") == 0 || _wcsicmp(fileName, L"verclsid.exe") == 0;
}

class CAveDesktopEffectsModule : public CAtlDllModuleT< CAveDesktopEffectsModule >
{
public :
	DECLARE_LIBID(LIBID_AveDesktopEffectsLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AVEDESKTOPEFFECTS, "{BC3CBDE9-BB8A-4B40-801F-68E3D9500377}")
};

CAveDesktopEffectsModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(hInstance, dwReason, lpReserved))
        return FALSE;
#endif
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hr = PrxDllCanUnloadNow();
    if (hr != S_OK)
        return hr;
#endif
    return _AtlModule.DllCanUnloadNow();
}


extern HWND CreateNotificationWindow();

// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	if(!IsRunningInExplorer())
		return E_FAIL;

	if(!CheckTimeBomb())
		return E_FAIL;

	if(!g_HasBeenInitialized)
	{
		g_HasBeenInitialized = TRUE;
		g_NotificationWindow = CreateNotificationWindow();
		if(g_NotificationWindow != NULL)
			PostMessage(g_NotificationWindow, WM_AVE_FIRE_IT_UP, 0, 0);
	}

#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
#endif
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
    if (FAILED(hr))
        return hr;
    hr = PrxDllUnregisterServer();
#endif
	return hr;
}

