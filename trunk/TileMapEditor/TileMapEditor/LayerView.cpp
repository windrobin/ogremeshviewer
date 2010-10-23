
#include "stdafx.h"
#include "MainFrm.h"
#include "LayerView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "ToolManager.h"
#include "ToolBrush.h"

using namespace Cactus;

#define M_ListCtrl_ID	(WM_USER + 100)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

LayerView::LayerView()
{
}

LayerView::~LayerView()
{
}

BEGIN_MESSAGE_MAP(LayerView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)

	//ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int LayerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();


	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_LIST;
	_listObjects.Create(dwViewStyle, CRect(0, 0, 100, 100), this, M_ListCtrl_ID);
	//_listObjects.SetBkColor(0);

	return 0;
}

void LayerView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	rectClient.top += cyTlb;
	_listObjects.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	_listObjects.Arrange(LVA_DEFAULT);
}

BOOL LayerView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void LayerView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

}

void LayerView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void LayerView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

