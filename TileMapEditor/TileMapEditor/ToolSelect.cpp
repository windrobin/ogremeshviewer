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

	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
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
		MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
		if (!pLayer)
			return;

		CPoint ptCurGrid = CPoint(_iGridX, _iGridY);
		CPoint ptOffset = ptCurGrid - _ptStartGrid;
		if (ptOffset.x != 0 || ptOffset.y != 0 )
		{
			CPoint ptNew = CPoint(_pSelectedTile->_posX, _pSelectedTile->_posY);
			ptNew.Offset(ptOffset);
			_ptStartGrid = ptCurGrid;

			if( pLayer->MoveTile(_pSelectedTile, ptNew) )
			{
				Log_Info("Tile移动成功，位置: (" << _pSelectedTile->_posX << ", " << _pSelectedTile->_posY << ") 资源组：" << _pSelectedTile->_strResGroup << " : " << _pSelectedTile->_strResItemID);
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
		MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
		if (!pLayer)
			return;

		_pSelectedTile->_bSelected = false;
		pLayer->UpdateTileVisual(_pSelectedTile);
		_pSelectedTile = 0;
	}

}

void ToolSelect::SelectTile(STile* pTile, bool bSelect/* = true*/)
{
	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
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
