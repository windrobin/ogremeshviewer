
// TileMapEditorDoc.cpp : CTileMapEditorDoc 类的实现
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "TileMapEditorDoc.h"

#include "DialogFileNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTileMapEditorDoc

IMPLEMENT_DYNCREATE(CTileMapEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CTileMapEditorDoc, CDocument)
END_MESSAGE_MAP()


// CTileMapEditorDoc 构造/析构

CTileMapEditorDoc::CTileMapEditorDoc()
{
	// TODO: 在此添加一次性构造代码

}

CTileMapEditorDoc::~CTileMapEditorDoc()
{
}

BOOL CTileMapEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	DialogFileNew dlg;
	if( dlg.DoModal() != IDOK)
		return FALSE;

	_theMap._iWidth				= dlg._iMapWidth;
	_theMap._iHeight			= dlg._iMapHeight;
	_theMap._iTileWidthDefault	= dlg._iTileSize;
	_theMap._iTileHeightDefault	= dlg._iTileSize;
	
	return TRUE;
}


// CTileMapEditorDoc 序列化

void CTileMapEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CTileMapEditorDoc 诊断

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


// CTileMapEditorDoc 命令
