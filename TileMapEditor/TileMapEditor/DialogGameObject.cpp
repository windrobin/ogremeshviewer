// DialogGameObject.cpp : implementation file
//

#include "stdafx.h"
#include "TileMapEditor.h"
#include "DialogGameObject.h"

#include "ResourceManager.h"
#include "ResourceTile.h"

#include "GameObjectEditorObjects.h"

// CDialogGameObject dialog

IMPLEMENT_DYNAMIC(CDialogGameObject, CDialog)

CDialogGameObject::CDialogGameObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGameObject::IDD, pParent)
	, _strGOGroupName(_T(""))
	, _strGOName(_T(""))
	, _iTileW(64)
	, _iTileH(64)
	, _iTileCount(15)
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
	DDX_Text(pDX, IDC_EDIT_GO_TILE_COUNT_X, _iTileCount);
	DDX_Text(pDX, IDC_EDIT_GO_CENTER_POS, _strCenterOffset);
	DDX_Text(pDX, IDC_EDIT_GO_ARTGROUP, _strArtSource);
	DDX_Control(pDX, IDC_COMBO_GO_ARTID, _comboArt);
	DDX_Radio(pDX, IDC_RADIO_GO_SELECT, _iMode);
	DDX_Text(pDX, IDC_EDIT_GO_MAPTYPE, _strMapType);
	DDX_Control(pDX, IDC_COMBO_GO_AI, _comboAIType);
	DDX_Control(pDX, IDC_SPIN, _spinTileCount);
}


BEGIN_MESSAGE_MAP(CDialogGameObject, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GO_CUR_BRUSH, &CDialogGameObject::OnBnClickedButtonGoCurBrush)
	ON_BN_CLICKED(IDC_BUTTON_GO_OK, &CDialogGameObject::OnBnClickedButtonGoOk)
	ON_BN_CLICKED(IDC_BUTTON_GO_CANCEL, &CDialogGameObject::OnBnClickedButtonGoCancel)
	ON_EN_CHANGE(IDC_EDIT_GO_TILE_COUNT_X, &CDialogGameObject::OnEnChangeEditGoTileCount)
END_MESSAGE_MAP()


BOOL CDialogGameObject::OnInitDialog()
{
	CDialog::OnInitDialog();

	_spinTileCount.SetRange(10, 300);
	_spinTileCount.SetBuddy(GetDlgItem(IDC_EDIT_GO_TILE_COUNT_X));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

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

void CDialogGameObject::EnumArtResItem(const Cactus::String& strResItem)
{
	_comboArt.ResetContent();

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup((LPCTSTR)_strArtSource);
	if (pResTile)
	{
		Cactus::StringVector* vectorCaps = pResTile->GetCaptions();
		for (size_t t = 0; t < vectorCaps->size(); ++t)
		{
			_comboArt.InsertString(t, (*vectorCaps)[t].c_str() );
		}

		if (vectorCaps->size())
		{
			if (strResItem.size())
				_comboArt.SelectString(0, strResItem.c_str());
			else
				_comboArt.SetCurSel(0);
		}
	}
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

void CDialogGameObject::DrawEditingObject(CDC* pDC, CPoint pt)
{
	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup((LPCTSTR)_strArtSource);
	if (pResTile)
	{
		CRect rc;
		rc.left = pt.x;
		rc.top	= pt.y;

		CString strLabel;
		_comboArt.GetLBText(_comboArt.GetCurSel(), strLabel);

		if (strLabel.GetLength())
		{
			pResTile->Draw(pDC, rc, eGridNone, (LPCTSTR)strLabel);
		}
	}

}

void CDialogGameObject::OnEnChangeEditGoTileCount()
{
	UpdateData(TRUE);

	if (_iTileCount < 10)
	{
		_iTileCount = 10;
		UpdateData(FALSE);
	}

	GameObjectEditorView* pView = GetGOView();
	pView->Invalidate(TRUE);
}
