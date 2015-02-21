// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"


#include "F:\\Coding\\DesktopProjects\\DesktopEffects\\DesktopFxSDK\\AveRegFuncs.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	desktop = NULL;
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	CheckDlgButton(IDC_XPMODE,		AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"Software\\AveSoftware\\DesktopEffects", L"ForceXPMode", 0));
	CheckDlgButton(IDC_NOEXPLORER,	AveRegFuncs::ReadInt(HKEY_CURRENT_USER, L"Software\\AveSoftware\\DesktopEffects", L"DoNotLoadInExplorer", 0));

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(desktop != NULL)
	{
		desktop->Release();
		desktop = NULL;
	}

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CloseDialog(0);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
LRESULT CMainDlg::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(NULL == desktop)
	{
		CComObject<CAveFakeDesktop>::CreateInstance(&desktop);
		if(NULL == desktop)
			return 0;

		desktop->AddRef();
	}


	if(desktop->IsWindow())
	{
		if(desktop->IsIconic())
			desktop->ShowWindow(SW_RESTORE);

		desktop->SetFocus();
	}
	else
	{
		desktop->loadHost = !(GetKeyState(VK_SHIFT) < 0);
		desktop->Create(NULL);
		desktop->ShowWindow(SW_MAXIMIZE);
	}


	return 0;
}

LRESULT CMainDlg::OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"Software\\AveSoftware\\DesktopEffects", L"CurrentEffectCLSID", L"");
	AveRegFuncs::WriteString(HKEY_CURRENT_USER, L"Software\\AveSoftware\\DesktopEffects", L"IsRunning", L"no");

	return 0;
}

LRESULT CMainDlg::OnBnClickedXpmode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"Software\\AveSoftware\\DesktopEffects", L"ForceXPMode", IsDlgButtonChecked(IDC_XPMODE));

	return 0;
}

LRESULT CMainDlg::OnBnClickedNoexplorer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	AveRegFuncs::WriteInt(HKEY_CURRENT_USER, L"Software\\AveSoftware\\DesktopEffects", L"DoNotLoadInExplorer", IsDlgButtonChecked(IDC_NOEXPLORER));

	return 0;
}
