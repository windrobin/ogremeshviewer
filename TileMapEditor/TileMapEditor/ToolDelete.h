#pragma once
#include "toolbase.h"

class ToolDelete : public ToolBase
{
public:
	ToolDelete();
	virtual ~ToolDelete();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
};
