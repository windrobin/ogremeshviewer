#include "StdAfx.h"
#include "ResourceGameObject.h"
#include "ResourceManager.h"
#include "ResourceTile.h"

using namespace Cactus;
using namespace PropertySys;

ResourceGameObject::ResourceGameObject()
{
}

ResourceGameObject::~ResourceGameObject()
{
}

bool ResourceGameObject::Load(const Cactus::String& strTile)
{
	//判断它依赖的图像资源十分有效
	return ResourceManager::getSingleton().IsResourceTileIDValid(strTile, _ArtResID);
}

void ResourceGameObject::Save(XMLOutStream& xmlOut)
{
	xmlOut.NodeBegin("item");
		xmlOut.AddAttribute("name", _strName);
		xmlOut.AddAttribute("iconid", _ArtResID);
		xmlOut.AddAttribute("xOffset", _ptOffset.x);
		xmlOut.AddAttribute("yOffset", _ptOffset.y);
		xmlOut.AddAttribute("xBarycentric", _xBaryCentric);
		xmlOut.AddAttribute("yBarycentric", _yBaryCentric);

		for (ObstacleListType::iterator it = _obstacle.begin(); it != _obstacle.end(); ++it)
		{
			xmlOut.NodeBegin("obstacle");
				xmlOut.AddAttribute("x", it->x);
				xmlOut.AddAttribute("y", it->y);
			xmlOut.NodeEnd("obstacle");
		}

	xmlOut.NodeEnd("item");
}

//--------------------------------------------------------------------------------------------------------
ResourceGameObjectGroup::ResourceGameObjectGroup()
{
	_eResType	= eResTypeGameObject;
	_szUnitTile	= CPoint(64, 64);
	_iMapType	= 0;
}

ResourceGameObjectGroup::ResourceGameObjectGroup(const Cactus::String& strName, const Cactus::String& strArtGroup, CPoint szTile)
{
	_eResType	= eResTypeGameObject;

	_strName		= strName;
	_strArtResKey	= strArtGroup;
	_szUnitTile		= szTile;
}


ResourceGameObjectGroup::~ResourceGameObjectGroup()
{
	for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
	{
		delete *it;
	}
	_ResGameObjects.clear();
}

void ResourceGameObjectGroup::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, ArtResKey, ResourceGameObjectGroup, Resource, "图标的来源.", BaseProperty::eReadOnly, _strArtResKey);
	pProp = M_RegisterPropertySimple(CPoint, UnitTileSize, ResourceGameObjectGroup, Resource, "单元格的大小.", BaseProperty::eReadOnly, _szUnitTile);
}

void ResourceGameObjectGroup::OnPropertyChanged(const std::string& propName)
{
}

void ResourceGameObjectGroup::CreateImageList(CDC* pDC)
{
	if (_bHasImageList)
		return;

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		pResTile->CreateImageList(pDC);
		CImageList* pImageList = pResTile->GetImageList();

		//从ResourceTile资源复制一份
		_imageList.Create(pImageList);

		Cactus::StringVector* pCaps = pResTile->GetCaptions();

		size_t iIndex = 0;
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if ((*it)->_ArtResID != (*pCaps)[iIndex])
			{
				for (size_t t = 0; t < (*pCaps).size(); ++t)	//找到引用的图片索引
				{
					if( (*it)->_ArtResID == (*pCaps)[t] )
					{
						_imageList.Copy(iIndex, t, ILCF_SWAP);	//交换位置
						break;
					}
				}
			}

			iIndex++;

			_captions.push_back( (*it)->_strName );
		}

		int iCount = _imageList.GetImageCount();

		for (int i = iIndex; i < iCount; ++i)
		{
			_imageList.Remove(i);
		}
	}

	_bHasImageList = true;
}

void ResourceGameObjectGroup::Draw(CDC* pDC, const CRect& curTile, const Cactus::String& strItemID)
{
	CreateImageList(pDC);

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if( (*it)->_strName == strItemID )
			{
				pResTile->Draw(pDC, curTile, (*it)->_ArtResID);
				break;
			}
		}
	}
}

CRect ResourceGameObjectGroup::GetResItemBoundingRect(const CRect& curTile, const Cactus::String& strItemID)
{
	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if( (*it)->_strName == strItemID )
			{
				return pResTile->GetResItemBoundingRect(curTile, (*it)->_ArtResID);
			}
		}
	}

	return Resource::GetResItemBoundingRect(curTile, strItemID);
}

ResourceGameObject* ResourceGameObjectGroup::GetGameObject(const Cactus::String& strItemID)
{
	for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
	{
		if( (*it)->_strName == strItemID )
		{
			return *it;
		}
	}

	return 0;
}

void ResourceGameObjectGroup::Save(XMLOutStream& xmlOut)
{
	xmlOut.NodeBegin("group");
		xmlOut.AddAttribute("name", _strName);
		xmlOut.AddAttribute("iconres", _strArtResKey);
		xmlOut.AddAttribute("mapType", _iMapType);
		xmlOut.AddAttribute("unitTileW", _szUnitTile.x);
		xmlOut.AddAttribute("unitTileH", _szUnitTile.y);

	for(ResGameObjectListType::iterator itItem = _ResGameObjects.begin();
		itItem != _ResGameObjects.end();
		++itItem)
	{
		ResourceGameObject* pGO = *itItem;

		pGO->Save(xmlOut);
	}

	xmlOut.NodeEnd("group");
}
