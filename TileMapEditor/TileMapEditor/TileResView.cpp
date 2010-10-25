
#include "stdafx.h"
#include "MainFrm.h"
#include "TileResView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "ToolManager.h"
#include "ToolBrush.h"

using namespace Cactus;

#define M_ListCtrl_ID	(WM_USER + 100)


/*
BUTTON      ID_TOOLBAR_RESDETAIL_RESGROUP_C
BUTTON      ID_TOOLBAR_REDETAIL_RESGROUP
SEPARATOR
BUTTON      ID_TOOLBAR_RESDETAIL_TYPE_C
BUTTON      ID_TOOLBAR_RESDETAIL_TYPE
SEPARATOR
BUTTON      ID_TOOLBAR_RESDETAIL_EDIT
*/
//
//BEGIN_MESSAGE_MAP(ResDetailToolBar, CClassToolBar)
//	//ON_WM_CTLCOLOR()
//END_MESSAGE_MAP()

//HBRUSH ResDetailToolBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	if(nCtlColor == CTLCOLOR_STATIC) 
//	{ 
//		//pDC->SetTextColor(RGB(0,0,255)); 
//		pDC->SetBkMode(TRANSPARENT);
//
//		return (HBRUSH)_brush;
//	} 
//
//	return CClassToolBar::OnCtlColor(pDC, pWnd, nCtlColor);
//}
//
//
//void ResDetailToolBar::CreateControls()
//{
//	_brush.CreateStockObject(NULL_BRUSH);
//	_font.CreateStockObject(DEFAULT_GUI_FONT);
//
//	int iIndex = 0;
//	while ( GetItemID(iIndex) != ID_TOOLBAR_RESDETAIL_RESGROUP_C) iIndex++;
//
//	CRect rc(5, 2, 55, 20);
//	_editResGroupC.Create("资源组：", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rc, this, ID_TOOLBAR_RESDETAIL_RESGROUP_C);
//	_editResGroupC.SetFont(&_font);
//
//
//	while ( GetItemID(iIndex) != ID_TOOLBAR_REDETAIL_RESGROUP) iIndex++;
//	rc.left		= rc.right + 5;
//	rc.right	= rc.left + 60;
//	_editResGroup.Create("", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rc, this, ID_TOOLBAR_REDETAIL_RESGROUP);
//	_editResGroup.SetFont(&_font);
//
//	while ( GetItemID(iIndex) != ID_TOOLBAR_RESDETAIL_TYPE_C) iIndex++;
//	rc.left		= rc.right + 5;
//	rc.right	= rc.left + 60;
//	_editResTypeC.Create("资源类型：", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rc, this, ID_TOOLBAR_RESDETAIL_TYPE_C);
//	_editResTypeC.SetFont(&_font);
//
//	while ( GetItemID(iIndex) != ID_TOOLBAR_RESDETAIL_TYPE) iIndex++;
//	rc.left		= rc.right + 5;
//	rc.right	= rc.left + 50;
//	_editResType.Create("", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rc, this, ID_TOOLBAR_RESDETAIL_TYPE);
//	_editResType.SetFont(&_font);
//
//
//	while ( GetItemID(iIndex) != ID_TOOLBAR_RESDETAIL_EDIT) iIndex++;
//	rc.left		= rc.right + 5;
//	rc.right	= rc.left + 36;
//	_btnEdit.Create("编辑", WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | BS_PUSHBUTTON, rc, this, ID_TOOLBAR_RESDETAIL_EDIT);
//	_btnEdit.SetFont(&_font);
//}


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
	ON_NOTIFY(NM_DBLCLK, M_ListCtrl_ID, &TileResView::OnNMDblclkListItem)

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


	//创建工具条
	_dialogBar.Create(DialogResDetail::IDD, this);

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

	int cyTlb = 30;

	_dialogBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

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
		CString str = _listImages.GetItemText(_iOldCheck, 0);

		//Set brush tool
		ToolBrush* pBrush = (ToolBrush*)ToolManager::getSingleton().SelectTool(eToolBrush);
		pBrush->SetResource(_strResKey, (LPCTSTR)str);
	} 
	else if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */ 
		) 
	{ 
		//TRACE("Item %d is unchecked\n", pNMLV->iItem); 
		_iOldCheck = -1;
		ToolManager::getSingleton().SelectTool(eToolSelect);
	} 
	else 
	{ 
		//TRACE("Item %d does't change the check-status\n", pNMLV->iItem); 
	} 

	*pResult = 0; 

}

void TileResView::OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR); 

	//if (_iOldCheck != -1)
	//{
	//	_listImages.SetCheck(_iOldCheck, FALSE);
	//}

	//_iOldCheck = pNMLV->iItem;
	//_listImages.SetCheck(_iOldCheck, TRUE);

	//CString str = _listImages.GetItemText(_iOldCheck, 0);

	////Set brush tool
	//ToolBrush* pBrush = (ToolBrush*)ToolManager::getSingleton().SelectTool(eToolBrush);
	//pBrush->SetResource(_strResKey, (LPCTSTR)str);

	*pResult = 0;
}
