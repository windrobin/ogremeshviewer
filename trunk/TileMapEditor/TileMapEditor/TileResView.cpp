
#include "stdafx.h"
#include "MainFrm.h"
#include "TileResView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "ToolManager.h"
#include "ToolBrush.h"

using namespace Cactus;

#define M_ListCtrl_ID	(WM_USER + 100)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

TileResView::TileResView()
: _iOldCheck(-1)
{
}

TileResView::~TileResView()
{
}

BEGIN_MESSAGE_MAP(TileResView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_NOTIFY(LVN_ITEMCHANGED, M_ListCtrl_ID, OnItemChanged)

	//ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int TileResView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_ICON;
	_listImages.Create(dwViewStyle, CRect(0, 0, 100, 100), this, M_ListCtrl_ID);
	_listImages.SetExtendedStyle( _listImages.GetExtendedStyle() | LVS_EX_CHECKBOXES);
	//_listImages.SetBkColor(0);

	return 0;
}

void TileResView::OnSize(UINT nType, int cx, int cy)
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
	_listImages.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	_listImages.Arrange(LVA_DEFAULT);
}

BOOL TileResView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void TileResView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

}

void TileResView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void TileResView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

void TileResView::BuildImageAndInfoes(const Cactus::String& strResKey, CImageList* pImage, const Cactus::StringVector& captions)
{
	_listImages.DeleteAllItems();

	_iOldCheck	= -1;
	_strResKey	= strResKey;

	//Clear brush tool
	ToolManager::getSingleton().SelectTool(eToolSelect);

	_listImages.SetImageList(pImage, LVSIL_NORMAL);

	for (size_t t = 0; t < captions.size(); ++t)
	{
		_listImages.InsertItem(t, captions[t].c_str(), t);
	}
}

void TileResView::OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR); 

	if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */ 
		) 
	{ 
		//TRACE("Item %d is checked\n", pNMLV->iItem);

		if (_iOldCheck != -1)
		{
			_listImages.SetCheck(_iOldCheck, FALSE);
		}

		_iOldCheck = pNMLV->iItem;

		//Set brush tool
		ToolBrush* pBrush = (ToolBrush*)ToolManager::getSingleton().SelectTool(eToolBrush);
		Cactus::ostringstream os;
		os << _iOldCheck;
		pBrush->SetResource(_strResKey, os.str());
	} 
	else if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */ 
		) 
	{ 
		//TRACE("Item %d is unchecked\n", pNMLV->iItem); 
	} 
	else 
	{ 
		//TRACE("Item %d does't change the check-status\n", pNMLV->iItem); 
	} 

	*pResult = 0; 

}

