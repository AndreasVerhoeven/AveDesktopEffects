#include "StdAfx.h"
#include "ConfigureDlg.h"

#include "DesktopEffectSnow.h"

CConfigureDlg::CConfigureDlg(CDesktopEffectSnow* effect) : effect(effect)
{
}

CConfigureDlg::~CConfigureDlg(void)
{
}

LRESULT CConfigureDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	init = TRUE;

	wind		= GetDlgItem(IDC_WIND);
	numFlakes	= GetDlgItem(IDC_NUMFLAKES);
	speed		= GetDlgItem(IDC_SPEED);
	size		= GetDlgItem(IDC_FLAKESIZE);

	CheckDlgButton(IDC_GROUND, effect->doRenderGround);
	CheckDlgButton(IDC_RANDOMFLAKES, effect->doRandomFlakes);
	numFlakes.EnableWindow(!effect->doRandomFlakes);

	wind.SetRange(-30, 30);
	numFlakes.SetRange(0, 30);
	speed.SetRange(10, 500);
	size.SetRange(3, 64);

	wind.SetPos(effect->wind);
	speed.SetPos(effect->speed);
	numFlakes.SetPos(30 - effect->maxCounter);
	size.SetPos(effect->size);

	init = FALSE;

	return 0;
}

LRESULT CConfigureDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CConfigureDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(!init)
	{
		HWND scroll = (HWND)lParam;
		if(scroll == wind)
		{
			effect->SetWind(wind.GetPos());
		}
		else if(scroll == numFlakes)
		{
			effect->SetMaxCounter(30 - numFlakes.GetPos());
		}
		else if(scroll == speed)
		{
			effect->SetSpeed(speed.GetPos());
		}
		else if(scroll == size)
		{
			effect->SetSize(size.GetPos());
		}

	}

	return 0;
}
LRESULT CConfigureDlg::OnBnClickedRandomflakes(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	effect->SetDoRandomFlakes(IsDlgButtonChecked(IDC_RANDOMFLAKES));
	numFlakes.EnableWindow(!IsDlgButtonChecked(IDC_RANDOMFLAKES));

	return 0;
}

LRESULT CConfigureDlg::OnBnClickedGround(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	effect->SetDoRenderGround(IsDlgButtonChecked(IDC_GROUND));

	return 0;
}
