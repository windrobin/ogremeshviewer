#include "StdAfx.h"
#include "MapLayer.h"
#include "ResourceManager.h"
#include "Map.h"
#include "ResourceGameObject.h"

using namespace Cactus;
using namespace PropertySys;

//---------------------------------------------------------------------------
bool STile_less(const STile* p1, const STile* p2)
{
#if 0
	Resource* pRes = ResourceManager::getSingleton().GetResource(_strResGroup);
	if (pRes)
	{
		if(pRes->GetResourceType() == eResTypeArt)
		{
			return _posX + _posY < right._posX + right._posY;
		}
		else
		{
			//ResourceGameObjectGroup* pGameObjectGroup = (ResourceGameObjectGroup*)pRes;
			//ResourceGameObject* pObject = pGameObjectGroup->GetGameObject(_strResItemID);
			//if (pObject)
			//{
			//	return pObject->operator < (right);
			//}
		}
	}

	return _posX + _posY < right._posX + right._posY;
#endif

	return p1->_posX + p1->_posY < p2->_posX + p2->_posY;
}



//---------------------------------------------------------------------------
MapLayer::MapLayer()
: _bVisible(true)
{
}

MapLayer::~MapLayer()
{
}

void MapLayer::Init(const Cactus::String& strName, Map* pParent)
{
	_pParentMap	= pParent;
	_strName	= strName;
}

void MapLayer::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(bool, Enable, MapLayer, Map, "是否激活，不激活将不会导出到游戏地图.", BaseProperty::eDefault, _bEnable);
	pProp = M_RegisterPropertySimple(bool, Visible, MapLayer, Map, "是否显示.", BaseProperty::eDefault, _bVisible);
}

void MapLayer::OnPropertyChanged(const std::string& propName)
{
	if (propName == "DrawGrid")
	{
		CView* pView = ((CFrameWnd*)AfxGetMainWnd())->GetActiveView(); 
		pView->Invalidate(TRUE);
	}
}

void MapLayer::Draw(CDC* pDC, const IntVectorType& regions)
{
	if (!_bVisible || !_pParentMap)
		return;

	// 只绘制区域内容
	Cactus::list<STile*>::type	tilesList;
	for (size_t t = 0; t < regions.size(); ++t)
	{
		TileVectorType& tiles = _GroupTiles[regions[t]];

		for (size_t k = 0; k < tiles.size(); ++k)
		{
			STile& tile = tiles[k];
			tilesList.push_back(&tile);
		}
	}
	if (tilesList.size() > 0)
		tilesList.sort(STile_less);

	for (Cactus::list<STile*>::type::iterator it = tilesList.begin(); it != tilesList.end(); ++it)
	{
		STile& tile = **it;

		Resource* pRes = ResourceManager::getSingleton().GetResource(tile._strResGroup);
		if (pRes)
		{
			CRect rc = _pParentMap->GetPixelCoordRect(CPoint(tile._posX, tile._posY));
			pRes->Draw(pDC, rc, tile._strResItemID);
		}
	}

	/*
	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			STile& tile = tiles[t];

			Resource* pRes = ResourceManager::getSingleton().GetResource(tile._strResGroup);
			if (pRes)
			{
				CRect rc = _pParentMap->GetPixelCoordRect(CPoint(tile._posX, tile._posY));
				pRes->Draw(pDC, rc, tile._strResItemID);
			}
		}
	}
	*/
}

bool MapLayer::ToolHitTest(CPoint pt, int& gridX, int& gridY, CRect& rc)
{
	CPoint ptGrid;
	bool bInRegion = _pParentMap->GetGridCoord(pt, ptGrid);
	if (bInRegion)
	{
		gridX = ptGrid.x;
		gridY = ptGrid.y;

		rc = _pParentMap->GetPixelCoordRect(ptGrid);
	}

	return bInRegion;
}

bool MapLayer::AddOrModifyTile(int gridX, int gridY, const Cactus::String& resGroup, const Cactus::String& strItemID)
{
	int regionID = _pParentMap->GetRegionID(CPoint(gridX, gridY));
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile& tile = tiles[t];

		if(tile._posX == gridX && tile._posY == gridY)
		{
			if(tile._strResItemID == strItemID && resGroup == tile._strResGroup)
			{
				return false;
			}
			else
			{
				tile._strResGroup	= resGroup;
				tile._strResItemID	= strItemID;
				return true;
			}
		}
	}

	if (!bFound)
	{
		STile newTile;
		newTile._posX			= gridX;
		newTile._posY			= gridY;
		newTile._strResItemID	= strItemID;
		newTile._strResGroup	= resGroup;
		newTile._regionID		= regionID;

		tiles.push_back(newTile);
	}

	return true;
}

bool MapLayer::ClearTile(int gridX, int gridY)
{
	int regionID = _pParentMap->GetRegionID(CPoint(gridX, gridY));
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

	if (tiles.size() == 0)
		return false;

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile& tile = tiles[t];
		if(tile._posX == gridX && tile._posY == gridY)
		{
			tiles.erase(tiles.begin() + t);
			bFound = true;
		}
	}

	return bFound;
}

bool MapLayer::GetTileInfo(int gridX, int gridY, STile& tile)
{
	int regionID = _pParentMap->GetRegionID(CPoint(gridX, gridY));
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

	if (tiles.size() == 0)
		return false;

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		if(tile._posX == gridX && tile._posY == gridY)
		{
			tile = tiles[t];
			bFound = true;
		}
	}

	return bFound;
}
