#pragma once
#include "toolbase.h"

struct STile;

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

	void	SelectTile(STile* pTile, bool bSelect = true);


protected:
	STile*	_pSelectedTile;
};
