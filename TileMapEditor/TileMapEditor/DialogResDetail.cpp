// DialogResDetail.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogResDetail.h"


// DialogResDetail dialog

IMPLEMENT_DYNAMIC(DialogResDetail, CDialog)

DialogResDetail::DialogResDetail(CWnd* pParent /*=NULL*/)
	: CDialog(DialogResDetail::IDD, pParent)
	, _strGroupName(_T(""))
	, _strResName(_T(""))
{

}

DialogResDetail::~DialogResDetail()
{
}

void DialogResDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RES_DETAIL_GROUPNAME, _strGroupName);
	DDX_Text(pDX, IDC_EDIT_RES_DETAIL_RESTYPE, _strResName);
}


BEGIN_MESSAGE_MAP(DialogResDetail, CDialog)
END_MESSAGE_MAP()


// DialogResDetail message handlers
