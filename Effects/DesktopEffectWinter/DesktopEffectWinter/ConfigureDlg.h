#pragma once
#include "atlwin.h"

#include "resource.h"
class CDesktopEffectSnow;

class CConfigureDlg :
	public CDialogImpl<CConfigureDlg>
{
	CDesktopEffectSnow* effect;
	BOOL init;

	CTrackBarCtrl wind;
	CTrackBarCtrl speed;
	CTrackBarCtrl numFlakes;
	CTrackBarCtrl size;

public:
	CConfigureDlg(CDesktopEffectSnow* effect);
	~CConfigureDlg(void);

	enum { IDD = IDD_CONFIGDLG };

BEGIN_MSG_MAP(CConfigureDlg)
	// dialog stuff
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)


	COMMAND_HANDLER(IDC_RANDOMFLAKES, BN_CLICKED, OnBnClickedRandomflakes)
	COMMAND_HANDLER(IDC_GROUND, BN_CLICKED, OnBnClickedGround)

	// other stuff
    REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBnClickedRandomflakes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedGround(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
