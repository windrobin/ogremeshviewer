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

void ResourceGameObject::CalculateBaryCentric()
{
	_xBaryCentric = 0;
	_yBaryCentric = 0;

	if (_obstacles.size() == 0)
		return;

	for (ObstacleListType::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it)
	{
		_xBaryCentric += it->x;
		_yBaryCentric += it->y;
	}

	_xBaryCentric /= _obstacles.size();
	_yBaryCentric /= _obstacles.size();
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

		for (ObstacleListType::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it)
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
	_eGridType	= eRectangle;
}

ResourceGameObjectGroup::ResourceGameObjectGroup(const Cactus::String& strName
												 , const Cactus::String& strArtGroup
												 , CPoint szTile
												 , EGridType e)
{
	_eResType		= eResTypeGameObject;

	_strName		= strName;
	_strArtResKey	= strArtGroup;
	_szUnitTile		= szTile;
	_eGridType		= e;
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

void ResourceGameObjectGroup::CreateImageList(CDC* pDC, bool bForceRecreate/* = false*/)
{
	if (_bHasImageList)
	{
		if (bForceRecreate)
		{
			_captions.clear();
			_imageList.DeleteImageList();
		}
		else
		{
			return;
		}
	}

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		pResTile->CreateImageList(pDC);
		CImageList* pImageList = pResTile->GetImageList();

		//从ResourceTile资源复制一份
		_imageList.Create(pImageList);

		Cactus::StringVector Caps = *pResTile->GetCaptions();

		size_t iIndex = 0;
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if ((*it)->_ArtResID != Caps[iIndex])
			{
				for (size_t t = 0; t < Caps.size(); ++t)	//找到引用的图片索引
				{
					if( (*it)->_ArtResID == Caps[t] )
					{
						_imageList.Copy(iIndex, t, ILCF_SWAP);	//交换位置
						
						String strTmp	= Caps[iIndex];
						Caps[iIndex]	= Caps[t];
						Caps[t]			= strTmp;

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


void ResourceGameObjectGroup::Draw(CDC* pDC, GridObject* pGridObject, const CPoint& ptGrid, const Cactus::String& strID)
{
	CreateImageList(pDC);

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if( (*it)->_strName == strID )
			{
				CRect rcCurTile = pGridObject->GetPixelCoordRect(ptGrid);
				CPoint ptTopLeft = rcCurTile.CenterPoint() + (*it)->_ptOffset;

				pResTile->Draw(pDC, ptTopLeft, (*it)->_ArtResID);

				//绘制中心
				pGridObject->DrawGrid(pDC, ptGrid, RGB(0, 0, 255), 0);

				// 绘制阻挡信息
				for (ObstacleListType::iterator itObstacle = (*it)->_obstacles.begin(); itObstacle != (*it)->_obstacles.end(); ++itObstacle)
				{
					CPoint pt = ptGrid;
					pt.Offset(*itObstacle);
					pGridObject->DrawGrid(pDC, pt, RGB(255, 0, 0), 2);
				}

				break;
			}
		}
	}
}

void ResourceGameObjectGroup::Draw(CDC* pDC, const CPoint& ptTopLeft, const Cactus::String& strID)
{
	//不支持，对游戏对象无意义
}

CRect ResourceGameObjectGroup::GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strItemID)
{
	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if( (*it)->_strName == strItemID )
			{
				CRect rcTile = pResTile->GetResItemBoundingRect(curTile, eGridNone, (*it)->_ArtResID);
				rcTile.OffsetRect((*it)->_ptOffset);
				rcTile.OffsetRect(_szUnitTile.x/2, _szUnitTile.y/2);

				//有阻挡的扩大范围
				if ((*it)->_obstacles.size())
				{
					rcTile.InflateRect(_szUnitTile.x/2, _szUnitTile.y/2);
				}

				return rcTile;
			}
		}
	}

	return Resource::GetResItemBoundingRect(curTile, eGrid, strItemID);
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
		xmlOut.AddAttribute("mapType", _eGridType);
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

bool ResourceGameObjectGroup::UpdateGameObject(ResourceGameObject* pGO)
{
	for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
	{
		if( *it != pGO && (*it)->_strName == pGO->_strName)
		{
			return false;
		}
	}

	pGO->CalculateBaryCentric();

	return true;
}

bool ResourceGameObjectGroup::AddGameObject(ResourceGameObject* pGO)
{
	for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
	{
		if( (*it)->_strName == pGO->_strName)
		{
			return false;
		}
	}

	pGO->CalculateBaryCentric();

	_ResGameObjects.push_back(pGO);

	return true;
}

bool ResourceGameObjectGroup::RemoveGameObject(const Cactus::String& strName)
{
	for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
	{
		if( (*it)->_strName == strName)
		{
			delete *it;
			_ResGameObjects.erase(it);
			return true;
		}
	}

	return false;
}
