#include "StdAfx.h"
#include "MapLayer.h"
#include "ResourceManager.h"
#include "Map.h"
#include "ResourceGameObject.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

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
	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			delete tiles[t];
		}
	}
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
			STile* pTile = tiles[k];
			tilesList.push_back(pTile);
		}
	}
	if (tilesList.size() > 0)
		tilesList.sort(STile_less);

	CPen pen(PS_SOLID, 1, RGB(231, 165, 25));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);

	for (Cactus::list<STile*>::type::iterator it = tilesList.begin(); it != tilesList.end(); ++it)
	{
		STile* pTile = *it;

		Resource* pRes = ResourceManager::getSingleton().GetResource(pTile->_strResGroup);
		if (pRes)
		{
			CRect rcPixel = _pParentMap->GetPixelCoordRect(CPoint(pTile->_posX, pTile->_posY));
			pRes->Draw(pDC, rcPixel, pTile->_strResItemID);

			if (pTile->_bSelected)
			{
				CRect rcDest = pRes->GetResItemBoundingRect(rcPixel, pTile->_strResItemID);
				pDC->Rectangle(rcDest);
			}
		}
	}
	pDC->SelectObject(pOldPen);
}

bool MapLayer::ToolHitTest(CPoint ptPixel, int& gridX, int& gridY, CRect& rcPixel)
{
	CPoint ptGrid;
	bool bInRegion = _pParentMap->GetGridCoord(ptPixel, ptGrid);
	if (bInRegion)
	{
		gridX = ptGrid.x;
		gridY = ptGrid.y;

		rcPixel = _pParentMap->GetPixelCoordRect(ptGrid);
	}

	return bInRegion;
}

STile* MapLayer::TileHitTest(CPoint ptPixel, CPoint& ptGrid)
{
	bool bInRegion = _pParentMap->GetGridCoord(ptPixel, ptGrid);
	if (!bInRegion)
		return 0;

	int regionID = _pParentMap->GetRegionID(ptGrid);
	if( regionID == -1 )
		return 0;

	TileVectorType& tiles = _GroupTiles[regionID];

	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile* pTile = tiles[t];

		if(pTile->_posX == ptGrid.x && pTile->_posY == ptGrid.y)
		{
			return pTile;
		}
		else
		{
			Resource* pRes = ResourceManager::getSingleton().GetResource(pTile->_strResGroup);

			CRect rcPixelGrid = _pParentMap->GetPixelCoordRect(CPoint(pTile->_posX, pTile->_posY));
			CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, pTile->_strResItemID);

			if (rcDest.PtInRect(ptPixel))
			{
				return pTile;
			}
		}
	}

	return 0;
}


void MapLayer::UpdateTileInfoInMapLayer(STile* pTile, ETileOp e/* = eTileOpAdd*/)
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		//更新地层列表
		LayerView* pView = pMainFrame->GetMapLayerView(); 
		if (pView)
		{
			if (e == eTileOpAdd)
				pView->AddTileInfo(pTile);
			else if (e == eTileOpRemove)
				pView->RemoveTileInfo(pTile);
			else
				pView->UpdateTileInfo(pTile);
		}
	}
}

void MapLayer::UpdateTileVisual(STile* pTile, bool bEnsureVisible/* = false*/)
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		//更新绘制
		CTileMapEditorView* pMapView = (CTileMapEditorView*)pMainFrame->GetActiveView();
		CRect rcPixelGrid = _pParentMap->GetPixelCoordRect(CPoint(pTile->_posX, pTile->_posY));

		Resource* pRes = ResourceManager::getSingleton().GetResource(pTile->_strResGroup);
		CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, pTile->_strResItemID);
		pMapView->LogicInvalidate(rcDest);

		if (bEnsureVisible)
		{
			// TODO
		}
	}
}


bool MapLayer::MoveTile(STile* pTile, CPoint ptNewGrid)
{
	int regionID = _pParentMap->GetRegionID(ptNewGrid);
	if( regionID == -1 )
		return false;

	if (regionID != pTile->_regionID)
	{
		TileVectorType& tiles = _GroupTiles[pTile->_regionID];

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			if(pTile == tiles[t])
			{
				tiles.erase(tiles.begin() + t);
				break;
			}
		}

		_GroupTiles[regionID].push_back(pTile);
	}

	STile tileOld = *pTile;

	pTile->_regionID	= regionID;
	pTile->_posX		= ptNewGrid.x;
	pTile->_posY		= ptNewGrid.y;

	UpdateTileInfoInMapLayer(pTile, eTileOpUpdate);

	UpdateTileVisual(&tileOld);
	UpdateTileVisual(pTile);

	return true;
}



bool MapLayer::AddOrUpdateTile(int gridX, int gridY, const Cactus::String& resGroup, const Cactus::String& strItemID)
{
	int regionID = _pParentMap->GetRegionID(CPoint(gridX, gridY));
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile* pTile = tiles[t];

		if(pTile->_posX == gridX && pTile->_posY == gridY)
		{
			if(pTile->_strResItemID == strItemID && resGroup == pTile->_strResGroup)
			{
				return false;
			}
			else
			{
				pTile->_strResGroup		= resGroup;
				pTile->_strResItemID	= strItemID;

				UpdateTileInfoInMapLayer(pTile, eTileOpUpdate);

				UpdateTileVisual(pTile);

				return true;
			}
		}
	}

	if (!bFound)
	{
		STile* pTile = new STile;
		pTile->_posX			= gridX;
		pTile->_posY			= gridY;
		pTile->_strResItemID	= strItemID;
		pTile->_strResGroup		= resGroup;
		pTile->_regionID		= regionID;

		tiles.push_back(pTile);

		UpdateTileInfoInMapLayer(pTile, eTileOpAdd);

		UpdateTileVisual(pTile);
	}

	return true;
}

bool MapLayer::RemoveTile(int gridX, int gridY)
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
		STile* pTile = tiles[t];
		if(pTile->_posX == gridX && pTile->_posY == gridY)
		{
			tiles.erase(tiles.begin() + t);

			UpdateTileInfoInMapLayer(pTile, eTileOpRemove);

			delete pTile;
			bFound = true;
		}
	}

	return bFound;
}

STile* MapLayer::GetTileInfo(int gridX, int gridY)
{
	int regionID = _pParentMap->GetRegionID(CPoint(gridX, gridY));
	if( regionID == -1 )
		return 0;

	TileVectorType& tiles = _GroupTiles[regionID];

	if (tiles.size() == 0)
		return 0;

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		if(tiles[t]->_posX == gridX && tiles[t]->_posY == gridY)
		{
			return tiles[t];
		}
	}

	return 0;
}


void MapLayer::FillMapLayerList()
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if (!pMainFrame)
		return;

	LayerView* pView = pMainFrame->GetMapLayerView(); 
	if (!pView)
		return;

	for (RegionTileMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		TileVectorType& tiles = it->second;

		for (size_t t = 0; t < tiles.size(); ++t)
		{
			pView->AddTileInfo(tiles[t]);
		}
	}

}
