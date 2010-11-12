#include "StdAfx.h"
#include "ToolBrush.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

#include "ResourceManager.h"

ToolBrush::ToolBrush()
{
	_refCursor		= RGB(192, 192, 0);
	_bDrawCursor	= true;
	_rcOldSelected.SetRectEmpty();
}

ToolBrush::~ToolBrush()
{
}

void ToolBrush::Draw(CDC* pDC)
{
	if (!_bInRegion)
		return;

	Resource* pRes = ResourceManager::getSingleton().GetResourceArtGroup(_strResGroup);
	if (!pRes)
		pRes = ResourceManager::getSingleton().GetResourceGameObjectGroup(_strResGroup);

	if (pRes)
	{
		CRect rc = ToolManager::getSingleton().GetMap()->GetPixelCoordRect(_ptGrid);
		pRes->Draw(pDC, ToolManager::getSingleton().GetMap(), _ptGrid, _strResID);
	}

	//pDC->FillSolidRect(_rcOldSelected, RGB(128, 128, 128));

	ToolBase::Draw(pDC);
}

void ToolBrush::OnLButtonDown(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonDown(nFlags, point);

	if (!_bInRegion)
		return;

	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	if( pLayer->AddOrUpdateTile(_ptGrid, _strResGroup, _strResID) )
	{
		Log_Info("��ӻ����޸�Tile�ɹ���λ��: (" << _ptGrid.x << ", " << _ptGrid.y << ") ��Դ�飺" << _strResGroup << " : " << _strResID);
	}
}

void ToolBrush::OnLButtonUp(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonUp(nFlags, point);

}

void ToolBrush::OnMouseMove(UINT nFlags, CPoint point)
{
	ToolBase::OnMouseMove(nFlags, point);

	if (!_bInRegion)
	{
		//�����һ�λ��Ƶ��϶�����
		if (!_rcOldSelected.IsRectEmpty())
		{
			CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
			pView->LogicInvalidate(_rcOldSelected);

			_rcOldSelected.SetRectEmpty();
		}

		return;
	}

	if ( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		//�༭
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		if( pLayer->AddOrUpdateTile(_ptGrid, _strResGroup, _strResID) )
		{
			Log_Info("��ӻ����޸�Tile�ɹ���λ��: (" << _ptGrid.x << ", " << _ptGrid.y << ") ��Դ�飺" << _strResGroup << " : " << _strResID);
		}
	}

	CRect rcDirty = _rcOldSelected;

	//�����϶�����
	Resource* pRes = ResourceManager::getSingleton().GetResourceArtGroup(_strResGroup);
	if (!pRes)
		pRes = ResourceManager::getSingleton().GetResourceGameObjectGroup(_strResGroup);

	if (pRes)
	{
		CRect rc = ToolManager::getSingleton().GetMap()->GetPixelCoordRect(_ptGrid);
		CRect rcDest = pRes->GetResItemBoundingRect(rc, ToolManager::getSingleton().GetMap()->GetType(), _strResID);

		rcDirty.UnionRect(&rcDirty, rcDest);
		_rcOldSelected = rcDest;
	}

	if (!rcDirty.IsRectEmpty())
	{
		CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
		pView->LogicInvalidate(rcDirty);
	}
}


//VK_ESCAPE
void ToolBrush::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		ToolManager::getSingleton().SelectTool(eToolSelect);
	}
}

void ToolBrush::OnTurnOn()
{
	ToolBase::OnTurnOn();

}

void ToolBrush::OnTurnOff()
{
	ToolBase::OnTurnOff();

	if (!_rcOldSelected.IsRectEmpty())
	{
		CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
		pView->LogicInvalidate(_rcOldSelected);
	}
}

void ToolBrush::SetResource(const Cactus::String& strRes, const Cactus::String& strID)
{
	_strResGroup	= strRes;
	_strResID	= strID;
}
