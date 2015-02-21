#include "StdAfx.h"
#include "SlideShowConfigDlg.h"
#include "AveDesktopEffect.h"

CSlideShowConfigDlg::CSlideShowConfigDlg(CAveDesktopEffect* effect) : effect(effect)
{
	intervalMap[0] = 5;
	intervalMap[1] = 10;
	intervalMap[2] = 30;
	intervalMap[3] = 60;
	intervalMap[4] = 60 * 2;
	intervalMap[5] = 60 * 5;
	intervalMap[6] = 60 * 10;
	intervalMap[7] = 60 * 15;
	intervalMap[8] = 60 * 30;
	intervalMap[9] = 60 * 60;
	intervalMap[10] = -1;

}

CSlideShowConfigDlg::~CSlideShowConfigDlg(void)
{
}

LRESULT CSlideShowConfigDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	init = TRUE;

	SetDlgItemText(IDC_WALLDIR, effect->wallBaseDir);


	intervalBox		 = GetDlgItem(IDC_PHOTODURATIONPRESETS);
	transitionSlider = GetDlgItem(IDC_TRANSITIONSPEED);

	transitionSlider.SetRange(0, 150000);

	transitionSlider.SetPos(effect->transitionDurationMs);

	BOOL isCustomInterval=TRUE;
	for(int i=0; i < sizeof(intervalMap)/sizeof(intervalMap[0]); ++i)
	{
		CString text;
		text.LoadStringW(IDS_PRESET0 + i);
		intervalBox.AddString(text);
		if(intervalMap[i] == effect->photoInterval)
		{
			intervalBox.SetCurSel(i);
			isCustomInterval = FALSE;
		}
	}

	if(isCustomInterval)
	{
		// last item in the map is the special one
		intervalBox.SetCurSel(sizeof(intervalMap)/sizeof(intervalMap[0])-1);
		GetDlgItem(IDC_CUSTOMINTERVAL).ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CUSTOMINTERVALLABEL).ShowWindow(SW_SHOW);
		SetDlgItemInt(IDC_CUSTOMINTERVAL, effect->photoInterval);
	}

	init = FALSE;

	return 0;
}

LRESULT CSlideShowConfigDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_hWnd = NULL;
	return 0;
}

LRESULT CSlideShowConfigDlg::OnEffectConfigOk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CSlideShowConfigDlg::OnBrowseButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFolderDialog dlg;
	dlg.SetInitialFolder(effect->wallBaseDir);
	if(dlg.DoModal(GetParent()) && wcslen(dlg.m_szFolderPath) > 0)
	{
		SetDlgItemText(IDC_WALLDIR, dlg.m_szFolderPath);
		effect->SetBaseDir(dlg.m_szFolderPath);
	}
	return 0;
}

LRESULT CSlideShowConfigDlg::OnCbnSelchangePhotodurationpresets(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int sel = intervalBox.GetCurSel();
	if(sel >= 0 && sel < sizeof(intervalMap)/sizeof(intervalMap[0]))
	{
		int newInterval = intervalMap[sel];
		if(-1 == newInterval)
		{
			GetDlgItem(IDC_CUSTOMINTERVAL).ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CUSTOMINTERVALLABEL).ShowWindow(SW_SHOW);
		}
		else
		{
			intervalBox.SetFocus();
			GetDlgItem(IDC_CUSTOMINTERVAL).ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CUSTOMINTERVALLABEL).ShowWindow(SW_HIDE);
			effect->SetPhotoInterval(intervalMap[sel]);
			GetDlgItem(IDC_CUSTOMINTERVAL).SetFocus();
		}
	}

	return 0;
}

LRESULT CSlideShowConfigDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if((HWND)lParam == transitionSlider.m_hWnd && !init)
	{
		int newVal = transitionSlider.GetPos();
		effect->SetFadeInterval(newVal);
		KillTimer(1);
		SetTimer(1, 500);
	}

	return 0;
}

LRESULT CSlideShowConfigDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	if(1 == wParam)
	{
		KillTimer(1);
		if(effect->directDevice != NULL && !effect->doStop)
			effect->ToNextPhoto();
	}


	return 0;
}

LRESULT CSlideShowConfigDlg::OnEnChangeCustominterval(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int value = GetDlgItemInt(IDC_CUSTOMINTERVAL);
	if(value > 0)
	{
		effect->SetPhotoInterval(value);	
	}

	return 0;
}