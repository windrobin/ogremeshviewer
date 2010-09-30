#include "StdAfx.h"
#include "ToolSelect.h"

ToolSelect::ToolSelect()
{
	_refCursor = RGB(192, 192, 192);
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

}
