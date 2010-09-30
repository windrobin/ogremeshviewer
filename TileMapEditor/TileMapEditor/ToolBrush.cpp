#include "StdAfx.h"
#include "ToolBrush.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

ToolBrush::ToolBrush()
{
	_refCursor = RGB(192, 192, 0);
}

ToolBrush::~ToolBrush()
{
}

void ToolBrush::Draw(CDC* pDC)
{
	ToolBase::Draw(pDC);

}

void ToolBrush::OnLButtonDown(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonDown(nFlags, point);

	MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
	if (!pLayer)
		return;

	if( pLayer->ModifyTile(_iGridX, _iGridY, _strResKey, _strResID) )
	{
		CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView(); 
		pView->LogicInvalidate(_rcTile);
	}
}

void ToolBrush::OnLButtonUp(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonUp(nFlags, point);

}

void ToolBrush::OnMouseMove(UINT nFlags, CPoint point)
{
	ToolBase::OnMouseMove(nFlags, point);

	if ( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		MapLayer* pLayer = ToolManager::getSingleton().GetDocument()->GetMap().GetCurLayer();
		if (!pLayer)
			return;

		if( pLayer->ModifyTile(_iGridX, _iGridY, _strResKey, _strResID) )
		{
			CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView(); 
			pView->LogicInvalidate(_rcTile);
		}
	}
}


void ToolBrush::OnTurnOn()
{
	ToolBase::OnTurnOn();

}

void ToolBrush::OnTurnOff()
{
	ToolBase::OnTurnOff();

}

void ToolBrush::SetResource(const Cactus::String& strRes, const Cactus::String& strID)
{
	_strResKey	= strRes;
	_strResID	= strID;
}
