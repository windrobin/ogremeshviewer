#pragma once

class Map;
class MapLayer;

class ToolBase
{
public:
	ToolBase();
	virtual ~ToolBase();

	virtual void Draw(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void OnTurnOn();
	virtual void OnTurnOff();

	virtual void OnNewMap(Map* pMap){}
	virtual void OnMapClose(Map* pMap){}
	virtual void OnCurrentMapLayer(MapLayer* pLayer){}

protected:

	bool				_bInRegion;
	bool				_bDrawCursor;

	CRect				_rcTilePixel;
	CPoint				_ptGrid;

	COLORREF			_refCursor;
};
