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

// OMV.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "OMV.h"
#include "MainFrm.h"

#include "OMVDoc.h"
#include "OMVView.h"

#include "OgreFramework.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


//-------------------------------------------------------------------------------------------
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void COMVApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


//-------------------------------------------------------------------------------------------
// COMVApp

BEGIN_MESSAGE_MAP(COMVApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COMVApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// COMVApp construction

COMVApp::COMVApp()
{
	m_bHiColorIcons = TRUE;
}

// The one and only COMVApp object

COMVApp theApp;
bool	g_bExisting = false;


// COMVApp customization load/save methods

void COMVApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void COMVApp::LoadCustomState()
{
}

void COMVApp::SaveCustomState()
{
}

// COMVApp initialization

BOOL COMVApp::InitInstance()
{
	setlocale(LC_CTYPE, "");

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("ZHKSoft"));
	LoadStdProfileSettings(0);  // Load standard INI file options

	new OgreFramework();

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COMVDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(COMVView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//enum { FileNew, FileOpen, FilePrint, FilePrintTo, FileDDE, AppRegister,
	//	AppUnregister, FileNothing = -1 } m_nShellCommand;

	bool bPostProcessShellCommand = false;
	if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew)
	{
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
		{
			bPostProcessShellCommand = true;
		}
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
	}

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	char buffer[MAX_PATH];
	::GetModuleFileName(0, buffer, MAX_PATH);
	std::string strModulePath = buffer;

	strModulePath.erase(strModulePath.find_last_of('\\') + 1);
	SetCurrentDirectory(strModulePath.c_str());

	try
	{
		CView* pView = 0;
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		if (posDoc != NULL)
		{
			CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);

			POSITION posView = pDoc->GetFirstViewPosition();
			if (posView != NULL)
				pView = pDoc->GetNextView(posView);
		}

		initDemo(pView ? pView->GetSafeHwnd() : 0);
	}
	catch(std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	//Handle FileOpen here, after Ogre initialized OK
	if (bPostProcessShellCommand )
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
		if(!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();


	runDemo();

	return TRUE;
}

int COMVApp::ExitInstance()
{
	delete OgreFramework::getSingletonPtr();

	return CWinAppEx::ExitInstance();
}

void COMVApp::initDemo(HWND hwnd)
{
	if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", hwnd))
		return;

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");

	OgreFramework::getSingletonPtr()->setupDemoScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void COMVApp::runDemo()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();


	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	// for tracking the idle time state
	//BOOL bIdle = TRUE;
	//LONG lIdleCount = 0;

	while(!g_bExisting) 
	{
		if(!OgreFramework::getSingletonPtr()->update())
		{
			Sleep(10);
		}

		// Ogre messagePump has no Idle msg handle
		Ogre::WindowEventUtilities::messagePump();

		OnIdle(0);

	}

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}

