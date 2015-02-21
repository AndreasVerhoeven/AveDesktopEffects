#pragma once
#include "atlwin.h"
#include "resource.h"

#include "EffectsListBox.h"

class CAveDesktopEffectsHost;
interface IAveDesktopEffect;

class CConfigureDlg :
	public CDialogImpl<CConfigureDlg>
{
protected:
	CAveDesktopEffectsHost* host;
	CEffectsListBox effectsList;

	SIZE origSize;

	HWND hwndConfigEffect;
	int prevSel;

public:
	CConfigureDlg(CAveDesktopEffectsHost* host);
	~CConfigureDlg(void);

	void GoBackToNormalConfig();
	HRESULT ShowConfigForEffect(IAveDesktopEffect* effect);

	enum { IDD = IDD_CONFIGUREDLG };

BEGIN_MSG_MAP(CConfigureDlg)
	// dialog stuff
	MESSAGE_HANDLER(WM_CLOSE, OnCloseDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_AVE_STATUSCHANGED, OnAveStatusChanged)

	COMMAND_ID_HANDLER(IDOK, OnCloseButtonClick)
	COMMAND_ID_HANDLER(IDTOGGLE, OnToggleButtonClick)

	COMMAND_HANDLER(IDC_EFFECTS, LBN_SELCHANGE, OnLbnSelchangeEffects)
	COMMAND_HANDLER(IDC_EFFECTS, LBN_DBLCLK, OnLbnDblclkEffects)
	COMMAND_HANDLER(IDCONFIG, BN_CLICKED, OnBnClickedConfig)

	// other stuff
    REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCloseDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAveStatusChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnCloseButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnToggleButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnLbnSelchangeEffects(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLbnDblclkEffects(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedConfig(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

