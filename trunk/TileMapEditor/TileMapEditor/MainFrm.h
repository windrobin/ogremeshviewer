
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "MapView.h"
#include "ResourceTreeView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "TileResView.h"
#include "LayerView.h"
#include "GameObjectEditor.h"
#include "MapThumbnailView.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	TileResView*		GetTileResView(){ return &_TileResView; }
	MapView*			GetMapView(){ return &_MapPanel; }
	CPropertiesWnd*		GetPropertyWnd(){ return &m_wndProperties; }
	LayerView*			GetMapLayerView(){ return &_LayerPanel; }
	MapThumbnailView*	GetMapThumbnailView(){ return &_MapThumbnailPanel; }

	void				SetCurLayerName(const Cactus::String& strLayerName);
	void				SetCursorPosition(int x, int y);


protected:  // 控件条嵌入成员
	CMFCRibbonBar				m_wndRibbonBar;
	CMFCRibbonApplicationButton	m_MainButton;
	CMFCToolBarImages			m_PanelImages;
	CMFCRibbonStatusBar			m_wndStatusBar;
	COutputWnd					m_wndOutput;
	CPropertiesWnd				m_wndProperties;

	ResourceTreeView	_ResTreePanel;
	MapView				_MapPanel;
	TileResView			_TileResView;
	LayerView			_LayerPanel;
	GameObjectEditor	_GameObjectEditor;
	MapThumbnailView	_MapThumbnailPanel;

	CMFCRibbonLabel*	_pCurLayerLabel;
	CMFCRibbonLabel*	_pCurPositionLabel;

// 生成的消息映射函数
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
	afx_msg void OnView_MayLayerPanel();
	afx_msg void OnUpdateView_MayLayerPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_GameObjectEditorPanel();
	afx_msg void OnUpdateView_GameObjectEditorPanel(CCmdUI* pCmdUI);
	afx_msg void OnView_MapThumbnailPanel();
	afx_msg void OnUpdateView_MapThumbnailPanel(CCmdUI* pCmdUI);

	afx_msg void OnTool_Select();
	afx_msg void OnUpdateButton_Select(CCmdUI* pCmdUI);
	afx_msg void OnTool_Brush();
	afx_msg void OnUpdateButton_Brush(CCmdUI* pCmdUI);
	afx_msg void OnTool_Remove();
	afx_msg void OnUpdateButton_Remove(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	void InitializeRibbon();
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


