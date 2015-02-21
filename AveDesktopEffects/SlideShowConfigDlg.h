#pragma once
#include "atlwin.h"
#include "resource.h"

class CAveDesktopEffect;

class CSlideShowConfigDlg :
	public CDialogImpl<CSlideShowConfigDlg>
{
protected:
	CAveDesktopEffect* effect;

	CComboBox intervalBox;
	CTrackBarCtrl transitionSlider;

	int intervalMap[11];

	BOOL init;

public:
	CSlideShowConfigDlg(CAveDesktopEffect* effect);
	~CSlideShowConfigDlg(void);

		enum { IDD = IDD_SLIDESHOWCONFIG };

BEGIN_MSG_MAP(CConfigureDlg)
	// dialog stuff
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_AVE_EFFECTCONFIG_OK, OnEffectConfigOk)

	COMMAND_ID_HANDLER(IDBROWSE, OnBrowseButtonClick)
	COMMAND_HANDLER(IDC_PHOTODURATIONPRESETS, CBN_SELCHANGE, OnCbnSelchangePhotodurationpresets)
	COMMAND_HANDLER(IDC_CUSTOMINTERVAL, EN_CHANGE, OnEnChangeCustominterval)

	// other stuff
    REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEffectConfigOk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnBrowseButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCbnSelchangePhotodurationpresets(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeCustominterval(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNMCustomdrawTransitionspeed(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
