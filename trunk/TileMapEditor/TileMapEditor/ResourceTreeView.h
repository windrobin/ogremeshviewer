#pragma once

#include "ViewTree.h"
#include "PanelToolBar.h"

class ResourceTreeView : public CDockablePane
{
// ����
public:
	ResourceTreeView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// ����
protected:

	CClassToolBar m_wndToolBar;

	CViewTree	_ResourceTree;
	CImageList	_TreeImageList;

	HTREEITEM	_treeArtBackgroundRes;
	HTREEITEM	_treeArtTileRes;
	HTREEITEM	_treeGameObjectRes;
	HTREEITEM	_treeGameEventRes;

protected:
	void FillFileView();

// ʵ��
public:
	virtual ~ResourceTreeView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	//afx_msg void OnFileOpen();
	//afx_msg void OnFileOpenWith();
	//afx_msg void OnDummyCompile();
	//afx_msg void OnEditCut();
	//afx_msg void OnEditCopy();
	//afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTvnSelchangedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};

