#include "StdAfx.h"
#include "ConfigureDlg.h"
#include "DesktopEffectVideoWallpaper.h"


CConfigureDlg::CConfigureDlg(CDesktopEffectVideoWallpaper* effect) : effect(effect)
{
}

CConfigureDlg::~CConfigureDlg(void)
{
}


LRESULT CConfigureDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetDlgItemText(IDC_VIDEO, effect->GetFileName());
	return 0;
}

LRESULT CConfigureDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CConfigureDlg::OnBnBrowseConfig(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog dlg(TRUE, L"*.avi;*.mpg;*.wmv", effect->GetFileName(), 4|2, L"*.avi;*.mpg;*.wmv\0*.avi;*.mpg;*.wmv\0*.*\0*.*\0\0", GetParent());
	if(dlg.DoModal(GetParent()))
	{
		SetDlgItemText(IDC_VIDEO, dlg.m_szFileName);
		effect->SetFileName(dlg.m_szFileName);
	}
	
	return 0;
}