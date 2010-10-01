// DialogNewLayer.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogNewLayer.h"


// DialogNewLayer dialog

IMPLEMENT_DYNAMIC(DialogNewLayer, CDialog)

DialogNewLayer::DialogNewLayer(CWnd* pParent /*=NULL*/)
	: CDialog(DialogNewLayer::IDD, pParent)
	, _strLayerName(_T("layer_xx"))
	, _iWidth(1024)
	, _iHeight(1024)
	, _iTileSize(64)
{

}

DialogNewLayer::~DialogNewLayer()
{
}

void DialogNewLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LAYERNAME, _strLayerName);
	DDX_Text(pDX, IDC_EDIT_WIDTH, _iWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, _iHeight);
	DDX_Text(pDX, IDC_EDIT_TILE_SIZE, _iTileSize);
}


BEGIN_MESSAGE_MAP(DialogNewLayer, CDialog)
END_MESSAGE_MAP()


// DialogNewLayer message handlers
void DialogNewLayer::OnOK()
{
	UpdateData();

	if (_strLayerName.IsEmpty())
	{
		MessageBox("层名不能为空！", "提示", MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (_iWidth <= 0 || _iHeight <= 0 || _iTileSize <= 1 || _iHeight < _iTileSize || _iWidth < _iTileSize)
	{
		MessageBox("请检查层大小或者Tile大小是否有意义？", "提示", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}
