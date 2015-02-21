#pragma once

#include "stdafx.h"

class AveRegFuncs
{
public:
	AveRegFuncs(void);
	~AveRegFuncs(void);

	static HKEY		OpenRegKey(HKEY hkey, const WCHAR* key);

	static void		WriteString(HKEY hkey, const WCHAR* key, const WCHAR* value, const WCHAR* str);
	static CString	ReadString(HKEY hkey, const WCHAR* key, const WCHAR* value, const WCHAR* defVal);

	static void		WriteInt(HKEY hkey, const WCHAR* key, const WCHAR* value, int val);
	static int		ReadInt(HKEY hkey, const WCHAR* key, const WCHAR* value, int defVal);

	static void		WriteHGlobal(HKEY hkey, const WCHAR* key, const WCHAR* value, HGLOBAL glob);
	static HGLOBAL	ReadHGlobal(HKEY hkey, const WCHAR* key, const WCHAR* value);

	static void		WriteBinary(HKEY hkey, const WCHAR* key, const WCHAR* value, LPVOID data, DWORD size);
	static BOOL		ReadBinary(HKEY hkey, const WCHAR* key, const WCHAR* value, LPVOID data, DWORD size);
};
