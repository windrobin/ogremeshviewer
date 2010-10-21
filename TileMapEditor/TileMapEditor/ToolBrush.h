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
	virtual void OnTurnOn();
	virtual void OnTurnOff();

	void		SetResource(const Cactus::String& strRes, const Cactus::String& strID);


protected:
	Cactus::String		_strResKey;
	Cactus::String		_strResID;

	CRect				_rcOldSelected;
};
