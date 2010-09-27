
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "MapView.h"
#include "ResourceTreeView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "TileResView.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	TileResView*	GetTileResView(){ return &_TileResView; }


protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutputWnd			m_wndOutput;
	CPropertiesWnd		m_wndProperties;

	ResourceTreeView	_ResTreePanel;
	MapView				_MapPanel;
	TileResView			_TileResView;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

	afx_msg void OnView_ResTreePanel();
	afx_msg void OnUpdateView_ResTreePanel(CCmdUI* pCmdUI);
	afx_msg void OnView_MapPanel();
	afx_msg void OnUpdateView_MapPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_ResDetailPanel();
	afx_msg void OnUpdateView_ResDetailPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_OutputPanel();
	afx_msg void OnUpdateView_OutputPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_PropertyPanel();
	afx_msg void OnUpdateView_PropertyPanel(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	void InitializeRibbon();
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


