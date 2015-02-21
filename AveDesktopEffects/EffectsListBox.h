#pragma once

struct CEffectLBItem
{
	CString clsid, name, author, description;
	HBITMAP icon;
};

template< class T, class TBase = CListBox, class TWinTraits = CControlWinTraits > 
class ATL_NO_VTABLE CEffectsListBoxImpl : 
	public CWindowImpl< T, TBase, TWinTraits >,
	public COwnerDraw< CEffectsListBoxImpl<T> >
{

public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

	CBitmap dummyBmp;
	std::vector<CEffectLBItem> items;

	CFont bigFont;
	CFont smallFont;
	CIcon noIcon;

	SIZE iconSize, defIconSize;

	CEffectsListBoxImpl()
	{
		defIconSize.cx = 64;
		defIconSize.cy = 64;
		iconSize.cx = defIconSize.cx;
		iconSize.cy = defIconSize.cy;

		//SetItemHeight(0, iconSize.cy + 4);

		//noIcon.LoadIcon(IDI_ICON3);

		LOGFONTW lfBig = {0};
		lfBig.lfCharSet = DEFAULT_CHARSET;
		wcscpy_s(lfBig.lfFaceName, LF_FACESIZE, L"MS Shell Dlg");
		lfBig.lfWeight = FW_BOLD;
		lfBig.lfHeight = 14;

		LOGFONTW lfSmall = {0};
		lfSmall.lfCharSet = DEFAULT_CHARSET;
		wcscpy_s(lfSmall.lfFaceName, LF_FACESIZE, L"MS Shell Dlg");
		lfSmall.lfWeight = FW_NORMAL;
		lfSmall.lfHeight = 12;

		bigFont.CreateFontIndirect(&lfBig);
		smallFont.CreateFontIndirect(&lfSmall);
	}
	~CEffectsListBoxImpl(){}

	 BEGIN_MSG_MAP(CEffectsListBoxImpl)
      //MESSAGE_HANDLER(WM_CREATE, OnCreate)
      //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CEffectsListBoxImpl>, 1)
      DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()


	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
		//if( bRet ) _Init();
		return bRet;
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {

		CDCHandle dc((HDC)wParam);
		RECT rc;
		GetClientRect(&rc);

		HRGN origClipRegion = NULL;
		{
			GetClipRgn(dc, origClipRegion);
			int numItems = GetCount();
			for(int i = 0; i < numItems; ++i)
			{
				RECT itemRect = {0};
				GetItemRect(i, &itemRect);
				ExcludeClipRect(dc, itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);
			}

		}

		//CBrush backBrush;
		//backBrush.CreateSysColorBrush(COLOR_WINDOW);
		dc.FillSolidRect(&rc, GetSysColor(COLOR_WINDOW));

		SelectClipRgn(dc, origClipRegion);
		DeleteObject(origClipRegion);

		return 1;   // no background needed
   }



	void DrawItem(LPDRAWITEMSTRUCT lpdis)
	{
		//ATLASSERT(val == 0);
		
		if( lpdis->itemID==-1 ) return; // If there are no list box items, skip this message. 
		

		CDCHandle dc(lpdis->hDC);
		RECT rcItem = lpdis->rcItem;
		bool bSelected = lpdis->itemState & ODS_SELECTED;
		int rcW = rcItem.right - rcItem.left;
		int rcH = rcItem.bottom - rcItem.top;
		int rcX = 0;//rcItem.left;
		int rcY = 0;//rcItem.top;
		UINT index = lpdis->itemID;

		 //winDC(NULL);
		//CDCHandle bufDC = dc;
		CDC bufDC;
		bufDC.CreateCompatibleDC(CWindowDC(NULL));
		CBitmap bufBmp;
		bufBmp.CreateCompatibleBitmap(CWindowDC(NULL), rcW, rcH);
		if(::GetObjectType(bufBmp) == OBJ_BITMAP)
			bufDC.SelectBitmap(bufBmp);


		CBrush backBrush;
		COLORREF frontColor;
		bool hasFocus = m_hWnd == ::GetFocus();
		int highLightIndex = hasFocus ? COLOR_HIGHLIGHT : COLOR_BTNSHADOW;
		frontColor = GetSysColor((bSelected) ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT);
		backBrush.CreateSysColorBrush(bSelected ? highLightIndex : COLOR_WINDOW);

		int realIndex = (int)GetItemData(index);
		if(realIndex < 0 || realIndex >= (int)items.size())
			return;

		CEffectLBItem& item = items[realIndex];
	
		RECT fillRect={rcX,rcY,rcW + rcX,rcH + rcY};
		bufDC.FillRect(&fillRect, backBrush);

		rcY += 12;
		RECT textRect={iconSize.cx + 4 + rcX,2 + rcY, rcW + rcX, rcH + rcY};
		bufDC.SetBkMode(TRANSPARENT);
		bufDC.SetTextColor(frontColor);
		CFontHandle oldFont;
		if(iconSize.cy > 16)
			oldFont = bufDC.SelectFont(bigFont);
		else
			oldFont = bufDC.SelectFont(smallFont);

		RECT outputRect = textRect;
		bufDC.DrawText(item.name, -1, &textRect, DT_LEFT |   DT_SINGLELINE	| DT_END_ELLIPSIS);

		RECT detailsTextRect={iconSize.cx + 4 + rcX, 16 + rcY, rcW + rcX, rcH + rcY};
		bufDC.SelectFont(smallFont);
		bufDC.DrawText(item.description, -1, &detailsTextRect, DT_LEFT |  DT_EDITCONTROL | DT_WORDBREAK);

		rcY -= 12;

		bufDC.SelectFont(oldFont);

		if(iconSize.cy > 16 && rcItem.left+2 < iconSize.cx)
		{		
			if(item.icon != NULL)
			{
				CBitmapHandle hbmp = item.icon;
				CDC tempDC, tempDC2;
				CBitmap tempBmp;
				tempDC.CreateCompatibleDC(CWindowDC(NULL));
				tempDC2.CreateCompatibleDC(CWindowDC(NULL));
				tempBmp.CreateCompatibleBitmap(CWindowDC(NULL), defIconSize.cx, defIconSize.cy);
				if(::GetObjectType(tempBmp) == OBJ_BITMAP)
					tempDC.SelectBitmap(tempBmp);

				RECT fillRect = {0,0,defIconSize.cx, defIconSize.cy};
				tempDC.FillRect(&fillRect, backBrush);

				if(::GetObjectType(hbmp) == OBJ_BITMAP)
					tempDC2.SelectBitmap(hbmp);
				BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
				tempDC.AlphaBlend(0,0, defIconSize.cx, defIconSize.cx, tempDC2, 0, 0, defIconSize.cx, defIconSize.cx, bf);
				bufDC.SetStretchBltMode(HALFTONE);
				bufDC.StretchBlt(2+rcX,2+rcY, iconSize.cx, iconSize.cy, tempDC, 0, 0, defIconSize.cx, defIconSize.cy, SRCCOPY);
			}
			else
			{
				if(noIcon.m_hIcon != NULL)
					noIcon.DrawIconEx(bufDC, 2+rcX, 2+rcY, iconSize.cx, iconSize.cy);
			}
		}
		
		dc.BitBlt(rcItem.left, rcItem.top, rcW, rcH, bufDC, 0, 0, SRCCOPY);
	}

	int AddItem(CEffectLBItem& item)
	{
		int newIndex = (int)items.size();
		items.push_back(item);

		int index = AddString(item.name);
		SetItemData(index, (DWORD)newIndex);
		SetItemHeight(index, iconSize.cy + 4);
		return index;
	}

	CEffectLBItem& GetItem(int index)
	{
		int realIndex = (int)GetItemData(index);
		return items[realIndex];
	}

	int AddString(LPCTSTR str)
	{
		return CListBox::AddString(str);
	}

};

class CEffectsListBox : public CEffectsListBoxImpl<CEffectsListBox>
{
public:
   DECLARE_WND_SUPERCLASS(_T("AveDesktopEffectsListBox"), GetWndClassName())  
};
