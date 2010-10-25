#pragma once

#include "ViewTree.h"
#include "PanelToolBar.h"

class ResourceGameObjectGroup;

class ResourceTreeView : public CDockablePane
{
// 构造
public:
	ResourceTreeView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// 属性
protected:

	CClassToolBar m_wndToolBar;

	CViewTree	_ResourceTree;
	CImageList	_TreeImageList;

	HTREEITEM	_treeArtBackgroundRes;
	HTREEITEM	_treeArtTileRes;
	HTREEITEM	_treeGameObjectRes;

	ResourceGameObjectGroup*	_pSelectedGroup;
	HTREEITEM					_hSelectedItem;

protected:
	void FillFileView();

// 实现
public:
	virtual ~ResourceTreeView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();

	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTvnSelchangedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickTree(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnGameObjectGroupAdd();
	afx_msg void OnGameObjectGroupRemove();
	afx_msg void OnUpdateCmdUI_GroupRemove(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

