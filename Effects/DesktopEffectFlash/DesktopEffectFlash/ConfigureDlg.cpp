#include "StdAfx.h"
#include "ConfigureDlg.h"

#include "DesktopEffectFlashHost.h"

CConfigureDlg::CConfigureDlg(CDesktopEffectFlashHost* host) : host(host)
{
}

CConfigureDlg::~CConfigureDlg(void)
{
}


LRESULT CConfigureDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//SetDlgItemText(IDC_VIDEO, effect->GetFileName());
	//CheckDlgButton(IDC_OUTPUTPERMONITOR, !effect->GetStretchOverWholeCanvas());

	CComboBox movieList = GetDlgItem(0);
	movieList.ModifyStyle(0, CBS_AUTOHSCROLL);
	movieList.LimitText(10000);

	SetDlgItemText(IDC_MOVIELIST, host->getMovieUrl());

	CString scale1;
	scale1.LoadString(IDS_SCALE1);

	CString scale2;
	scale2.LoadString(IDS_SCALE2);

	CString scale3;
	scale3.LoadString(IDS_SCALE3);

	CComboBox scaleBox = GetDlgItem(IDC_SCALELIST);
	scaleBox.AddString(scale1);
	scaleBox.AddString(scale2);
	scaleBox.AddString(scale3);

	scaleBox.SetCurSel(host->getScaling());

	SetDlgItemText(IDC_FLASHVARS, host->getFlashVars());

	return 0;
}

LRESULT CConfigureDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}


LRESULT CConfigureDlg::OnCbnSelchangeScalelist(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CComboBox scaleBox = GetDlgItem(IDC_SCALELIST);

	int curSel = scaleBox.GetCurSel();
	host->setScaling(curSel);

	return 0;
}

LRESULT CConfigureDlg::OnBnClickedLoadmovie(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	WCHAR buf[2048] = {0};
	GetDlgItemText(IDC_MOVIELIST, buf, 2048);
	host->setMovieUrl(buf);

	CComboBox box = GetDlgItem(IDC_MOVIELIST);
	if(box.FindString(0, buf) == -1)
		box.AddString(buf);

	WCHAR bufVars[10000] = {0};
	GetDlgItemText(IDC_FLASHVARS, bufVars, 10000);
	host->setFlashVars(bufVars);

	return 0;
}
