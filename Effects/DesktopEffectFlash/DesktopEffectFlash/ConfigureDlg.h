#pragma once
#include "atlwin.h"

#include "resource.h"

class CDesktopEffectFlashHost;

class CConfigureDlg :
	public CDialogImpl<CConfigureDlg>
{
CDesktopEffectFlashHost* host;

public:
	CConfigureDlg(CDesktopEffectFlashHost* host);
	~CConfigureDlg(void);

	enum { IDD = IDD_CONFIG };

BEGIN_MSG_MAP(CConfigureDlg)
	// dialog stuff
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)


	// other stuff
    //REFLECT_NOTIFICATIONS()
	COMMAND_HANDLER(IDC_SCALELIST, CBN_SELCHANGE, OnCbnSelchangeScalelist)
	COMMAND_HANDLER(IDC_LOADMOVIE, BN_CLICKED, OnBnClickedLoadmovie)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCbnSelchangeScalelist(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedLoadmovie(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
