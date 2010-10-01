#include "StdAfx.h"
#include "ToolRemove.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

ToolRemove::ToolRemove()
{
	_refCursor = RGB(192, 0, 0);
}

ToolRemove::~ToolRemove()
{
}

void ToolRemove::Draw(CDC* pDC)
{
	ToolBase::Draw(pDC);

}

void ToolRemove::OnLButtonDown(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonDown(nFlags, point);

	if (!_bDrawCursor)
		return;

	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
	if (!pLayer)
		return;

	if( pLayer->ClearTile(_iGridX, _iGridY) )
	{
		CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView(); 
		pView->LogicInvalidate(_rcTile);
	}
}

void ToolRemove::OnLButtonUp(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonUp(nFlags, point);

}

void ToolRemove::OnMouseMove(UINT nFlags, CPoint point)
{
	ToolBase::OnMouseMove(nFlags, point);

	if (!_bDrawCursor)
		return;

	if ( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
		if (!pLayer)
			return;

		if( pLayer->ClearTile(_iGridX, _iGridY) )
		{
			CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView(); 
			pView->LogicInvalidate(_rcTile);
		}
	}
}


void ToolRemove::OnTurnOn()
{
	ToolBase::OnTurnOn();

}

void ToolRemove::OnTurnOff()
{
	ToolBase::OnTurnOff();

}
