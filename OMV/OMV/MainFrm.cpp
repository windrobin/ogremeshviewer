/*
-----------------------------------------------------------------------------
This source file is part of OMV(Ogre Mesh Viewer)
For the latest info, see http://code.google.com/p/ogremeshviewer/

Copyright (c) 2010 Zhang Kun(zhk.tiger@gmail.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

-----------------------------------------------------------------------------
This software also uses Microsoft Fluent UI.
License terms to copy, use or distribute the Fluent UI are available separately.  
To learn more about our Fluent UI licensing program, please visit 
http://msdn.microsoft.com/officeui.

Generally speaking, Fluent UI is free, if you do not use it make a contest software
like Office.
-----------------------------------------------------------------------------
*/

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OMV.h"

#include "MainFrm.h"

#include "OgreFramework.h"
#include "CameraController.h"
#include "Actor.h"

using namespace Ogre;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	
	ON_COMMAND_RANGE(ID_VIEW_VIEW_ACTOR_PANEL, ID_VIEW_VIEW_PROPERTY_PANEL, &CMainFrame::OnViewPanels)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_VIEW_ACTOR_PANEL, ID_VIEW_VIEW_PROPERTY_PANEL, &CMainFrame::OnUpdateViewPanels)

	ON_COMMAND_RANGE(ID_SCENE_LOOKATMODE, ID_SCENE_WIREFRAME, &CMainFrame::OnSceneOptions)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCENE_LOOKATMODE, ID_SCENE_WIREFRAME, &CMainFrame::OnUpdateSceneOptions)

END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	ASSERT(bNameValid);

	// Load panel images:
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);

	// Init main button:
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

	//-------------------------------------------------------
	// Add "Home" category with "Clipboard" panel:
	bNameValid = strTemp.LoadString(IDS_RIBBON_HOME);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE);

#if 0
	// Create "Clipboard" panel:
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
#endif

#define M_Add_Ribbon_CheckBox(Panel, IDS, ID) \
	{ \
		bNameValid = strTemp.LoadString(IDS); \
		ASSERT(bNameValid); \
		CMFCRibbonButton* pTmp = new CMFCRibbonCheckBox(ID, strTemp); \
		Panel->Add(pTmp); \
	}
	
	// Create and add a "Panels" panel:
	bNameValid = strTemp.LoadString(IDS_RIBBON_VIEW);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

	M_Add_Ribbon_CheckBox(pPanelView, IDS_RIBBON_STATUSBAR, ID_VIEW_STATUS_BAR);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_VIEW_OUTPUT_PANEL, ID_VIEW_VIEW_OUTPUT_PANEL);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_VIEW_MESH_PANEL, ID_VIEW_VIEW_MESH_PANEL);
	
	M_Add_Ribbon_CheckBox(pPanelView, IDS_VIEW_PROPERTY_PANEL, ID_VIEW_VIEW_PROPERTY_PANEL);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_VIEW_ACTOR_PANEL, ID_VIEW_VIEW_ACTOR_PANEL);
	M_Add_Ribbon_CheckBox(pPanelView, IDS_VIEW_ANIMATION_PANEL, ID_VIEW_VIEW_ANIMATION_PANEL);

	//bNameValid = strTemp.LoadString(IDS_RIBBON_OUTLOOKPANEL);
	//ASSERT(bNameValid);
	//CMFCRibbonButton* pBtnOutlookBar = new CMFCRibbonCheckBox(ID_VIEW_OUTLOOK_BAR, strTemp);
	//pPanelView->Add(pBtnOutlookBar);


	//-------------------------------------------------------
	// Create and add a "Scene" panel:
	bNameValid = strTemp.LoadString(IDS_RIBBON_SCENE);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelScene = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

	M_Add_Ribbon_CheckBox(pPanelScene, IDS_RIBBON_SCENE_LOOKATMODE, ID_SCENE_LOOKATMODE);
	M_Add_Ribbon_CheckBox(pPanelScene, IDS_RIBBON_SCENE_WIREFRAME, ID_SCENE_WIREFRAME);
	M_Add_Ribbon_CheckBox(pPanelScene, IDS_RIBBON_SCENE_FPS, ID_SCENE_SHOWFPS);
	M_Add_Ribbon_CheckBox(pPanelScene, IDS_RIBBON_SCENE_AXES, ID_SCENE_SHOWAXES);
	M_Add_Ribbon_CheckBox(pPanelScene, IDS_RIBBON_SCENE_BOUDINGBOX, ID_SCENE_SHOWBOUNDINGBOX);
	M_Add_Ribbon_CheckBox(pPanelScene, IDS_RIBBON_SCENE_BONES, ID_SCENE_SHOWBONES);


	//-------------------------------------------------------
	// Add elements to the right side of tabs:
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* No default command */, TRUE /* Right align */);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	
	
	// Add quick access toolbar commands:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);
	lstQATCmds.AddTail(ID_FILE_PRINT_DIRECT);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Navigation pane will be created at left, so temporary disable docking at the left side:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

