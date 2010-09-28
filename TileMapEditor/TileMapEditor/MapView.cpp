
#include "stdafx.h"
#include "MainFrm.h"
#include "MapView.h"
#include "Resource.h"
#include "TileMapEditor.h"
#include "MapLayer.h"

#define M_TreeID	(WM_USER + 100)

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class MapView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// ����/����
//////////////////////////////////////////////////////////////////////

MapView::MapView()
{
}

MapView::~MapView()
{
}

BEGIN_MESSAGE_MAP(MapView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_CLICK, M_TreeID, &MapView::OnNMClickedTreeDetails)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView ��Ϣ�������

int MapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_CHECKBOXES | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!_TreeMapItem.Create(dwViewStyle, rectDummy, this, M_TreeID))
	{
		TRACE0("δ�ܴ�������ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ����ͼ��:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);


	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	FillClassView();

	return 0;
}

void MapView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void MapView::AddMapLayer(MapLayer* pLayer)
{
	HTREEITEM hItem = _TreeMapItem.InsertItem(pLayer->GetLayerName().c_str(), 3, 3, _hLayerRoot);
	_TreeMapItem.SetItemData(hItem, (DWORD_PTR)pLayer);
	_TreeMapItem.SetCheck(hItem, pLayer->IsVisible());

	_TreeMapItem.Expand(_hLayerRoot, TVE_EXPAND);
}

void MapView::FillClassView()
{
	HTREEITEM hRoot = _TreeMapItem.InsertItem(_T("��ͼ����"), 0, 0);
	_TreeMapItem.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = _TreeMapItem.InsertItem(_T("����"), 3, 3, hRoot);
	_TreeMapItem.Expand(hRoot, TVE_EXPAND);

	_hLayerRoot = _TreeMapItem.InsertItem(_T("��"), 1, 1, hRoot);
	//_TreeMapItem.InsertItem(_T("Layer0"), 3, 3, _hLayerRoot);
	//_TreeMapItem.InsertItem(_T("Layer1"), 3, 3, _hLayerRoot);
	//_TreeMapItem.InsertItem(_T("Layer2"), 3, 3, _hLayerRoot);
	
	_hBrushRoot = _TreeMapItem.InsertItem(_T("��ˢ"), 1, 1, hRoot);
	//_TreeMapItem.InsertItem(_T("Brush0"), 3, 3, _hBrushRoot);
	//_TreeMapItem.InsertItem(_T("Brush1"), 3, 3, _hBrushRoot);
	_TreeMapItem.Expand(_hBrushRoot, TVE_EXPAND);

	//����Icon : 2, 5, 6

}

void MapView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&_TreeMapItem;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
}

void MapView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	_TreeMapItem.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL MapView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void MapView::OnProperties()
{
}

void MapView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	_TreeMapItem.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void MapView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	_TreeMapItem.SetFocus();
}

void MapView::OnChangeVisualStyle()
{
	//������
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ���� */);


	//Tree��ͼ��
	_TreeImageList.DeleteImageList();
	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	_TreeImageList.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	_TreeImageList.Add(&bmp, RGB(255, 0, 0));

	_TreeMapItem.SetImageList(&_TreeImageList, TVSIL_NORMAL);

}

void MapView::OnNMClickedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//CString strText = _TreeMapItem.GetItemText(pNMTreeView->itemNew.hItem);
	//DWORD_PTR ptr = _TreeMapItem.GetItemData(pNMTreeView->itemNew.hItem);
	//if (ptr)
	//{
	//}

	CPoint point;
	GetCursorPos(&point);
	_TreeMapItem.ScreenToClient(&point);

	UINT uFlag;
	HTREEITEM hTree = _TreeMapItem.HitTest(point, &uFlag);
	if (hTree && (TVHT_ONITEMSTATEICON & uFlag))
	{
		DWORD_PTR ptr = _TreeMapItem.GetItemData(hTree);
		if(ptr)
		{
		}
	}

	*pResult = 0;
}

