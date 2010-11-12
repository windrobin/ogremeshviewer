#include "StdAfx.h"
#include "ToolSelect.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

ToolSelect::ToolSelect()
{
	_refCursor = RGB(192, 192, 192);
	_pSelectedTile = 0;
}

ToolSelect::~ToolSelect()
{
}

void ToolSelect::Draw(CDC* pDC)
{
	ToolBase::Draw(pDC);

}

void ToolSelect::OnLButtonDown(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonDown(nFlags, point);

	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	STile* pTile = pLayer->TileHitTest(point, _ptStartGrid);
	if (!pTile)
		return;

	SelectTile(pTile, true);
}

void ToolSelect::OnLButtonUp(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonUp(nFlags, point);

}

void ToolSelect::OnMouseMove(UINT nFlags, CPoint point)
{
	ToolBase::OnMouseMove(nFlags, point);

	if ( _pSelectedTile && (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		//编辑
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		CPoint ptCurGrid = _ptGrid;
		CPoint ptOffset = ptCurGrid - _ptStartGrid;
		if (ptOffset.x != 0 || ptOffset.y != 0 )
		{
			CPoint ptNew = _pSelectedTile->_ptGrid;
			ptNew.Offset(ptOffset);
			_ptStartGrid = ptCurGrid;

			if( pLayer->MoveTile(_pSelectedTile, ptNew) )
			{
				Log_Info("Tile移动成功，位置: (" << _pSelectedTile->_ptGrid.x << ", " << _pSelectedTile->_ptGrid.y << ") 资源组：" << _pSelectedTile->_strResGroup << " : " << _pSelectedTile->_strResItemID);
			}
		}
	}

}


void ToolSelect::OnTurnOn()
{
	ToolBase::OnTurnOn();

}

void ToolSelect::OnTurnOff()
{
	ToolBase::OnTurnOff();

	if (_pSelectedTile)
	{
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		_pSelectedTile->_bSelected = false;
		pLayer->UpdateTileVisual(_pSelectedTile);
		_pSelectedTile = 0;
	}

}

void ToolSelect::SelectTile(STile* pTile, bool bSelect/* = true*/)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	if (!bSelect)
	{
		_pSelectedTile = 0;
		pTile->_bSelected = false;
		pLayer->UpdateTileVisual(pTile);
		return;
	}

	if (_pSelectedTile)
	{
		if(pTile != _pSelectedTile)
		{
			_pSelectedTile->_bSelected = false;
			pLayer->UpdateTileVisual(_pSelectedTile);

			pTile->_bSelected = true;
			pLayer->UpdateTileVisual(pTile);

			_pSelectedTile = pTile;
		}
	}
	else
	{
		_pSelectedTile = pTile;
		_pSelectedTile->_bSelected = true;
		pLayer->UpdateTileVisual(_pSelectedTile);
	}
}

//VK_ESCAPE VK_DELETE VK_SHIFT
void ToolSelect::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		//DeSelect
		if (_pSelectedTile)
			SelectTile(_pSelectedTile, false);
	}
	else if (nChar == VK_DELETE)
	{
		if (!_pSelectedTile)
			return;

		//Delete
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		if( pLayer->RemoveTile(_pSelectedTile->_ptGrid) )
		{
			_pSelectedTile = 0;

			Log_Info("删除Tile成功" );
		}
	}

}