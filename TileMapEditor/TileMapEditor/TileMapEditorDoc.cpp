
// TileMapEditorDoc.cpp : CTileMapEditorDoc ���ʵ��
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "TileMapEditorDoc.h"

#include "DialogFileNew.h"
#include "MainFrm.h"
#include "MapView.h"

#include "ToolManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTileMapEditorDoc

IMPLEMENT_DYNCREATE(CTileMapEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CTileMapEditorDoc, CDocument)
END_MESSAGE_MAP()


// CTileMapEditorDoc ����/����

CTileMapEditorDoc::CTileMapEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CTileMapEditorDoc::~CTileMapEditorDoc()
{
}

BOOL CTileMapEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	DialogFileNew dlg;
	if( dlg.DoModal() != IDOK)
		return FALSE;

	_theMap._iWidth				= dlg._iMapWidth;
	_theMap._iHeight			= dlg._iMapHeight;
	_theMap._iTileWidthDefault	= dlg._iTileSize;
	_theMap._iTileHeightDefault	= dlg._iTileSize;

	ToolManager::getSingleton().SetDocument(this);

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	pMainFrame->GetMapView()->Reset();
	pMainFrame->GetPropertyWnd()->Reset();

	pMainFrame->GetMapView()->SetMapObject(&_theMap);

	return TRUE;
}

// CTileMapEditorDoc ���

#ifdef _DEBUG
void CTileMapEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTileMapEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTileMapEditorDoc ����

BOOL CTileMapEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	{
		char buffer[MAX_PATH];
		::GetModuleFileName(0, buffer, MAX_PATH);
		std::string strModulePath = buffer;

		strModulePath.erase(strModulePath.find_last_of('\\') + 1);
		SetCurrentDirectory(strModulePath.c_str());
	}

	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;

	Map map;
	_theMap = map;

	if( !_theMap.Load(lpszPathName) )
	{
		AfxMessageBox("����ʧ�ܣ������ļ��Ƿ�Ϸ���", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->GetPropertyWnd()->Reset();

	MapView* pMapView = pMainFrame->GetMapView();
	pMapView->Reset();
	pMapView->SetMapObject(&_theMap);

	return TRUE;
}

BOOL CTileMapEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	{
		char buffer[MAX_PATH];
		::GetModuleFileName(0, buffer, MAX_PATH);
		std::string strModulePath = buffer;

		strModulePath.erase(strModulePath.find_last_of('\\') + 1);
		SetCurrentDirectory(strModulePath.c_str());
	}

	_theMap.Save(lpszPathName);

	//return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}
