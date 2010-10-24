#include "stdafx.h"
#include "GameObjectEditor.h"

#include "Resource.h"

using namespace Cactus;


#define M_VIEW_ID	(WM_USER + 10)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

GameObjectEditor::GameObjectEditor()
{
}

GameObjectEditor::~GameObjectEditor()
{
}

BEGIN_MESSAGE_MAP(GameObjectEditor, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int GameObjectEditor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	_dlgPanel.Create(CDialogGameObject::IDD, this);

	_pView = new GameObjectEditorView;
	_pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rectDummy, this, M_VIEW_ID);
	_pView->OnInitialUpdate();
	_pView->ShowWindow(SW_SHOW);

	return 0;
}

void GameObjectEditor::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.bottom = 60;
	_dlgPanel.MoveWindow(&rectClient);

	rectClient.top = rectClient.bottom;
	_pView->SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL GameObjectEditor::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void GameObjectEditor::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

}

void GameObjectEditor::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void GameObjectEditor::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}
