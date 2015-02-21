#pragma once
#include "atlwin.h"

#include "resource.h"
class CDesktopEffectVideoWallpaper;

class CConfigureDlg :
	public CDialogImpl<CConfigureDlg>
{
	CDesktopEffectVideoWallpaper* effect;

public:
	CConfigureDlg(CDesktopEffectVideoWallpaper* effect);
	~CConfigureDlg(void);

	enum { IDD = IDD_CONFIGDLG };

BEGIN_MSG_MAP(CConfigureDlg)
	// dialog stuff
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

	COMMAND_HANDLER(IDBROWSE, BN_CLICKED, OnBnBrowseConfig)

	// other stuff
    //REFLECT_NOTIFICATIONS()
	COMMAND_HANDLER(IDC_OUTPUTPERMONITOR, BN_CLICKED, OnBnClickedOutputpermonitor)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBnBrowseConfig(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOutputpermonitor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