#if 0
	// Create and setup "Outlook" navigation bar:
	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_OUTLOOK_BAR, 250))	//ID_VIEW_NAVIGATION
	{
		TRACE0("Failed to create navigation pane\n");
		return -1;      // fail to create
	}

	// Outlook bar is created and docking on the left side should be allowed.
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);
#endif

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	_MeshPanel.EnableDocking(/*CBRS_ALIGN_LEFT | */CBRS_ALIGN_RIGHT);
	DockPane(&_MeshPanel);


	_ActorPanel.EnableDocking(CBRS_ALIGN_RIGHT);
	DockPane(&_ActorPanel);
	CDockablePane* pTabbedBar = NULL;
	_ActorPanel.AttachToTabWnd(&_MeshPanel, DM_SHOW, FALSE, &pTabbedBar);


	_wndProperties.EnableDocking(CBRS_ALIGN_RIGHT);
	DockPane(&_wndProperties);
	_wndProperties.AttachToTabWnd(&_MeshPanel, DM_SHOW, FALSE, &pTabbedBar);


	_AnimationPanel.ShowWindow(SW_SHOW);
	_AnimationPanel.EnableDocking(CBRS_ALIGN_LEFT/* | CBRS_ALIGN_RIGHT*/);
	DockPane(&_AnimationPanel);
	

	_LogPanel.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	DockPane(&_LogPanel);

	return 0;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;


	// Create mesh panel
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_MESH_VIEW);
	ASSERT(bNameValid);
	if (!_MeshPanel.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_VIEW_MESH_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create mesh panel\n");
		return FALSE; // failed to create
	}

	bNameValid = strClassView.LoadString(IDS_ACTOR_PANEL);
	ASSERT(bNameValid);
	if (!_ActorPanel.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_VIEW_ACTOR_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create actor panel\n");
		return FALSE; // failed to create
	}

	// Create animation panel
	bNameValid = strClassView.LoadString(IDS_ANIMATION_VIEW);
	ASSERT(bNameValid);
	if (!_AnimationPanel.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_VIEW_ANIMATION_PANEL, WS_CHILD /*| WS_VISIBLE*/ | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create animation panel\n");
		return FALSE; // failed to create
	}
	

#if 0
	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}
#endif

	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!_LogPanel.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_VIEW_OUTPUT_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}


	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_VIEW_PROPERTY_PANEL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	//HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	_MeshPanel.SetIcon(hClassViewIcon, FALSE);

	HICON hActorViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	_ActorPanel.SetIcon(hActorViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	_LogPanel.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	return TRUE;
}

#if 0
BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth)
{
	CWindowDC dc(NULL);

	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // fail to create
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	DWORD dwPaneStyle = AFX_DEFAULT_TOOLBAR_STYLE | CBRS_FLOAT_MULTI;

	// can float, can auto-hide, can resize, CAN NOT CLOSE
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	m_wndTree.Create(dwTreeStyle, rectDummy, &bar, 1200);
	bNameValid = strTemp.LoadString(IDS_FOLDERS);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&m_wndTree, strTemp, 2, TRUE, dwStyle);

	m_wndCalendar.Create(rectDummy, &bar, 1201);
	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&m_wndCalendar, strTemp, 3, TRUE, dwStyle);

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}
#endif

// CMainFrame diagnostics

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


// CMainFrame message handlers

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

void CMainFrame::OnViewPanels(UINT id)
{
	if (id == ID_VIEW_VIEW_ACTOR_PANEL)
	{
		_ActorPanel.ShowPane(!_ActorPanel.IsVisible(), FALSE, !_ActorPanel.IsVisible());
	}
	else if (id == ID_VIEW_VIEW_OUTPUT_PANEL)
	{
		_LogPanel.ShowPane(!_LogPanel.IsVisible(), FALSE, !_LogPanel.IsVisible());
	}
	else if (id == ID_VIEW_VIEW_MESH_PANEL)
	{
		_MeshPanel.ShowPane(!_MeshPanel.IsVisible(), FALSE, !_MeshPanel.IsVisible());
	}
	else if(id == ID_VIEW_VIEW_PROPERTY_PANEL)
	{
		_wndProperties.ShowPane(!_wndProperties.IsVisible(), FALSE, !_wndProperties.IsVisible());
	}
	else if(id == ID_VIEW_VIEW_ANIMATION_PANEL)
	{
		_AnimationPanel.ShowPane(!_AnimationPanel.IsVisible(), FALSE, !_AnimationPanel.IsVisible());
	}
}

