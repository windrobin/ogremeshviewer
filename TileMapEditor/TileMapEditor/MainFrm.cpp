
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "TileMapEditor.h"

#include "MainFrm.h"
#include "ToolManager.h"
#include "MapLayer.h"

#define M_TOOL_SELECT	(WM_USER + 100)
#define M_TOOL_BRUSH	(WM_USER + 101)
#define M_TOOL_REMOVE	(WM_USER + 102)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)

	ON_COMMAND(ID_VIEW_RES_PANEL, &CMainFrame::OnView_ResTreePanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RES_PANEL, &CMainFrame::OnUpdateView_ResTreePanel)

	ON_COMMAND(ID_VIEW_MAP_PANEL, &CMainFrame::OnView_MapPanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_PANEL, &CMainFrame::OnUpdateView_MapPanel)

	ON_COMMAND(ID_VIEW_RES_DETAIL_PANEL, &CMainFrame::OnView_ResDetailPanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RES_DETAIL_PANEL, &CMainFrame::OnUpdateView_ResDetailPanel)

	ON_COMMAND(ID_VIEW_OUTPUTWND, &CMainFrame::OnView_OutputPanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUTWND, &CMainFrame::OnUpdateView_OutputPanel)

	ON_COMMAND(ID_VIEW_PROPERTIESWND, &CMainFrame::OnView_PropertyPanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIESWND, &CMainFrame::OnUpdateView_PropertyPanel)

	ON_COMMAND(ID_VIEW_MAP_THUMBNAIL_PANEL, &CMainFrame::OnView_MapThumbnailPanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_THUMBNAIL_PANEL, &CMainFrame::OnUpdateView_MapThumbnailPanel)
	
	ON_COMMAND(ID_VIEW_MAPLAYER_PANEL, &CMainFrame::OnView_MayLayerPanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAPLAYER_PANEL, &CMainFrame::OnUpdateView_MayLayerPanel)

	ON_COMMAND(ID_VIEW_GAMEOBJECTEDITOR, &CMainFrame::OnView_GameObjectEditorPanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GAMEOBJECTEDITOR, &CMainFrame::OnUpdateView_GameObjectEditorPanel)


	ON_COMMAND(M_TOOL_SELECT, &CMainFrame::OnTool_Select)
	ON_UPDATE_COMMAND_UI(M_TOOL_SELECT, &CMainFrame::OnUpdateButton_Select)
	ON_COMMAND(M_TOOL_BRUSH, &CMainFrame::OnTool_Brush)
	ON_UPDATE_COMMAND_UI(M_TOOL_BRUSH, &CMainFrame::OnUpdateButton_Brush)
	ON_COMMAND(M_TOOL_REMOVE, &CMainFrame::OnTool_Remove)
	ON_UPDATE_COMMAND_UI(M_TOOL_REMOVE, &CMainFrame::OnUpdateButton_Remove)


