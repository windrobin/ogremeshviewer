// DialogFileNew.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogFileNew.h"


// DialogFileNew dialog

IMPLEMENT_DYNAMIC(DialogFileNew, CDialog)

DialogFileNew::DialogFileNew(CWnd* pParent /*=NULL*/)
	: CDialog(DialogFileNew::IDD, pParent)
	, _strFootnotes(_T("�����뱸ע��"))
	, _strMapName(_T("map_xx"))
{
	_iMapWidth	= 1024;
	_iMapHeight	= 1024;
	_iTileSize	= 64;
}

DialogFileNew::~DialogFileNew()
{
}

void DialogFileNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, _iMapWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, _iMapHeight);
	DDX_Text(pDX, IDC_EDIT_TILE_SIZE, _iTileSize);
	DDX_Text(pDX, IDC_EDIT_FOOTNOTES, _strFootnotes);
	DDX_Text(pDX, IDC_EDIT_MAPNAME, _strMapName);
}


BEGIN_MESSAGE_MAP(DialogFileNew, CDialog)
END_MESSAGE_MAP()


// DialogFileNew message handlers

void DialogFileNew::OnOK()
{
	UpdateData();

	if (_strMapName.IsEmpty())
	{
		MessageBox("��ͼ������Ϊ�գ�", "��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (_iMapWidth <= 0 || _iMapHeight <= 0 || _iTileSize <= 1 || _iMapHeight < _iTileSize || _iMapWidth < _iTileSize)
	{
		MessageBox("�����ͼ��С����Tile��С�Ƿ������壿", "��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}
