// DialogFileNew.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogFileNew.h"


// DialogFileNew dialog

IMPLEMENT_DYNAMIC(DialogFileNew, CDialog)

DialogFileNew::DialogFileNew(CWnd* pParent /*=NULL*/)
	: CDialog(DialogFileNew::IDD, pParent)
	, _strFootnotes(_T("请输入备注。"))
	, _strMapName(_T("map_xx"))
{
	_iMapWidth			= 20;
	_iMapHeight			= 20;
	_iUnitTileWidth		= 64;
	_iUnitTileHeight	= 64;
}

DialogFileNew::~DialogFileNew()
{
}

void DialogFileNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, _iMapWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, _iMapHeight);
	DDX_Text(pDX, IDC_EDIT_TILE_WIDTH, _iUnitTileWidth);
	DDX_Text(pDX, IDC_EDIT_TILE_HEIGHT, _iUnitTileHeight);
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
		MessageBox("地图名不能为空！", "提示", MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (_iMapWidth < 1 || _iMapHeight < 1
		|| _iUnitTileWidth < 1 || _iUnitTileHeight < 1)
	{
		MessageBox("请检查地图大小或者Tile大小是否有意义？", "提示", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}
