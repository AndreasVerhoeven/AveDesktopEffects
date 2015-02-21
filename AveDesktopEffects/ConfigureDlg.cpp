#include "StdAfx.h"
#include "ConfigureDlg.h"
#include "AveDesktopEffectsHost.h"

CConfigureDlg::CConfigureDlg(CAveDesktopEffectsHost* host) : host(host)
{
}

CConfigureDlg::~CConfigureDlg(void)
{
}


extern Gdiplus::Bitmap* LoadBitmapResource(HINSTANCE instance, LPCTSTR section, LPCTSTR resname);

LRESULT CConfigureDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	prevSel = -1;
	hwndConfigEffect = NULL;
	RECT rc = {0};
	GetWindowRect(&rc);
	origSize.cx = rc.right - rc.left;
	origSize.cy = rc.bottom - rc.top;

	if(USE_TIMEBOMB)
	{
		SYSTEMTIME bomb = {0};
		bomb.wYear =  TIMEBOMB_DATE_YEAR;
		bomb.wMonth =  TIMEBOMB_DATE_MONTH;
		bomb.wDay =  TIMEBOMB_DATE_DAY;

		WCHAR str[1024] = {0};
		GetDateFormat(LOCALE_USER_DEFAULT, 0, &bomb, L"dddd d MMMM, yyyy", str, 1024);
		SetDlgItemText(IDC_EXPIRE2, str);

		GetDlgItem(IDC_EXPIRE1).ShowWindow(TRUE);
		GetDlgItem(IDC_EXPIRE2).ShowWindow(TRUE);
	}

	ModifyStyleEx(0,WS_EX_APPWINDOW); // for appearance in task bar
   //HICON hIco = LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE( IDI_ICON2 ));
   // SetIcon( hIco, TRUE );
    //SetIcon( hIco, FALSE );
    CenterWindow();

	effectsList.SubclassWindow(GetDlgItem(IDC_EFFECTS));


	CEffectLBItem defaultItem;
	defaultItem.name.LoadString(IDS_NOEFFECTNAME);
	defaultItem.description.LoadString(IDS_NOEFFECTDESCRIPTION);
	Bitmap* defaultIcon = LoadBitmapResource(_AtlBaseModule.GetResourceInstance(), L"PNG", MAKEINTRESOURCE(IDR_PNG3));
	if(defaultIcon != NULL)
	{
		defaultIcon->GetHBITMAP(NULL, &defaultItem.icon);
		delete defaultIcon;
	}
	int index = effectsList.AddItem(defaultItem);
	effectsList.SetCurSel(index);

	host->FillEffectInfos();
	for(CAveDesktopEffectsHost::EffectInfoIterator iter = host->effectInfos.begin(); iter != host->effectInfos.end(); ++iter)
	{
		CEffectLBItem item;
		item.clsid = iter->clsid;
		item.name = iter->name;
		item.author = iter->author;
		item.description = iter->description;
		item.icon = iter->image;
		int index = effectsList.AddItem(item);

		for(std::vector<CString>::iterator iter = host->effectCLSIDs.begin(); iter != host->effectCLSIDs.end(); ++iter)
		{
			if(_wcsicmp(item.clsid, *iter) == 0)
				effectsList.SetCurSel(index);
				
		}
	}

	BOOL enableConfig = FALSE;
	if(host->effects.size() > 0)
	{
		DWORD supports=0;
		host->effects[0]->DoesSupport(&supports);
		enableConfig = AVE_SUPPORTS_CONFIG & supports;
	}

	GetDlgItem(IDCONFIG).EnableWindow(enableConfig);

	VARIANT_BOOL isRunning = VARIANT_FALSE;
	host->IsRunning(&isRunning);
	CString toggleButtonText;
	toggleButtonText.LoadStringW( isRunning != VARIANT_FALSE ? IDS_STOP : IDS_START);
	GetDlgItem(IDTOGGLE).SetWindowTextW(toggleButtonText);

	return 0;
}

LRESULT CConfigureDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(hwndConfigEffect != NULL)
	{
		::DestroyWindow(hwndConfigEffect);
		hwndConfigEffect = NULL;
	}

	host->ClearEffectInfos();
	CoFreeUnusedLibraries();
	return 0;
}

LRESULT CConfigureDlg::OnCloseDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT CConfigureDlg::OnAveStatusChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CString toggleButtonText;
	toggleButtonText.LoadStringW( wParam ? IDS_STOP : IDS_START);
	GetDlgItem(IDTOGGLE).SetWindowTextW(toggleButtonText);
	return 0;
}

