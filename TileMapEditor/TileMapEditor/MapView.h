
#pragma once

#include "ViewTree.h"
#include "PanelToolBar.h"

class MapLayer;

class MapView : public CDockablePane
{
public:
	MapView();
	virtual ~MapView();

	void AdjustLayout();
	void OnChangeVisualStyle();

	void			AddMapLayer(MapLayer* pLayer);

protected:
	CClassToolBar	m_wndToolBar;
	CViewTree		_TreeMapItem;
	CImageList		_TreeImageList;

	HTREEITEM		_hLayerRoot;
	HTREEITEM		_hBrushRoot;

	void FillClassView();

// ÖØÐ´
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnNMClickedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};

