#include "StdAfx.h"
#include "AveRegFuncs.h"

AveRegFuncs::AveRegFuncs(void)
{
}

AveRegFuncs::~AveRegFuncs(void)
{
}

HKEY	AveRegFuncs::OpenRegKey(HKEY hkey, const WCHAR* key)
{
	HKEY res = NULL;
	RegOpenKey(hkey, key, &res);
	return res;
}


void AveRegFuncs::WriteString(HKEY hkey, const WCHAR* key, const WCHAR* value, const WCHAR* str)
{
	SHSetValue(hkey, key, value, REG_SZ, str, DWORD(sizeof(WCHAR) * (wcslen(str) + 1)));
}

CString	AveRegFuncs::ReadString(HKEY hkey, const WCHAR* key, const WCHAR* value, const WCHAR* defVal)
{
	DWORD type = REG_SZ;
	DWORD size = 0;
	LRESULT lRes = SHGetValue(hkey, key, value, &type, NULL, &size);
	if(lRes != ERROR_SUCCESS)
		return defVal;

	WCHAR* str = new WCHAR[size/2];
	memset(str, 0, size);

	type = REG_SZ;
	SHGetValue(hkey, key, value, &type, str, &size);

	CString val = str;
	delete[] str;

	return val;
}

void AveRegFuncs::WriteInt(HKEY hkey, const WCHAR* key, const WCHAR* value, int val)
{
	DWORD dwVal = (DWORD)val;
	SHSetValue(hkey, key, value, REG_DWORD, &dwVal, sizeof(dwVal));
}

int	AveRegFuncs::ReadInt(HKEY hkey, const WCHAR* key, const WCHAR* value, int defVal)
{
	DWORD dwVal = 0;
	DWORD type = REG_DWORD;
	DWORD size = sizeof(dwVal);
	LRESULT lRes = SHGetValue(hkey, key, value, &type, &dwVal, &size);
	if(lRes != ERROR_SUCCESS)
		return defVal;

	return (int)dwVal;
}


void	AveRegFuncs::WriteBinary(HKEY hkey, const WCHAR* key, const WCHAR* value, LPVOID data, DWORD size)
{
	SHSetValue(hkey, key, value, REG_BINARY, data, size);
}

BOOL	AveRegFuncs::ReadBinary(HKEY hkey, const WCHAR* key, const WCHAR* value, LPVOID data, DWORD size)
{
	DWORD type = REG_BINARY;
	DWORD regSize = 0;
	LRESULT lRes = SHGetValue(hkey, key, value, &type, NULL, &regSize);
	if(lRes != ERROR_SUCCESS || regSize != size)
		return FALSE;

	SHGetValue(hkey, key, value, &type, data, &size);
	return true;
}

void	AveRegFuncs::WriteHGlobal(HKEY hkey, const WCHAR* key, const WCHAR* value, HGLOBAL glob)
{
	LPVOID data = GlobalLock(glob);
	DWORD size = (DWORD)GlobalSize(glob);
	SHSetValue(hkey, key, value, REG_BINARY, data, size);
	GlobalUnlock(glob);
}

HGLOBAL	AveRegFuncs::ReadHGlobal(HKEY hkey, const WCHAR* key, const WCHAR* value)
{
	DWORD type = REG_BINARY;
	DWORD size = 0;
	LRESULT lRes = SHGetValue(hkey, key, value, &type, NULL, &size);
	if(lRes != ERROR_SUCCESS || size <= 0)
		return NULL;

	HGLOBAL global =  GlobalAlloc(GMEM_MOVEABLE | GMEM_DISCARDABLE, (SIZE_T)size);
	if(NULL == global)
		return NULL;

	LPVOID data = GlobalLock(global);

	type = REG_BINARY;
	SHGetValue(hkey, key, value, &type, data, &size);

	GlobalUnlock(global);

	return global;
}