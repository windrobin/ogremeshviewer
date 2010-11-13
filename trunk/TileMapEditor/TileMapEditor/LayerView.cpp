
#include "stdafx.h"
#include "MainFrm.h"
#include "LayerView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "MapLayer.h"
#include "ToolManager.h"
#include "ToolSelect.h"

using namespace Cactus;

#define M_ListCtrl_ID	(WM_USER + 100)
#define M_TOOLBAR_ID	(WM_USER + 101)

//////////////////////////////////////////////////////////////////////
// ����/����
//////////////////////////////////////////////////////////////////////

LayerView::LayerView()
: _pLayer(0)
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
	ON_COMMAND(ID_TOOLBAR_MAPLAYER_SELECT_SIMILAR, OnInvertSelect)
	ON_COMMAND(ID_TOOLBAR_MAPLAYER_INVERT_SELECT, OnSelectSimilar)
	ON_COMMAND(ID_TOOLBAR_MAPLAYER_DELETE_SELECT, OnDeleteSelection)

	ON_NOTIFY(LVN_COLUMNCLICK, M_ListCtrl_ID, OnLvnColumnclickList)
	ON_NOTIFY(NM_CLICK, M_ListCtrl_ID, OnNMclickList)
	ON_NOTIFY(LVN_ITEMCHANGED, M_ListCtrl_ID, OnItemStateChanged)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView ��Ϣ�������

int LayerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();


	// ����ͼ��:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, M_TOOLBAR_ID);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MAPLAYER, 0, 0, TRUE /* ������*/);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SORTASCENDING;
	_listObjects.Create(dwViewStyle, CRect(0, 0, 100, 100), this, M_ListCtrl_ID);
	//_listObjects.SetBkColor(0);
	_listObjects.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	_listObjects.InsertColumn(0, "��Դ��"	, LVCFMT_LEFT, 50);
	_listObjects.InsertColumn(1, "��ԴID"	, LVCFMT_LEFT, 70);
	_listObjects.InsertColumn(2, "X", LVCFMT_LEFT, 30);
	_listObjects.InsertColumn(3, "Y", LVCFMT_LEFT, 30);
	_listObjects.InsertColumn(4, "����"	, LVCFMT_LEFT, 40);

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
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

}

void LayerView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void LayerView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("��ӳ�Ա����..."));
}

int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	STile* pData1 = (STile*)lParam1;
	STile* pData2 = (STile*)lParam2;

	int iColum = (int)lParamSort;
	if (iColum == 0)		//��Դ��
	{
		return pData1->_strResGroup > pData2->_strResGroup;
	}
	else if (iColum == 1)	//��ԴID
	{
		return pData1->_strResItemID > pData2->_strResItemID;
	}
	else if (iColum == 2)	//X
	{
		return pData1->_ptGrid.x - pData2->_ptGrid.x;
	}
	else if (iColum == 3)	//Y
	{
		return pData1->_ptGrid.y - pData2->_ptGrid.y;
	}
	else if (iColum == 4)	//����
	{
		return pData1->_regionID - pData2->_regionID;
	}

	return (int)(lParam1 - lParam2);
}


void LayerView::OnLvnColumnclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	_listObjects.SortItems(MyCompareProc, pNMLV->iSubItem);

	*pResult = 0;
}

void LayerView::OnNMclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = _listObjects.GetFirstSelectedItemPosition();
	if(pos != NULL)
	{
		int nItem = _listObjects.GetNextSelectedItem(pos);
	}

	*pResult = 0;
}

void LayerView::OnItemStateChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW lpView = (LPNMLISTVIEW) pNMHDR;

	if (lpView->iItem != -1 && lpView->uChanged != 0)
	{
		STile* pData = (STile*)_listObjects.GetItemData(lpView->iItem);

		if (lpView->uNewState & LVIS_SELECTED)
		{
			Log_Info("ѡ��Tile��λ��: (" << pData->_ptGrid.x << ", " << pData->_ptGrid.y << ") ��Դ�飺" << pData->_strResGroup << " : " << pData->_strResItemID);

			ToolSelect* pSelectTool = (ToolSelect*)ToolManager::getSingleton().GetTool(eToolSelect);
			pSelectTool->SelectTile(pData, true);
		}
		else if (lpView->uOldState & LVIS_SELECTED)
		{
			Log_Info("ѡ��Tile��λ��: (" << pData->_ptGrid.x << ", " << pData->_ptGrid.y << ") ��Դ�飺" << pData->_strResGroup << " : " << pData->_strResItemID);

			ToolSelect* pSelectTool = (ToolSelect*)ToolManager::getSingleton().GetTool(eToolSelect);
			pSelectTool->SelectTile(pData, false);
		}
	}

	*pResult = 0;
}

void LayerView::SetCurrentLayer(MapLayer* pLayer)
{
	if (_pLayer == pLayer)
		return;

	_pLayer = pLayer;
	_listObjects.DeleteAllItems();

	if (_pLayer)
	{
		_pLayer->FillMapLayerList();
	}
}

void LayerView::AddTileInfo(STile* pTile)
{
	int iIndex = _listObjects.InsertItem(_listObjects.GetItemCount(), _T(""));

	_listObjects.SetItemText(iIndex, 0, pTile->_strResGroup.c_str());
	_listObjects.SetItemText(iIndex, 1, pTile->_strResItemID.c_str());
	
	CString strTmp;
	strTmp.Format("%d", pTile->_ptGrid.x);
	_listObjects.SetItemText(iIndex, 2, strTmp);

	strTmp.Format("%d", pTile->_ptGrid.y);
	_listObjects.SetItemText(iIndex, 3, strTmp);

	strTmp.Format("%d", pTile->_regionID);
	_listObjects.SetItemText(iIndex, 4, strTmp);

	_listObjects.SetItemData(iIndex, DWORD_PTR(pTile));
}

bool LayerView::UpdateTileInfo(STile* pTile)
{
	for (int t = 0; t < _listObjects.GetItemCount(); ++t)
	{
		STile* pData = (STile*)_listObjects.GetItemData(t);
		if (pTile == pData)
		{
			CString strTmp;
			strTmp.Format("%d", pTile->_ptGrid.x);
			_listObjects.SetItemText(t, 2, strTmp);

			strTmp.Format("%d", pTile->_ptGrid.y);
			_listObjects.SetItemText(t, 3, strTmp);

			strTmp.Format("%d", pTile->_regionID);
			_listObjects.SetItemText(t, 4, strTmp);
			
			return true;
		}
	}

	return false;
}

bool LayerView::RemoveTileInfo(STile* pTile)
{
	for (int t = 0; t < _listObjects.GetItemCount(); ++t)
	{
		STile* pData = (STile*)_listObjects.GetItemData(t);
		if (pTile == pData)
		{
			_listObjects.DeleteItem(t);
			
			return true;
		}
	}

	return false;
}

void LayerView::OnInvertSelect()
{
}

void LayerView::OnSelectSimilar()
{
}

void LayerView::OnDeleteSelection()
{
	POSITION pos = _listObjects.GetFirstSelectedItemPosition();
	if(pos != NULL)
	{
		int nItem = _listObjects.GetNextSelectedItem(pos);

		STile* pData = (STile*)_listObjects.GetItemData(nItem);
		if (pData)
		{
			_pLayer->RemoveTile(pData);
			_listObjects.DeleteItem(nItem);

			Log_Info("����ɾ���ɹ���");
		}
	}
}
