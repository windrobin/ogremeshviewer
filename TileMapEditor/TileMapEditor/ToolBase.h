#pragma once

class ToolBase
{
public:
	ToolBase();
	virtual ~ToolBase();

	virtual void Draw(CDC* pDC) = 0;

	virtual void OnLButtonDown(UINT nFlags, CPoint point) = 0;
	virtual void OnLButtonUp(UINT nFlags, CPoint point) = 0;
	virtual void OnMouseMove(UINT nFlags, CPoint point) = 0;
};