END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	//Left
	_ResTreePanel.EnableDocking(CBRS_ALIGN_ANY);
	_MapPanel.EnableDocking(CBRS_ALIGN_ANY);
	_LayerPanel.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&_ResTreePanel);
	_LayerPanel.DockToWindow(&_ResTreePanel, CBRS_ALIGN_BOTTOM, CRect(0, 0, 200, 200));
	_MapPanel.DockToWindow(&_ResTreePanel, CBRS_ALIGN_BOTTOM, CRect(0, 0, 200, 200));


	//Right
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	_TileResView.EnableDocking(CBRS_ALIGN_ANY);
	_MapThumbnailPanel.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndProperties);
	_TileResView.DockToWindow(&m_wndProperties, CBRS_ALIGN_TOP, CRect(0, 0, 200, 150));
	_MapThumbnailPanel.DockToWindow(&m_wndProperties, CBRS_ALIGN_BOTTOM, CRect(0, 0, 200, 150));


	//Bottom
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	_GameObjectEditor.EnableDocking(CBRS_ALIGN_BOTTOM);
	DockPane(&m_wndOutput);
	DockPane(&_GameObjectEditor);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	ASSERT(bNameValid);


	// �������ͼ��:
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);


	// ��ʼ����ť:
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0, 0));
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPEN);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1, 1));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2, 2));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVEAS);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, strTemp, 3, 3));

	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, strTemp, 6, 6);
	pBtnPrint->SetKeys(_T("p"), _T("w"));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_LABEL);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonLabel(strTemp));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_QUICK);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_DIRECT, strTemp, 7, 7, TRUE));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_PREVIEW);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, strTemp, 8, 8, TRUE));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_SETUP);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, strTemp, 11, 11, TRUE));
	pMainPanel->Add(pBtnPrint);
	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));

	bNameValid = strTemp.LoadString(IDS_RIBBON_CLOSE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, strTemp, 9, 9));

	bNameValid = strTemp.LoadString(IDS_RIBBON_RECENT_DOCS);
	ASSERT(bNameValid);
	pMainPanel->AddRecentFilesList(strTemp);

	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));



	// Ϊ�������塱�����ӡ���ҳ�����:
	bNameValid = strTemp.LoadString(IDS_RIBBON_HOME);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE);

	
	// �����������塱���:
	bNameValid = strTemp.LoadString(IDS_RIBBON_CLIPBOARD);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelClipboard = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon(27));

	bNameValid = strTemp.LoadString(IDS_RIBBON_PASTE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnPaste = new CMFCRibbonButton(ID_EDIT_PASTE, strTemp, 0, 0);
	pPanelClipboard->Add(pBtnPaste);

	bNameValid = strTemp.LoadString(IDS_RIBBON_CUT);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_CUT, strTemp, 1));
	bNameValid = strTemp.LoadString(IDS_RIBBON_COPY);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_COPY, strTemp, 2));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SELECTALL);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_SELECT_ALL, strTemp, -1));



	// ��������ӡ����������:
	CMFCRibbonPanel* pPanelOperations = pCategoryHome->AddPanel("����");
	pPanelOperations->Add(new CMFCRibbonUndoButton(ID_EDIT_UNDO, "����", 0, 2));
	pPanelOperations->Add(new CMFCRibbonUndoButton(ID_EDIT_REDO, "����", 0, 3));


	// ��������ӡ���ͼ�����:
	CMFCRibbonPanel* pPanelMapEdit = pCategoryHome->AddPanel("��ͼ");
	CMFCRibbonLabel* pLabel = new CMFCRibbonLabel("��ǰ�㣺");
	pPanelMapEdit->Add(pLabel);

	_pCurLayerLabel = new CMFCRibbonLabel("MMMMMMMMMMMM");
	pPanelMapEdit->Add(_pCurLayerLabel);

	_pCurPositionLabel = new CMFCRibbonLabel("���λ�ã�(xxxx, xxxx)");
	pPanelMapEdit->Add( _pCurPositionLabel  );

	pPanelMapEdit->Add(new CMFCRibbonButton(M_TOOL_SELECT, "ѡȡ", 0, 6));
	pPanelMapEdit->Add(new CMFCRibbonButton(M_TOOL_BRUSH, "��ˢ", 0, 4));
	pPanelMapEdit->Add(new CMFCRibbonButton(M_TOOL_REMOVE, "ɾ��", 0, 5));


	// ��������ӡ���ͼ�����:
#define M_Add_Ribbon_CheckBox(Panel, IDS, ID) \
	{ \
		bNameValid = strTemp.LoadString(IDS); \
		ASSERT(bNameValid); \
		CMFCRibbonButton* pTmp = new CMFCRibbonCheckBox(ID, strTemp); \
		Panel->Add(pTmp); \
	}

	bNameValid = strTemp.LoadString(IDS_RIBBON_VIEW);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

	M_Add_Ribbon_CheckBox(pPanelView, IDS_RES_PANEL, ID_VIEW_RES_PANEL);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_MAP_PANEL, ID_VIEW_MAP_PANEL);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_MAPLAYER_PANEL, ID_VIEW_MAPLAYER_PANEL);

	M_Add_Ribbon_CheckBox(pPanelView, IDS_RES_DETAIL, ID_VIEW_RES_DETAIL_PANEL);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_PROPERTIES_WND, ID_VIEW_PROPERTIESWND);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_MAP_THUMBNAIL, ID_VIEW_MAP_THUMBNAIL_PANEL);

	M_Add_Ribbon_CheckBox(pPanelView, IDS_GAMEOBJECT_EDITOR, ID_VIEW_GAMEOBJECTEDITOR);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_OUTPUT_WND, ID_VIEW_OUTPUTWND);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_RIBBON_STATUSBAR, ID_VIEW_STATUS_BAR);

	
	// ��Ԫ����ӵ�ѡ��Ҳ�:
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* ��Ĭ������*/, TRUE /* �Ҷ���*/);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	
	
	// ��ӿ��ٷ��ʹ���������:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);
	lstQATCmds.AddTail(ID_FILE_PRINT_DIRECT);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ��������ͼ
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_MAP_PANEL);
	ASSERT(bNameValid);
	if (!_MapPanel.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MAP_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	// ������Դ����ͼ
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_RES_PANEL);
	ASSERT(bNameValid);
	if (!_ResTreePanel.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_RES_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������Դ������\n");
		return FALSE; // δ�ܴ���
	}

	
	// ����MapLayer��ͼ
	CString strLayerView;
	bNameValid = strLayerView.LoadString(IDS_MAPLAYER_PANEL);
	ASSERT(bNameValid);
	if (!_LayerPanel.Create(strLayerView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MAPLAYER_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������ͼ�㡱����\n");
		return FALSE; // δ�ܴ���
	}


	// �����������
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}

	// �������Դ���
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ��������ԡ�����\n");
		return FALSE; // δ�ܴ���
	}

	// ����TileResView����
	CString strTmp;
	bNameValid = strTmp.LoadString(IDS_RES_DETAIL);
	ASSERT(bNameValid);
	if (!_TileResView.Create(strTmp, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_RES_DETAIL_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�����TileResView������\n");
		return FALSE; // δ�ܴ���
	}

	bNameValid = strTmp.LoadString(IDS_MAP_THUMBNAIL);
	ASSERT(bNameValid);
	if (!_MapThumbnailPanel.Create(strTmp, this, CRect(0, 0, 200, 150), TRUE, ID_VIEW_MAP_THUMBNAIL_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ���������ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	// ����_GameObjectEditor����
	bNameValid = strTmp.LoadString(IDS_GAMEOBJECT_EDITOR);
	ASSERT(bNameValid);
	if (!_GameObjectEditor.Create("����༭��", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_GAMEOBJECTEDITOR, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�������Ϸ����༭��������\n");
		return FALSE; // δ�ܴ���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	_ResTreePanel.SetIcon(hFileViewIcon, FALSE);

	HICON hMapViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	_MapPanel.SetIcon(hMapViewIcon, FALSE);

	HICON hMapLayerViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_MAPLAYER_HC : IDI_MAPLAYER), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	_LayerPanel.SetIcon(hMapLayerViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnView_ResTreePanel()
{
	_ResTreePanel.ShowPane(!_ResTreePanel.IsVisible(), FALSE, !_ResTreePanel.IsVisible());
}
void CMainFrame::OnUpdateView_ResTreePanel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_ResTreePanel.IsVisible());
}

void CMainFrame::OnView_MapPanel()
{
	_MapPanel.ShowPane(!_MapPanel.IsVisible(), FALSE, !_MapPanel.IsVisible());
}
void CMainFrame::OnUpdateView_MapPanel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_MapPanel.IsVisible());
}

void CMainFrame::OnView_ResDetailPanel()
{
	_TileResView.ShowPane(!_TileResView.IsVisible(), FALSE, !_TileResView.IsVisible());
}
void CMainFrame::OnUpdateView_ResDetailPanel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_TileResView.IsVisible());
}

