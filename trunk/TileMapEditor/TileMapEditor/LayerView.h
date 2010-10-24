#pragma once

#include "PanelToolBar.h"

class MapLayer;

class LayerView : public CDockablePane
{
public:
	LayerView();
	virtual ~LayerView();

	void	SetCurrentLayer(MapLayer* pLayer);

protected:
	CClassToolBar	m_wndToolBar;
	CListCtrl		_listObjects;

// ÖØÐ´
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnInvertSelect();
	afx_msg void OnSelectSimilar();
	afx_msg void OnDeleteSelection();

	DECLARE_MESSAGE_MAP()
};