void CMainFrame::OnUpdateViewPanels(CCmdUI* pCmdUI)
{
	if (pCmdUI->m_nID == ID_VIEW_VIEW_ACTOR_PANEL)
	{
		pCmdUI->SetCheck(_ActorPanel.IsVisible());
	}
	else if (pCmdUI->m_nID == ID_VIEW_VIEW_OUTPUT_PANEL)
	{
		pCmdUI->SetCheck(_LogPanel.IsVisible());
	}
	else if (pCmdUI->m_nID == ID_VIEW_VIEW_MESH_PANEL)
	{
		pCmdUI->SetCheck(_MeshPanel.IsVisible());
	}
	else if(pCmdUI->m_nID == ID_VIEW_VIEW_PROPERTY_PANEL)
	{
		pCmdUI->SetCheck(_wndProperties.IsVisible());
	}
	else if(pCmdUI->m_nID == ID_VIEW_VIEW_ANIMATION_PANEL)
	{
		pCmdUI->SetCheck(_AnimationPanel.IsVisible());
	}
}

extern bool g_bExisting;
void CMainFrame::OnClose()
{
	g_bExisting = true;

	CFrameWndEx::OnClose();
}


void CMainFrame::OnSceneOptions(UINT id)
{
	if (OgreFramework::getSingletonPtr() == NULL)
		return;

	if (ID_SCENE_LOOKATMODE == id)
	{
		bool b = OgreFramework::getSingleton().GetCameraController()->GetLookAtMode();
		OgreFramework::getSingleton().GetCameraController()->SetLookAtMode(!b);

		if (!b)
			OgreFramework::getSingleton().GetCameraController()->StartLookAt();
	}
	else if (ID_SCENE_WIREFRAME == id)
	{
		OgreFramework::getSingleton().ToggleWireframe();
	}
	else if (ID_SCENE_SHOWFPS == id)
	{
		OgreFramework::getSingleton().ToggleFPS();
	}
	else if (ID_SCENE_SHOWAXES == id)
	{
		Actor* pActor = OgreFramework::getSingleton().GetCurrentActor();
		if (!pActor)
		{
			return;
		}

		pActor->ToggleAxes();
	}
	else if (ID_SCENE_SHOWBOUNDINGBOX == id)
	{
		Actor* pActor = OgreFramework::getSingleton().GetCurrentActor();
		if (!pActor)
		{
			return;
		}

		pActor->ToggleBoundingBox();
	}
	else if (ID_SCENE_SHOWBONES == id)
	{
		Actor* pActor = OgreFramework::getSingleton().GetCurrentActor();
		if (!pActor)
		{
			return;
		}

		pActor->ToggleBone();
	}
}

void CMainFrame::OnUpdateSceneOptions(CCmdUI* pCmdUI)
{
	if (OgreFramework::getSingletonPtr() == NULL)
		return;

	if (ID_SCENE_LOOKATMODE == pCmdUI->m_nID)
	{
		pCmdUI->SetCheck( OgreFramework::getSingleton().GetCameraController()->GetLookAtMode() );
	}
	else if (ID_SCENE_WIREFRAME == pCmdUI->m_nID)
	{
		pCmdUI->SetCheck( OgreFramework::getSingleton().IsWireframeEnabled() );
	}
	else if (ID_SCENE_SHOWFPS == pCmdUI->m_nID)
	{
		pCmdUI->SetCheck( OgreFramework::getSingleton().IsFPSEnabled() );
	}
	else if (ID_SCENE_SHOWAXES == pCmdUI->m_nID)
	{
		Actor* pActor = OgreFramework::getSingleton().GetCurrentActor();
		if (!pActor)
		{
			pCmdUI->Enable(FALSE);
			return;
		}

		pCmdUI->SetCheck( pActor->IsShowAxes() );
	}
	else if (ID_SCENE_SHOWBOUNDINGBOX == pCmdUI->m_nID)
	{
		Actor* pActor = OgreFramework::getSingleton().GetCurrentActor();
		if (!pActor)
		{
			pCmdUI->Enable(FALSE);
			return;
		}

		pCmdUI->SetCheck( pActor->IsShowBoundingBox() );
	}
	else if (ID_SCENE_SHOWBONES == pCmdUI->m_nID)
	{
		Actor* pActor = OgreFramework::getSingleton().GetCurrentActor();
		if (!pActor)
		{
			pCmdUI->Enable(FALSE);
			return;
		}

		pCmdUI->SetCheck( pActor->IsShowBone() );
	}
}
