
// TileMapEditorDoc.cpp : CTileMapEditorDoc ���ʵ��
//

#include "stdafx.h"
#include "TileMapEditor.h"

#include "TileMapEditorDoc.h"

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

	return TRUE;
}




// CTileMapEditorDoc ���л�

void CTileMapEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
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
