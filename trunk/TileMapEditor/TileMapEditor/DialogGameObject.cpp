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

bool CDialogGameObject::GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid)
{
	if (ptPixel.x > GetPixelWidth() || ptPixel.y > GetPixelHeight() || ptPixel.x < 0 || ptPixel.y < 0)
	{
		return false;
	}

	if (_iMapType == 0)
	{
		ptGrid.x	= ptPixel.x / _iTileW;
		ptGrid.y	= ptPixel.y / _iTileH;

		return true;
	}

	bool bInRegion = false;

	int iMapW = GetPixelWidth();
	int iMapH = GetPixelHeight();

	int y = 0;
	float k = 1.0f * iMapH / iMapW;
	if (ptPixel.x <= iMapW/2)
	{
		if ( (ptPixel.y >= -k * ptPixel.x + iMapH/2) && (ptPixel.y <= k * ptPixel.x + iMapH/2) )
			bInRegion = true;
	}
	else
	{
		if ( (ptPixel.y >= k * ptPixel.x - iMapH/2) && (ptPixel.y <= -k * ptPixel.x + 1.5 * iMapH) )
			bInRegion = true;
	}

	if (bInRegion)
	{
		//¼ÆËãy
		//y = kx + b;	b[-0.5H, 0.5H]
		float b = 1.0f * -iMapH/2;
		ptGrid.y = -1;
		while(ptPixel.y > k * ptPixel.x + b)
		{
			b += _iTileH;
			ptGrid.y++;
		}

		//¼ÆËãx
		//y = -kx + b;	b[0.5H, 1.5H]
		b = 1.0f * iMapH/2;
		ptGrid.x = -1;
		while(ptPixel.y > -k * ptPixel.x + b)
		{
			b += _iTileH;
			ptGrid.x++;
		}
	}

	return bInRegion;
}

CRect CDialogGameObject::GetPixelCoordRect(const CPoint& ptGrid)
{
	if (_iMapType == 0)
	{
		return CRect(CPoint(ptGrid.x * _iTileW, ptGrid.y * _iTileH), CSize(_iTileW, _iTileH));
	}
	else
	{
		int xOffset = (GetPixelWidth() - _iTileW)/2;

		int xLeft	= xOffset + (ptGrid.x - ptGrid.y) * _iTileW / 2;
		int yTop	= (ptGrid.x + ptGrid.y) * _iTileH / 2;

		return CRect(CPoint(xLeft, yTop), CSize(_iTileW, _iTileH));
	}
}
