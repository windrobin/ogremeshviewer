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

	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
	if (!pLayer)
		return;

	if( pLayer->RemoveTile(_iGridX, _iGridY) )
	{
		Log_Info("删除Tile成功，位置: (" << _iGridX << ", " << _iGridY << ")" );
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
		MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
		if (!pLayer)
			return;

		if( pLayer->RemoveTile(_iGridX, _iGridY) )
		{
			Log_Info("删除Tile成功，位置: (" << _iGridX << ", " << _iGridY << ")" );
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
