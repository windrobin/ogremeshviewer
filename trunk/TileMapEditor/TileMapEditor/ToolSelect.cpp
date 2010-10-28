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

	STile* pTile = pLayer->TileHitTest(point);
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
