#include "StdAfx.h"
#include "ResourceTileFolder.h"
#include "ResourceManager.h"
#include "TileMapEditorView.h"
#include "MainFrm.h"

#include "ToolManager.h"
#include "Map.h"

using namespace Cactus;
using namespace PropertySys;


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

	pProp = M_RegisterPropertySimple(Cactus::String, FolderName, ResourceTileFolder, Resource, "图像目录名.", BaseProperty::eDefault, _strFolderName);
	pProp->SetValueSpecify(eFilePathName, "");

	pProp = M_RegisterPropertySimple(Cactus::String, FileExt, ResourceTileFolder, Resource, "图像文件扩展名.", BaseProperty::eDefault, _strFileExt);
}

void ResourceTileFolder::OnPropertyChanged(const std::string& propName)
{
}

bool ResourceTileFolder::Load()
{
	_tilesCount = 0;

	String strFull = ResourceManager::getSingleton().GetRootFolder() + _strFolderName;

	CFileFind finder;
	CString str = strFull.c_str() + CString("*.") + _strFileExt.c_str();

	// start working for files
	BOOL bWorking = finder.FindFile(str);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// skip directory
		if (finder.IsDirectory())
			continue;

		CxImage* pImage = new CxImage;
		if ( pImage->Load( finder.GetFilePath() ) )
		{
			String strTitle = (LPCTSTR)finder.GetFileTitle();
			_captions.push_back( strTitle );

			_images[strTitle] = pImage;
			_BitmapTiles[strTitle] = 0;

			_tilesCount++;
		}
	}
	finder.Close();

	return (_images.size() != 0);
}

void ResourceTileFolder::CreateImageList(CDC* pDC)
{
	if (_bHasImageList)
		return;

	AfxGetMainWnd()->BeginWaitCursor();

	_imageList.Create(_iIconSize, _iIconSize, ILC_COLOR32, 0, 4);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	IDBitmapMapType::iterator itBmp = _BitmapTiles.begin();

	for (IDImageMapType::iterator it = _images.begin(); it != _images.end(); ++it)
	{
		CBitmap* bmpTile = new CBitmap;
		CBitmap* bmpOld;

		bmpTile->CreateCompatibleBitmap(pDC, _iIconSize, _iIconSize);
		bmpOld = dcMem.SelectObject(bmpTile);

		dcMem.FillSolidRect(0, 0, _iIconSize, _iIconSize, RGB(255, 255, 255));

		int iW = it->second->GetWidth();
		int iH = it->second->GetHeight();

		if (__max(iW, iH) > _iIconSize)
		{
			if (iW > iH)
			{
				float fR = 1.0f * _iIconSize / iW;
				it->second->Draw(dcMem.GetSafeHdc(), 0, 0, _iIconSize, int(iH * fR), 0, true);
			}
			else
			{
				float fR = 1.0f * _iIconSize / iH;
				it->second->Draw(dcMem.GetSafeHdc(), int((_iIconSize - fR * iW)/2), 0, int(fR * iW), _iIconSize, 0, true);
			}
		}
		else
		{
			it->second->Draw(dcMem.GetSafeHdc(), (_iIconSize - iW)/2, 0, -1, -1, 0, true);
		}

		dcMem.SelectObject(bmpOld);

		_imageList.Add(bmpTile, RGB(0, 0, 0));

		itBmp->second = bmpTile;
		itBmp++;
	}

	_bHasImageList = true;

	AfxGetMainWnd()->EndWaitCursor();
}

void ResourceTileFolder::Draw(CDC* pDC, const CRect& curTile, const Cactus::String& strID)
{
	CreateImageList(pDC);

	if (_images.find(strID) == _images.end())
	{
		Log_Error("ResourceTileFolder::Draw, can not find Resource for " << strID);
		return;
	}

	CxImage* pImage = _images[strID];

	if (ToolManager::getSingleton().GetMap()->GetType() == eRectangle)
	{
		//如果是井形地图，按照图片左上角对齐

		pImage->Draw(pDC->GetSafeHdc(), curTile.left, curTile.top);
	}
	else
	{
		//如果是菱形地图，按照图片中间顶端对齐

		int startX = curTile.CenterPoint().x - pImage->GetWidth()/2;
		int startY = curTile.top;

		pImage->Draw(pDC->GetSafeHdc(), startX, startY);
	}
}

CRect ResourceTileFolder::GetResItemBoundingRect(const CRect& curTile, const Cactus::String& strID)
{
	if (_images.find(strID) == _images.end())
	{
		Log_Error("ResourceTileFolder::GetResItemBoundingRect, can not find Resource for " << strID);
		return CRect(0, 0, 1, 1);
	}

	CxImage* pImage = _images[strID];

	CRect rcDest;
	if (ToolManager::getSingleton().GetMap()->GetType() == eRectangle)
	{
		//如果是井形地图，按照图片左上角对齐

		rcDest = curTile;
		rcDest.right	= rcDest.left + pImage->GetWidth();
		rcDest.bottom	= rcDest.top + pImage->GetHeight();
	}
	else
	{
		//如果是菱形地图，按照图片水平中间、垂直顶端对齐

		int startX = curTile.CenterPoint().x - pImage->GetWidth()/2;
		int startY = curTile.top;

		rcDest = CRect(startX, startY, startX + pImage->GetWidth(), startY + pImage->GetHeight());
	}

	return rcDest;
}
