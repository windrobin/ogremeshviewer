#include "stdafx.h"
#include "MainFrm.h"
#include "MapThumbnailView.h"
#include "Resource.h"
#include "TileMapEditor.h"

using namespace Cactus;


//////////////////////////////////////////////////////////////////////
// ����/����
//////////////////////////////////////////////////////////////////////

MapThumbnailView::MapThumbnailView()
{
}

MapThumbnailView::~MapThumbnailView()
{
}

BEGIN_MESSAGE_MAP(MapThumbnailView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapThumbnailView ��Ϣ�������

int MapThumbnailView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void MapThumbnailView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
}

BOOL MapThumbnailView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void MapThumbnailView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rcClient;
	GetClientRect(rcClient);

	dc.FillSolidRect(rcClient, RGB(0, 0, 0));
}

void MapThumbnailView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void MapThumbnailView::OnContextMenu(CWnd* pWnd, CPoint point)
{
}
