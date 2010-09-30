#pragma once
#include "toolbase.h"

class ToolRemove : public ToolBase
{
public:
	ToolRemove();
	virtual ~ToolRemove();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnTurnOn();
	virtual void OnTurnOff();
};
