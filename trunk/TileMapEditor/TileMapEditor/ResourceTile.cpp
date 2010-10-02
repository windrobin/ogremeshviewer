#include "StdAfx.h"
#include "ResourceTile.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

ResourceTile::ResourceTile()
{
	_bHasImageList = false;
}

ResourceTile::~ResourceTile()
{
	for (IDBitmapMapType::iterator it = _BitmapTiles.begin(); it != _BitmapTiles.end(); ++it)
	{
		if (it->second)
		{
			it->second->DeleteObject();
			delete it->second;
		}
	}
}

void ResourceTile::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, ResourceName, ResourceTile, Resource, "图像名.", BaseProperty::eDefault, _strName);

	pProp = M_RegisterPropertySimple(int, TileWidth, ResourceTile, Resource, "tile单位宽度.", BaseProperty::eDefault, _tileWidth);
	pProp = M_RegisterPropertySimple(int, TileHeight, ResourceTile, Resource, "tile单位高度.", BaseProperty::eDefault, _tileHeight);
	
	pProp = M_RegisterPropertySimple(int, TileCount, ResourceTile, Resource, "tile数量.", BaseProperty::eDefault, _tilesCount);
}

void ResourceTile::OnPropertyChanged(const std::string& propName)
{
}

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

	//_image.GetColorType()

	BOOL b = _imageList.Create(_tileWidth, _tileHeight, ILC_COLOR32, 0, 4);

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
		bmpTile->CreateCompatibleBitmap(pDC, _tileWidth, _tileHeight);
		bmpOld = dcMem.SelectObject(bmpTile);

		dcMem.FillSolidRect(0, 0, _tileWidth, _tileHeight, RGB(255, 255, 255));
		dcMem.BitBlt(0, 0, _tileWidth, _tileHeight, &dcBmp, (i % iLineCount) * _tileWidth, (i / iLineCount) * _tileHeight, SRCCOPY);
		dcMem.SelectObject(bmpOld);

		_imageList.Add(bmpTile, RGB(0, 0, 0));

		osCap.str("");
		osCap << i;
		_captions.push_back(osCap.str());

		_BitmapTiles[osCap.str()] = bmpTile;
	}

	bmp.DeleteObject();

	_bHasImageList = true;
}

void ResourceTileSingleImage::Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID)
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

	//pDC->BitBlt(posX, posY, _tileWidth, _tileHeight, &memDC, 0, 0, SRCCOPY);
	pDC->TransparentBlt(posX, posY, _tileWidth, _tileHeight, &memDC, 0, 0, _tileWidth, _tileHeight, 0);

	memDC.SelectObject(pOldBmp);

}

//---------------------------------------------------------------------------------------------------------
ResourceTileFolder::ResourceTileFolder()
{
}

ResourceTileFolder::~ResourceTileFolder()
{
	for (IDImageMapType::iterator it = _images.begin(); it != _images.end(); ++it)
	{
		delete it->second;
	}
}

void ResourceTileFolder::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, FolderName, ResourceTileFolder, Resource, "图像名.", BaseProperty::eDefault, _strFolderName);
	pProp->SetValueSpecify(eFilePathName, "");

	pProp = M_RegisterPropertySimple(Cactus::String, FileExt, ResourceTileFolder, Resource, "图像文件扩展名.", BaseProperty::eDefault, _strFileExt);
}

void ResourceTileFolder::OnPropertyChanged(const std::string& propName)
{
}

bool ResourceTileFolder::Load()
{
	String strFull = ResourceManager::getSingleton().GetRootFolder() + _strFolderName;

	Cactus::ostringstream osFile;
	Cactus::ostringstream osCap;
	for (int i = 0; i < _tilesCount; ++i)
	{
		osFile.str("");
		osFile << strFull << std::setw(_iBits) << std::setfill('0') << i << _strFileExt;

		CxImage* pImage = new CxImage;
		if ( pImage->Load( osFile.str().c_str() ) )
		{
			if (pImage->GetWidth() == _tileWidth && pImage->GetHeight() == _tileHeight)
			{
				osCap.str("");
				osCap << i;
				_captions.push_back(osCap.str());

				_images[osCap.str()] = pImage;

				_BitmapTiles[osCap.str()] = 0;

				continue;
			}
		}

		delete pImage;
	}

	return (_images.size() != 0);
}

void ResourceTileFolder::CreateImageList(CDC* pDC)
{
	if (_bHasImageList)
		return;

	_imageList.Create(_tileWidth, _tileHeight, ILC_COLOR32, 0, 4);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	IDBitmapMapType::iterator itBmp = _BitmapTiles.begin();

	for (IDImageMapType::iterator it = _images.begin(); it != _images.end(); ++it)
	{
		CBitmap* bmpTile = new CBitmap;
		CBitmap* bmpOld;

		bmpTile->CreateCompatibleBitmap(pDC, _tileWidth, _tileHeight);
		bmpOld = dcMem.SelectObject(bmpTile);

		dcMem.FillSolidRect(0, 0, _tileWidth, _tileHeight, RGB(255, 255, 255));
		it->second->Draw(dcMem.GetSafeHdc());

		dcMem.SelectObject(bmpOld);

		_imageList.Add(bmpTile, RGB(0, 0, 0));

		itBmp->second = bmpTile;
		itBmp++;
	}

	_bHasImageList = true;
}

void ResourceTileFolder::Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID)
{
	CreateImageList(pDC);

#if 0
	if (_BitmapTiles.find(strID) == _BitmapTiles.end())
	{
		Log_Error("ResourceTileFolder::Draw, can not find Resource for " << strID);
		return;
	}

	CBitmap* pBmp = _BitmapTiles[strID];

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = memDC.SelectObject(pBmp);

	//pDC->BitBlt(posX, posY, _tileWidth, _tileHeight, &memDC, 0, 0, SRCCOPY);
	pDC->TransparentBlt(posX, posY, _tileWidth, _tileHeight, &memDC, 0, 0, _tileWidth, _tileHeight, 0);

	memDC.SelectObject(pOldBmp);

#else

	if (_images.find(strID) == _images.end())
	{
		Log_Error("ResourceTileFolder::Draw, can not find Resource for " << strID);
		return;
	}

	CxImage* pImage = _images[strID];
	pImage->Draw(pDC->GetSafeHdc(), posX, posY);

#endif
}

//---------------------------------------------------------------------------------------------------------
