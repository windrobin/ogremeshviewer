#include "StdAfx.h"
#include "ResourceTileSingleImage.h"
#include "ResourceManager.h"
#include "TileMapEditorView.h"
#include "MainFrm.h"

using namespace Cactus;
using namespace PropertySys;

//---------------------------------------------------------------------------------------------------------
ResourceTileSingleImage::ResourceTileSingleImage()
{
}

ResourceTileSingleImage::~ResourceTileSingleImage()
{
}

void ResourceTileSingleImage::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, ResourceName, ResourceTileSingleImage, Resource, "图像文件名.", BaseProperty::eDefault, _strImageName);
	pProp->SetValueSpecify(eFilePathName, "");

	pProp = M_RegisterPropertySimple(int, ImageWidth, ResourceTileSingleImage, Resource, "图像宽度.", BaseProperty::eReadOnly, _imageWidth);
	pProp = M_RegisterPropertySimple(int, ImageHeight, ResourceTileSingleImage, Resource, "图像高度.", BaseProperty::eReadOnly, _imageHeight);
}

void ResourceTileSingleImage::OnPropertyChanged(const std::string& propName)
{
}

bool ResourceTileSingleImage::Load()
{
	String strFull = ResourceManager::getSingleton().GetRootFolder() + _strImageName;

	if( _image.Load(strFull.c_str()) )
	{
		_imageWidth		= _image.GetWidth();
		_imageHeight	= _image.GetHeight();

		return true;
	}

	return false;
}

void ResourceTileSingleImage::CreateImageList(CDC* pDC)
{
	if (_bHasImageList)
		return;

	BOOL b = _imageList.Create(_iIconSize, _iIconSize, ILC_COLOR32, 0, 4);

	HBITMAP hBmp = _image.MakeBitmap(pDC->GetSafeHdc());
	CBitmap bmp;
	bmp.Attach(hBmp);

	CDC dcBmp;
	dcBmp.CreateCompatibleDC(pDC);
	dcBmp.SelectObject(&bmp);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	int iLineCount = _imageWidth/_tileWidth;

	Cactus::ostringstream osCap;
	for (int i = 0; i < _tilesCount; ++i)
	{
		CBitmap* bmpTile = new CBitmap;
		CBitmap* bmpOld;
		bmpTile->CreateCompatibleBitmap(pDC, _iIconSize, _iIconSize);
		bmpOld = dcMem.SelectObject(bmpTile);

		dcMem.FillSolidRect(0, 0, _iIconSize, _iIconSize, RGB(255, 255, 255));

		if (__max(_tileWidth, _tileHeight) > _iIconSize)
		{
			dcMem.SetStretchBltMode(HALFTONE);
			if (_tileWidth > _tileHeight)
			{
				float fR = 1.0f * _iIconSize / _tileWidth;
				dcMem.StretchBlt(0, 0, _iIconSize, int(_tileHeight * fR)
					, &dcBmp, (i % iLineCount) * _tileWidth, (i / iLineCount) * _tileHeight, _tileWidth, _tileHeight, SRCCOPY);
			}
			else
			{
				float fR = 1.0f * _iIconSize / _tileHeight;
				dcMem.StretchBlt(int((_iIconSize - fR * _tileWidth)/2), 0, int(fR * _tileWidth), _iIconSize
					, &dcBmp, (i % iLineCount) * _tileWidth, (i / iLineCount) * _tileHeight, _tileWidth, _tileHeight, SRCCOPY);
			}
		}
		else
		{
			dcMem.BitBlt((_iIconSize - _tileWidth)/2, 0, _tileWidth, _tileHeight
				, &dcBmp, (i % iLineCount) * _tileWidth, (i / iLineCount) * _tileHeight, SRCCOPY);
		}
		dcMem.SelectObject(bmpOld);

		_imageList.Add(bmpTile, (CBitmap*)0);

		osCap.str("");
		osCap << i;
		_captions.push_back(osCap.str());

		_BitmapTiles[osCap.str()] = bmpTile;
	}

	bmp.DeleteObject();

	_bHasImageList = true;
}

void ResourceTileSingleImage::Draw(CDC* pDC, const CRect& curTile, const Cactus::String& strID)
{
	CreateImageList(pDC);

	if (_BitmapTiles.find(strID) == _BitmapTiles.end())
	{
		Log_Error("ResourceTileSingleImage::Draw, can not find Resource for " << strID);
		return;
	}

	CBitmap* pBmp = _BitmapTiles[strID];

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	CBitmap* pOldBmp = memDC.SelectObject(pBmp);


	BITMAP bmpInfo;
	pBmp->GetBitmap(&bmpInfo);

	int startX = curTile.CenterPoint().x - bmpInfo.bmWidth/2;
	int startY = curTile.CenterPoint().y - bmpInfo.bmHeight/2;

	CRect rcDest(startX, startY, startX + bmpInfo.bmWidth, startY + bmpInfo.bmHeight);

	//pDC->BitBlt(posX, posY, _tileWidth, _tileHeight, &memDC, 0, 0, SRCCOPY);
	pDC->TransparentBlt(startX, startY, rcDest.Width(), rcDest.Height()
		, &memDC, 0, 0, rcDest.Width(),rcDest.Height(), RGB(255,255,255));

	memDC.SelectObject(pOldBmp);

	CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
	pView->LogicInvalidate(rcDest);

}