LRESULT CConfigureDlg::OnToggleButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	VARIANT_BOOL isRunning = VARIANT_FALSE;
	host->IsRunning(&isRunning);
	if(isRunning != VARIANT_FALSE)
	{
		host->Stop();
	}
	else
	{
		if(host->effects.size() == 0)
		{
			int sel = effectsList.GetCurSel();
			if(LB_ERR == sel)
				return 0;

			size_t index = (size_t)effectsList.GetItemData(sel);
			if(index >=0 && index < host->effectInfos.size())
			{
				HRESULT hRes = host->LoadPluginByCLSID(host->effectInfos[index].clsid);
			}
		}
		host->Start();
	}

	return 0;
}


LRESULT CConfigureDlg::OnCloseButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(hwndConfigEffect)
	{
		SendMessage(hwndConfigEffect, WM_AVE_EFFECTCONFIG_OK, 0, 0);
		GoBackToNormalConfig();
		effectsList.SetFocus();
	}
	else
	{
		DestroyWindow();
	}

	return 0;
}

void CConfigureDlg::GoBackToNormalConfig()
{
	if(hwndConfigEffect != NULL)
	{
		CString okBut;
		okBut.LoadStringW(IDS_CONFIGOK);
		GetDlgItem(IDOK).SetWindowText(okBut);
		GetDlgItem(IDOK).SetFocus();
		::DestroyWindow(hwndConfigEffect);
		GetDlgItem(IDC_TOPLABEL).ShowWindow(SW_SHOW);
		effectsList.ShowWindow(SW_SHOW);
		GetDlgItem(IDCONFIG).ShowWindow(SW_SHOW);

		hwndConfigEffect = NULL;

		CString title;
		title.LoadString(IDS_CONFIGTITLE);
		SetWindowText(title);
	}
}

HRESULT CConfigureDlg::ShowConfigForEffect(IAveDesktopEffect* effect)
{
	if(NULL == effect)
		return E_FAIL;

	HRESULT hRes = effect->GetConfigurationWindow(&hwndConfigEffect, m_hWnd);
	if(hwndConfigEffect != NULL)
	{
		::SetParent(hwndConfigEffect, m_hWnd);
		RECT rc = {0};
		::GetWindowRect(hwndConfigEffect, &rc);
		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;
		::SetWindowPos(hwndConfigEffect, HWND_TOP, 0, 0, w, h, SWP_SHOWWINDOW);
		SetWindowPos(NULL, 0, 0, max(w+20, origSize.cx), max(h+40, origSize.cy), SWP_NOMOVE | SWP_NOZORDER);

		CString backBut;
		backBut.LoadStringW(IDS_CONFIGBACK);
		GetDlgItem(IDOK).SetWindowText(backBut);
		GetDlgItem(IDOK).SetFocus();
		GetDlgItem(IDC_TOPLABEL).ShowWindow(SW_HIDE);
		GetDlgItem(IDCONFIG).ShowWindow(SW_HIDE);
		effectsList.ShowWindow(SW_HIDE);

		BSTR name = NULL;
		effect->GetName(&name);
		if(name != NULL)
			SetWindowText(name);
		SysFreeString(name);

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

LRESULT CConfigureDlg::OnLbnSelchangeEffects(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int sel = effectsList.GetCurSel();
	if(LB_ERR == sel)
		return 0;

	if(sel == prevSel)
		return 0;

	prevSel = sel;

	CEffectLBItem& item = effectsList.GetItem(sel);

	BOOL isActive=FALSE;
	for(std::vector<CString>::iterator iter = host->effectCLSIDs.begin(); iter != host->effectCLSIDs.end(); ++iter)
	{
		if(_wcsicmp(item.clsid, *iter) == 0)
		{
			isActive = TRUE;
			break;
		}
	}
	
	if(!isActive)
	{
		HRESULT hRes = host->LoadPluginByCLSID(CComBSTR(item.clsid));
	}

	BOOL enableConfig = FALSE;
	if(host->effects.size() > 0)
	{
		DWORD supports=0;
		if(host->effects[0] != NULL)
			host->effects[0]->DoesSupport(&supports);
		enableConfig = AVE_SUPPORTS_CONFIG & supports;
	}

	GetDlgItem(IDCONFIG).EnableWindow(enableConfig);

	VARIANT_BOOL isRunning = VARIANT_FALSE;
	host->IsRunning(&isRunning);
	if(!isRunning && !item.clsid.IsEmpty())
		host->Start();

	return 0;
}

LRESULT CConfigureDlg::OnLbnDblclkEffects(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(host->effects.size() > 0)
	{
		ShowConfigForEffect(host->effects[0]);
	}

	//size_t index = (size_t)effectsList.GetItemData(sel);
	//if(index >=0 && index < host->effectInfos.size())
	{
		//HRESULT hRes = host->LoadPluginByCLSID(host->effectInfos[index].clsid);
	}

	return 0;
}

LRESULT CConfigureDlg::OnBnClickedConfig(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(host->effects.size() > 0)
	{
		ShowConfigForEffect(host->effects[0]);
	}

	return 0;
}
