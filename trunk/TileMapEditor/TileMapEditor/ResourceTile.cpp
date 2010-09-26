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
	for (BitmapListType::iterator it = _BitmapTiles.begin(); it != _BitmapTiles.end(); ++it)
	{
		(*it)->DeleteObject();
		delete *it;
	}
}

void ResourceTile::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, ResourceName, ResourceTile, Resource, "ͼ����.", BaseProperty::eDefault, _strName);

	pProp = M_RegisterPropertySimple(int, TileWidth, ResourceTile, Resource, "tile��λ���.", BaseProperty::eDefault, _tileWidth);
	pProp = M_RegisterPropertySimple(int, TileHeight, ResourceTile, Resource, "tile��λ�߶�.", BaseProperty::eDefault, _tileHeight);
	
	pProp = M_RegisterPropertySimple(int, TileCount, ResourceTile, Resource, "tile����.", BaseProperty::eDefault, _tilesCount);
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

	pProp = M_RegisterPropertySimple(Cactus::String, ResourceName, ResourceTileSingleImage, Resource, "ͼ���ļ���.", BaseProperty::eDefault, _strImageName);
	pProp->SetValueSpecify(eFilePathName, "");

	pProp = M_RegisterPropertySimple(int, ImageWidth, ResourceTileSingleImage, Resource, "ͼ����.", BaseProperty::eReadOnly, _imageWidth);
	pProp = M_RegisterPropertySimple(int, ImageHeight, ResourceTileSingleImage, Resource, "ͼ��߶�.", BaseProperty::eReadOnly, _imageHeight);
}

void ResourceTileSingleImage::OnPropertyChanged(const std::string& propName)
{
}

bool ResourceTileSingleImage::Load()
{
	String strFull = ResourceManager::getSingleton().GetRootFolder() + _strImageName;

	if( _image.Load(_strImageName.c_str()) )
	{
		_imageWidth		= _image.GetWidth();
		_imageHeight	= _image.GetHeight();

		return true;
	}

	return true;
}

void ResourceTileSingleImage::CreateImageList()
{
	if (_bHasImageList)
		return;

	//_image.GetColorType()

	_imageList.Create(_tileWidth, _tileHeight, ILC_COLOR32, 4);

	CDC dcMem;
	dcMem.CreateCompatibleDC(0);

	int iLineCount = _imageWidth/_tileWidth;

	for (int i = 0; i < _tilesCount; ++i)
	{
		CBitmap* bmpTile = new CBitmap;
		CBitmap* bmpOld;
		bmpOld = dcMem.SelectObject(bmpTile);

		_image.Draw(dcMem.GetSafeHdc()
			, (i % iLineCount) * _tileWidth, (i / iLineCount) * iLineCount * _tileHeight
			, _tileWidth, _tileHeight
			);

		dcMem.SelectObject(bmpOld);

		_imageList.Add(bmpTile, (CBitmap*)0);

		_BitmapTiles.push_back(bmpTile);
	}

	_bHasImageList = true;
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

	pProp = M_RegisterPropertySimple(Cactus::String, FolderName, ResourceTileFolder, Resource, "ͼ����.", BaseProperty::eDefault, _strFolderName);
	pProp->SetValueSpecify(eFilePathName, "");

	pProp = M_RegisterPropertySimple(Cactus::String, FileExt, ResourceTileFolder, Resource, "ͼ���ļ���չ��.", BaseProperty::eDefault, _strFileExt);
}

void ResourceTileFolder::OnPropertyChanged(const std::string& propName)
{
}

bool ResourceTileFolder::Load()
{
	String strFull = ResourceManager::getSingleton().GetRootFolder() + _strFolderName;

	Cactus::ostringstream os;
	for (int i = 0; i < _tilesCount; ++i)
	{
		os.str("");
		os << strFull << std::setw(_iBits) << std::setfill('0') << i << _strFileExt;

		CxImage* pImage = new CxImage;
		if ( pImage->Load( os.str().c_str() ) )
		{
			if (pImage->GetWidth() == _tileWidth && pImage->GetHeight() == _tileHeight)
			{
				_images[i] = pImage;
				continue;
			}
		}

		delete pImage;
	}

	return (_images.size() != 0);
}

void ResourceTileFolder::CreateImageList()
{
	if (_bHasImageList)
		return;

	_imageList.Create(_tileWidth, _tileHeight, ILC_COLOR32, 4);

	CDC dcMem;
	dcMem.CreateCompatibleDC(0);

	for (IDImageMapType::iterator it = _images.begin(); it != _images.end(); ++it)
	{
		CBitmap* bmpTile = new CBitmap;
		CBitmap* bmpOld;
		bmpOld = dcMem.SelectObject(bmpTile);

		it->second->Draw(dcMem.GetSafeHdc());

		dcMem.SelectObject(bmpOld);

		_imageList.Add(bmpTile, (CBitmap*)0);

		_BitmapTiles.push_back(bmpTile);
	}

	_bHasImageList = true;
}

//---------------------------------------------------------------------------------------------------------
