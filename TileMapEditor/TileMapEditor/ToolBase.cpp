#include "StdAfx.h"
#include "ToolBase.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

ToolBase::ToolBase()
: _bInRegion(false)
, _bDrawCursor(false)
{
}

ToolBase::~ToolBase()
{
}

void ToolBase::Draw(CDC* pDC)
{
	if (_bInRegion && _bDrawCursor)
	{
		CPen pen(PS_SOLID, 2, _refCursor);

		pDC->SelectStockObject(NULL_BRUSH);
		CPen* pOldPen = pDC->SelectObject(&pen);

		CRect rc = _rcTile;

		if (ToolManager::getSingleton().GetDocument()->GetMap().GetType() == eRectangle)
		{
			rc.DeflateRect(1, 1, 0, 0);

			pDC->Rectangle(rc);
		}
		else
		{
			rc.DeflateRect(1, 1, 1, 1);

			CPoint pts[4];
			pts[0] = CPoint(rc.left, rc.top + rc.Height()/2);
			pts[1] = CPoint(rc.left + rc.Width()/2, rc.top);
			pts[2] = CPoint(rc.right, rc.top + rc.Height()/2);
			pts[3] = CPoint(rc.left + rc.Width()/2, rc.bottom);

			pDC->Polygon(pts, 4);
		}

		pDC->SelectObject(pOldPen);
	}
}

void ToolBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
	if (!pLayer)
		return;

	_bInRegion = pLayer->ToolHitTest(point, _iGridX, _iGridY, _rcTile);
}

void ToolBase::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void ToolBase::OnMouseMove(UINT nFlags, CPoint point)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
	if (!pLayer)
	{
		_bInRegion = false;
		return;
	}

	CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	int gridX, gridY;
	CRect rcPixel;
	if(!pLayer->ToolHitTest(point, gridX, gridY, rcPixel))
	{
		if (_bInRegion)
			pView->LogicInvalidate(&_rcTile);	//刷新老的

		_iGridX = -1;
		_iGridY = -1;

		_bInRegion = false;

		((CMainFrame*)AfxGetMainWnd())->SetCursorPosition(0, 0);

		return;
	}

	if (gridX != _iGridX || gridY != _iGridY)
	{
		if (_bInRegion)
			pView->LogicInvalidate(&_rcTile);	//刷新老的

		_rcTile = rcPixel;
		pView->LogicInvalidate(&_rcTile);		//绘制新的
	}

	_iGridX = gridX;
	_iGridY = gridY;
	_rcTile = rcPixel;
	_bInRegion = true;

	((CMainFrame*)AfxGetMainWnd())->SetCursorPosition(_iGridX, _iGridY);
}


void ToolBase::OnTurnOn()
{

}

void ToolBase::OnTurnOff()
{
	if (_bInRegion)
	{
		_bInRegion = false;

		CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
		pView->LogicInvalidate(&_rcTile);	//刷新老的
	}

}
