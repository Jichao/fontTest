#include "stdafx.h"
#include "config.h"
#include "resource.h"
#include "command.h"

bool CheckMenu(HMENU hmenu, UINT nItem, bool byPosition, bool check);

void OnCommand(HWND hwnd, int cmdId) 
{
	if (cmdId == ID_LINESTYLE_LINE) {
		g_lineStyle = kLineStyle_Line;
	} else if (cmdId == ID_LINESTYLE_QUADARTICBEZIER) {
		g_lineStyle = kLineStyle_2Bezier;
	} else if (cmdId == ID_LINESTYLE_NNNNBEZIER) {
		g_lineStyle = kLineStyle_nBezier;
	}

	if (cmdId == ID_FILLSTYLE_SUCKFILL) {
		g_fillStyle = kFillStyle_SuckFill;
	} else if (cmdId == ID_FILLSTYLE_FILL) {
		g_fillStyle = kFillStyle_Fill;
	} else if (cmdId == ID_FILLSTYLE_CONTOUR) {
		g_fillStyle = kFillStyle_OnlyContour;
	}

	UpdateMenuState(hwnd);
	InvalidateRect(hwnd, NULL, FALSE);
}

void UpdateMenuState(HWND hwnd) 
{
	HMENU hMenu = GetSubMenu(GetMenu(hwnd), 2);
	CheckMenu(hMenu, ID_LINESTYLE_LINE, false, g_lineStyle == kLineStyle_Line);
	CheckMenu(hMenu, ID_LINESTYLE_QUADARTICBEZIER, false, g_lineStyle == kLineStyle_2Bezier);
	CheckMenu(hMenu, ID_LINESTYLE_NNNNBEZIER, false, g_lineStyle == kLineStyle_nBezier);
	hMenu = GetSubMenu(GetMenu(hwnd), 1);
	CheckMenu(hMenu, ID_FILLSTYLE_SUCKFILL, false, g_fillStyle == kFillStyle_SuckFill);
	CheckMenu(hMenu, ID_FILLSTYLE_FILL, false, g_fillStyle == kFillStyle_Fill);
	CheckMenu(hMenu, ID_FILLSTYLE_CONTOUR, false, g_fillStyle == kFillStyle_OnlyContour);
}

bool CheckMenu(HMENU hmenu, UINT nItem, bool byPosition, bool check)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	if (!hmenu)
		return false;
	if (!GetMenuItemInfo(hmenu, nItem, byPosition, &mii))
		return false;
	if (check) {
		mii.fState |= MFS_CHECKED;
	} else {
		mii.fState &= ~MFS_CHECKED;
	}
	return !!SetMenuItemInfo(hmenu, nItem, byPosition, &mii);
}
