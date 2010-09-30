#include "StdAfx.h"
#include "ToolRemove.h"

ToolRemove::ToolRemove()
{
	_refCursor = RGB(192, 0, 0);
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

}

void ToolRemove::OnLButtonUp(UINT nFlags, CPoint point)
{
	ToolBase::OnLButtonUp(nFlags, point);

}

void ToolRemove::OnMouseMove(UINT nFlags, CPoint point)
{
	ToolBase::OnMouseMove(nFlags, point);

}


void ToolRemove::OnTurnOn()
{
	ToolBase::OnTurnOn();

}

void ToolRemove::OnTurnOff()
{
	ToolBase::OnTurnOff();

}
