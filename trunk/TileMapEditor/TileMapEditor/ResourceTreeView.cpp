
#include "stdafx.h"
#include "mainfrm.h"
#include "ResourceTreeView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "ResourceManager.h"

#include "TileResView.h"

#define M_TreeID	(WM_USER + 100)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// ArtSourceTreeView

ResourceTreeView::ResourceTreeView()
{
}

ResourceTreeView::~ResourceTreeView()
{
}

BEGIN_MESSAGE_MAP(ResourceTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(TVN_SELCHANGED, M_TreeID, &ResourceTreeView::OnTvnSelchangedTreeDetails)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序


int ResourceTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_DISABLEDRAGDROP | TVS_TRACKSELECT;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, M_TreeID))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	FillFileView();
	AdjustLayout();

	return 0;
}

void ResourceTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void ResourceTreeView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("Root"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hArtSrc = m_wndFileView.InsertItem(_T("美术"), 0, 0, hRoot);

	_treeArtBackgroundRes	= m_wndFileView.InsertItem(_T("背景资源"), 0, 0, hArtSrc);
	for (ResourceManager::ResBackgroundType::iterator it = ResourceManager::getSingleton()._ResBackgrounds.begin(); 
		it != ResourceManager::getSingleton()._ResBackgrounds.end(); ++it)
	{
		HTREEITEM hItem = m_wndFileView.InsertItem(it->first.c_str(), 2, 2, _treeArtBackgroundRes);
		m_wndFileView.SetItemData(hItem, (DWORD_PTR)it->second);
	}

	_treeArtTileRes			= m_wndFileView.InsertItem(_T("Tile资源"), 0, 0, hArtSrc);
	for (ResourceManager::ResTileType::iterator it = ResourceManager::getSingleton()._ResTiles.begin(); 
		it != ResourceManager::getSingleton()._ResTiles.end(); ++it)
	{
		HTREEITEM hItem = m_wndFileView.InsertItem(it->first.c_str(), 2, 2, _treeArtTileRes);
		m_wndFileView.SetItemData(hItem, (DWORD_PTR)it->second);
	}

	_treeGameObjectRes	= m_wndFileView.InsertItem(_T("游戏对象"), 0, 0, hRoot);

	HTREEITEM hItem = m_wndFileView.InsertItem(_T("NPC"), 2, 2, _treeGameObjectRes);
	m_wndFileView.SetItemData(hItem, (DWORD_PTR)ResourceManager::getSingleton()._ResGameObjectGroups["npc"]);
	
	hItem = m_wndFileView.InsertItem(_T("Monster"), 2, 2, _treeGameObjectRes);
	m_wndFileView.SetItemData(hItem, (DWORD_PTR)ResourceManager::getSingleton()._ResGameObjectGroups["monster"]);
	
	hItem = m_wndFileView.InsertItem(_T("Function Point"), 2, 2, _treeGameObjectRes);
	m_wndFileView.SetItemData(hItem, (DWORD_PTR)ResourceManager::getSingleton()._ResGameObjectGroups["funcpoint"]);

	_treeGameEventRes	= m_wndFileView.InsertItem(_T("游戏事件"), 0, 0, hRoot);

	//m_wndFileView.InsertItem(_T("通用"), 2, 2, hRes);
	//m_wndFileView.InsertItem(_T("单人"), 2, 2, hRes);
	//m_wndFileView.InsertItem(_T("多人"), 2, 2, hRes);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hArtSrc, TVE_EXPAND);
	m_wndFileView.Expand(_treeGameObjectRes, TVE_EXPAND);
	m_wndFileView.Expand(_treeGameEventRes, TVE_EXPAND);
}

void ResourceTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
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
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void ResourceTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void ResourceTreeView::OnProperties()
{
	AfxMessageBox(_T("属性...."));

}

void ResourceTreeView::OnFileOpen()
{
	// TODO: 在此处添加命令处理程序代码
}

void ResourceTreeView::OnFileOpenWith()
{
	// TODO: 在此处添加命令处理程序代码
}

void ResourceTreeView::OnDummyCompile()
{
	// TODO: 在此处添加命令处理程序代码
}

void ResourceTreeView::OnEditCut()
{
	// TODO: 在此处添加命令处理程序代码
}

void ResourceTreeView::OnEditCopy()
{
	// TODO: 在此处添加命令处理程序代码
}

void ResourceTreeView::OnEditClear()
{
	// TODO: 在此处添加命令处理程序代码
}

void ResourceTreeView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void ResourceTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void ResourceTreeView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定 */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void ResourceTreeView::OnTvnSelchangedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CString strText = m_wndFileView.GetItemText(pNMTreeView->itemNew.hItem);

	DWORD_PTR ptr = m_wndFileView.GetItemData(pNMTreeView->itemNew.hItem);
	if (ptr)
	{
		Resource* pRes = (Resource*)ptr;
		pRes->CreateImageList();

		//insert image into TileResView

		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );

		pMainFrame->GetTileResView()->BuildImageAndInfoes(pRes->GetImageList(), *pRes->GetCaptions());

	}

	*pResult = 0;
}
