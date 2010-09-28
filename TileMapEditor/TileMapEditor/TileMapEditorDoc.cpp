
// TileMapEditorDoc.cpp : CTileMapEditorDoc ���ʵ��
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "TileMapEditorDoc.h"

#include "DialogFileNew.h"
#include "MainFrm.h"
#include "MapView.h"

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

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	DialogFileNew dlg;
	if( dlg.DoModal() != IDOK)
		return FALSE;

	_theMap._iWidth				= dlg._iMapWidth;
	_theMap._iHeight			= dlg._iMapHeight;
	_theMap._iTileWidthDefault	= dlg._iTileSize;
	_theMap._iTileHeightDefault	= dlg._iTileSize;
	
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
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;

	if( !_theMap.Load(lpszPathName) )
	{
		AfxMessageBox("����ʧ�ܣ������ļ��Ƿ�Ϸ���", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	MapView* pMapView = pMainFrame->GetMapView();


	return TRUE;
}

BOOL CTileMapEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	_theMap.Save(lpszPathName);

	//return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}
