#pragma once

#include "PanelToolBar.h"

class GameObjectResView : public CDockablePane
{
public:
	GameObjectResView();
	virtual ~GameObjectResView();

protected:
	CClassToolBar m_wndToolBar;

	// ��д
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnClassAddMemberFunction();

	DECLARE_MESSAGE_MAP()
};

