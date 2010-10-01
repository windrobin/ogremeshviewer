#include "StdAfx.h"
#include "ToolBase.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

ToolBase::ToolBase()
: _bDrawCursor(false)
{
}

ToolBase::~ToolBase()
{
}

void ToolBase::Draw(CDC* pDC)
{
	if (_bDrawCursor)
	{
		CPen pen(PS_SOLID, 2, _refCursor);

		pDC->SelectStockObject(NULL_BRUSH);
		CPen* pOldPen = pDC->SelectObject(&pen);

		CRect rc = _rcTile;
		rc.DeflateRect(1, 1, 0, 0);
		pDC->Rectangle(rc);

		pDC->SelectObject(pOldPen);
	}
}

void ToolBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
	if (!pLayer)
		return;

	_bDrawCursor = pLayer->ToolHitTest(point, _iGridX, _iGridY, _rcTile);
}

void ToolBase::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void ToolBase::OnMouseMove(UINT nFlags, CPoint point)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
	if (!pLayer)
	{
		_bDrawCursor = false;
		return;
	}

	int gridX, gridY;
	CRect rc;
	if(!pLayer->ToolHitTest(point, gridX, gridY, rc))
		return;

	CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView(); 
	if (gridX != _iGridX || gridY != _iGridY)
	{
		if (_bDrawCursor)
			pView->LogicInvalidate(&_rcTile);	//刷新老的

		_rcTile = rc;
		pView->LogicInvalidate(&_rcTile);		//绘制新的
	}

	_iGridX = gridX;
	_iGridY = gridY;
	_rcTile = rc;
	_bDrawCursor = true;
}


void ToolBase::OnTurnOn()
{

}

void ToolBase::OnTurnOff()
{
	if (_bDrawCursor)
	{
		_bDrawCursor = false;

		CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView(); 
		pView->LogicInvalidate(&_rcTile);	//刷新老的
	}

}
