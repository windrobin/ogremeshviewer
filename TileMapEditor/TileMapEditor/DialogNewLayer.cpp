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
	, _iWInTiles(1)
	, _iHInTiles(1)
{

}

DialogNewLayer::~DialogNewLayer()
{
}

void DialogNewLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LAYERNAME, _strLayerName);
	DDX_Text(pDX, IDC_EDIT_WIDTH, _iWInTiles);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, _iHInTiles);
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

	if (_iWInTiles < 1 || _iHInTiles < 1)
	{
		MessageBox("请检查层的Tile大小是否有意义？", "提示", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}