void CMainFrame::OnView_OutputPanel()
{
	m_wndOutput.ShowPane(!m_wndOutput.IsVisible(), FALSE, !m_wndOutput.IsVisible());
}
void CMainFrame::OnUpdateView_OutputPanel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}

void CMainFrame::OnView_PropertyPanel()
{
	m_wndProperties.ShowPane(!m_wndProperties.IsVisible(), FALSE, !m_wndProperties.IsVisible());
}
void CMainFrame::OnUpdateView_PropertyPanel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndProperties.IsVisible());
}

void CMainFrame::OnView_MapThumbnailPanel()
{
	_MapThumbnailPanel.ShowPane(!_MapThumbnailPanel.IsVisible(), FALSE, !_MapThumbnailPanel.IsVisible());
}
void CMainFrame::OnUpdateView_MapThumbnailPanel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_MapThumbnailPanel.IsVisible());
}

void CMainFrame::OnView_MayLayerPanel()
{
	_LayerPanel.ShowPane(!_LayerPanel.IsVisible(), FALSE, !_LayerPanel.IsVisible());
}
void CMainFrame::OnUpdateView_MayLayerPanel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_LayerPanel.IsVisible());
}

void CMainFrame::OnView_GameObjectEditorPanel()
{
	_GameObjectEditor.ShowPane(!_GameObjectEditor.IsVisible(), FALSE, !_GameObjectEditor.IsVisible());
}
void CMainFrame::OnUpdateView_GameObjectEditorPanel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_GameObjectEditor.IsVisible());
}

void CMainFrame::SetCurLayer(MapLayer* pLayer)
{
	if(pLayer)
	{
		_pCurLayerLabel->SetText(pLayer->GetObjectName().c_str());
		_LayerPanel.SetCurrentLayer(pLayer);
	}
	else
	{
		_pCurLayerLabel->SetText("");
		_LayerPanel.SetCurrentLayer(0);
	}

	ToolManager::getSingleton().OnCurrentMapLayer(pLayer);

	_pCurLayerLabel->Redraw();
}

void CMainFrame::SetCursorPosition(const CPoint& pt)
{
	CString str;
	str.Format("���λ�ã�(%d, %d)", pt.x, pt.y);
	_pCurPositionLabel->SetText(str);
	_pCurPositionLabel->Redraw();
}


void CMainFrame::OnTool_Select()
{
	ToolManager::getSingleton().SelectTool(eToolSelect);
}
void CMainFrame::OnUpdateButton_Select(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(ToolManager::getSingleton().GetCurrentToolType() == eToolSelect);
}

void CMainFrame::OnTool_Brush()
{
	ToolManager::getSingleton().SelectTool(eToolBrush);
}
void CMainFrame::OnUpdateButton_Brush(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(ToolManager::getSingleton().GetCurrentToolType() == eToolBrush);
}

void CMainFrame::OnTool_Remove()
{
	ToolManager::getSingleton().SelectTool(eToolRemove);
}
void CMainFrame::OnUpdateButton_Remove(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(ToolManager::getSingleton().GetCurrentToolType() == eToolRemove);
}
