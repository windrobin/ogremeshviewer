#pragma once

class ToolBase
{
public:
	ToolBase();
	virtual ~ToolBase();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	virtual void OnTurnOn();
	virtual void OnTurnOff();

protected:

	bool				_bInRegion;
	bool				_bDrawCursor;

	CRect				_rcTile;
	int					_iGridX;
	int					_iGridY;

	COLORREF			_refCursor;
};
