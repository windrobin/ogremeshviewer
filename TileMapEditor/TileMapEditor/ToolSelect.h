#pragma once
#include "toolbase.h"

class ToolSelect : public ToolBase
{
public:
	ToolSelect();
	virtual ~ToolSelect();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnTurnOn();
	virtual void OnTurnOff();
};
