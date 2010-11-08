#include "StdAfx.h"
#include "ToolGOEditor.h"
#include "GameObjectEditorObjects.h"

using namespace Cactus;

ToolGOEditor::ToolGOEditor()
: _eCurToolMode(eToolModeSelect)
, _bInRegion(false)
, _bDrawCursor(true)
, _refCursor(RGB(231, 165, 25))
{
}

ToolGOEditor::~ToolGOEditor()
{
}

bool ToolGOEditor::ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel)
{
	CDialogGameObject* pGODlg = GetGODlg();

	bool bInRegion = pGODlg->GetGridCoord(ptPixel, ptGrid);
	if (bInRegion)
	{
		rcPixel = pGODlg->GetPixelCoordRect(ptGrid);
	}
	else
	{
		ptGrid.x	= -1;
		ptGrid.y	= -1;
	}

	return bInRegion;
}

void ToolGOEditor::Draw(CDC* pDC)
{
	if (_bInRegion && _bDrawCursor)
	{
		CPen pen(PS_SOLID, 2, _refCursor);

		pDC->SelectStockObject(NULL_BRUSH);
		CPen* pOldPen = pDC->SelectObject(&pen);

		CRect rc = _rcTilePixel;

		CDialogGameObject* pGODlg = GetGODlg();

		if (pGODlg->_iMapType == 0)
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

void ToolGOEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogGameObject* pGODlg = GetGODlg();
	if (!pGODlg)
		return;

	_bInRegion = ToolHitTest(point, _ptGrid, _rcTilePixel);

	_bSelected = pGODlg->HitTest(point);
	if (_bSelected)
	{
		_ptStart = point;
	}
}

void ToolGOEditor::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void ToolGOEditor::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogGameObject* pGODlg = GetGODlg();
	if (!pGODlg)
	{
		_bInRegion = false;
		return;
	}

	GameObjectEditorView* pView = GetGOView();

	if ( _bSelected && (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		CPoint ptOffset = point - _ptStart;
		if (ptOffset.x != 0 || ptOffset.y != 0 )
		{
			_ptStart = point;

			pGODlg->MoveGameObject(ptOffset);
		}
	}

	CRect rcPixel;
	CPoint ptOldGird = _ptGrid;
	if(!ToolHitTest(point, _ptGrid, rcPixel))
	{
		if (_bInRegion)
			pView->LogicInvalidate(&_rcTilePixel);	//刷新老的

		_bInRegion	= false;

		//((CMainFrame*)AfxGetMainWnd())->SetCursorPosition(CPoint(0, 0));

		return;
	}

	if (ptOldGird != _ptGrid)
	{
		if (_bInRegion)
			pView->LogicInvalidate(&_rcTilePixel);	//刷新老的

		_rcTilePixel = rcPixel;
		pView->LogicInvalidate(&_rcTilePixel);		//绘制新的
	}

	_rcTilePixel = rcPixel;
	_bInRegion = true;

	//((CMainFrame*)AfxGetMainWnd())->SetCursorPosition(_ptGrid);
}

void ToolGOEditor::SetCurToolMode(EToolMode e)
{

}
