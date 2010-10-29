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
	_refCursor		= RGB(192, 0, 0);
	_bDrawCursor	= true;
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

	if (!_bInRegion)
		return;

	MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
	if (!pLayer)
		return;

	if( pLayer->RemoveTile(_ptGrid) )
	{
		Log_Info("删除Tile成功，位置: (" << _ptGrid.x << ", " << _ptGrid.y << ")" );
	}
}

void ToolRemove::OnLButtonUp(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonUp(nFlags, point);

}

void ToolRemove::OnMouseMove(UINT nFlags, CPoint point)
{
	ToolBase::OnMouseMove(nFlags, point);

	if (!_bInRegion)
		return;

	if ( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		MapLayer* pLayer = ToolManager::getSingleton().GetMap()->GetCurLayer();
		if (!pLayer)
			return;

		if( pLayer->RemoveTile(_ptGrid) )
		{
			Log_Info("删除Tile成功，位置: (" << _ptGrid.x << ", " << _ptGrid.y << ")" );
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
