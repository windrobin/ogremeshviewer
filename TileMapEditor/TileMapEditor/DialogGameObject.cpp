// DialogGameObject.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogGameObject.h"


// CDialogGameObject dialog

IMPLEMENT_DYNAMIC(CDialogGameObject, CDialog)

CDialogGameObject::CDialogGameObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGameObject::IDD, pParent)
	, _strGOGroupName(_T(""))
	, _strGOName(_T(""))
	, _iTileW(64)
	, _iTileH(64)
	, _iTileCountX(10)
	, _iTileCountY(10)
	, _strCenterOffset(_T(""))
	, _strArtSource(_T(""))
	, _iMode(0)
	, _strMapType(_T(""))
{

}

CDialogGameObject::~CDialogGameObject()
{
}

void CDialogGameObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GO_GROUP, _strGOGroupName);
	DDX_Text(pDX, IDC_EDIT_GO_OBJ_NAME, _strGOName);
	DDX_Text(pDX, IDC_EDIT_GO_TILEW, _iTileW);
	DDX_Text(pDX, IDC_EDIT_GO_TILEH, _iTileH);
	DDX_Text(pDX, IDC_EDIT_GO_TILE_COUNT_X, _iTileCountX);
	DDX_Text(pDX, IDC_EDIT_GO_TILE_COUNT_Y, _iTileCountY);
	DDX_Text(pDX, IDC_EDIT_GO_CENTER_POS, _strCenterOffset);
	DDX_Text(pDX, IDC_EDIT_GO_ARTGROUP, _strArtSource);
	DDX_Control(pDX, IDC_COMBO_GO_ARTID, _comboArt);
	DDX_Radio(pDX, IDC_RADIO_GO_SELECT, _iMode);
	DDX_Text(pDX, IDC_EDIT_GO_MAPTYPE, _strMapType);
	DDX_Control(pDX, IDC_COMBO_GO_AI, _comboAIType);
}


BEGIN_MESSAGE_MAP(CDialogGameObject, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GO_CUR_BRUSH, &CDialogGameObject::OnBnClickedButtonGoCurBrush)
	ON_BN_CLICKED(IDC_BUTTON_GO_OK, &CDialogGameObject::OnBnClickedButtonGoOk)
	ON_BN_CLICKED(IDC_BUTTON_GO_CANCEL, &CDialogGameObject::OnBnClickedButtonGoCancel)
END_MESSAGE_MAP()


// CDialogGameObject message handlers

void CDialogGameObject::OnBnClickedButtonGoCurBrush()
{
	// TODO: Add your control notification handler code here
}

void CDialogGameObject::OnBnClickedButtonGoOk()
{
	// TODO: Add your control notification handler code here
}

void CDialogGameObject::OnBnClickedButtonGoCancel()
{
	// TODO: Add your control notification handler code here
}
