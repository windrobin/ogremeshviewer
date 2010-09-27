#pragma once
#include "toolbase.h"

class ToolBrush : public ToolBase
{
public:
	ToolBrush();
	virtual ~ToolBrush();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
};
