#include "StdAfx.h"
#include "ToolBrush.h"

#include "TileMapEditorDoc.h"
#include "ToolManager.h"
#include "Map.h"

ToolBrush::ToolBrush()
{
}

ToolBrush::~ToolBrush()
{
}

void ToolBrush::Draw(CDC* pDC)
{

}

void ToolBrush::OnLButtonDown(UINT nFlags, CPoint point)
{

}

void ToolBrush::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void ToolBrush::OnMouseMove(UINT nFlags, CPoint point)
{

}

void ToolBrush::SetResource(const Cactus::String& strRes, const Cactus::String& strID)
{
	_strResKey	= strRes;
	_strResID	= strID;
}
