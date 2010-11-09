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
			return _ptGrid.x + _ptGrid.y < right._ptGrid.x + right._ptGrid.y;
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

	return _ptGrid.x + _ptGrid.y < right._ptGrid.x + right._ptGrid.y;
#endif

	return p1->_ptGrid.x + p1->_ptGrid.y < p2->_ptGrid.x + p2->_ptGrid.y;
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

		Resource* pRes = ResourceManager::getSingleton().GetResourceArtGroup(pTile->_strResGroup);
		if (!pRes)
			pRes = ResourceManager::getSingleton().GetResourceGameObjectGroup(pTile->_strResGroup);

		if (pRes)
		{
			CRect rcPixel = _pParentMap->GetPixelCoordRect(CPoint(pTile->_ptGrid.x, pTile->_ptGrid.y));
			pRes->Draw(pDC, rcPixel, _pParentMap->GetType(), pTile->_strResItemID);

			if (pTile->_bSelected)
			{
				CRect rcDest = pRes->GetResItemBoundingRect(rcPixel, _pParentMap->GetType(), pTile->_strResItemID);
				pDC->Rectangle(rcDest);
			}
		}
	}
	pDC->SelectObject(pOldPen);
}

bool MapLayer::ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel)
{
	bool bInRegion = _pParentMap->GetGridCoord(ptPixel, ptGrid);
	if (bInRegion)
	{
		rcPixel = _pParentMap->GetPixelCoordRect(ptGrid);
	}
	else
	{
		ptGrid.x	= -1;
		ptGrid.y	= -1;
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

	//先判断点击点是否有Tile
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile* pTile = tiles[t];

		if(pTile->_ptGrid.x == ptGrid.x && pTile->_ptGrid.y == ptGrid.y)
		{
			return pTile;
		}
	}
	
	//再判断点击点是否在某个Tile的包围盒范围内；目前只搜索当前Region，以后可以增加相邻Region
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile* pTile = tiles[t];

		Resource* pRes = ResourceManager::getSingleton().GetResourceArtGroup(pTile->_strResGroup);
		if (!pRes)
			pRes = ResourceManager::getSingleton().GetResourceGameObjectGroup(pTile->_strResGroup);

		CRect rcPixelGrid = _pParentMap->GetPixelCoordRect(CPoint(pTile->_ptGrid.x, pTile->_ptGrid.y));
		CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, _pParentMap->GetType(), pTile->_strResItemID);

		if (rcDest.PtInRect(ptPixel))
		{
			return pTile;
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
		CRect rcPixelGrid = _pParentMap->GetPixelCoordRect(CPoint(pTile->_ptGrid.x, pTile->_ptGrid.y));

		Resource* pRes = ResourceManager::getSingleton().GetResourceArtGroup(pTile->_strResGroup);
		if (!pRes)
			pRes = ResourceManager::getSingleton().GetResourceGameObjectGroup(pTile->_strResGroup);

		CRect rcDest = pRes->GetResItemBoundingRect(rcPixelGrid, _pParentMap->GetType(), pTile->_strResItemID);
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
	pTile->_ptGrid.x		= ptNewGrid.x;
	pTile->_ptGrid.y		= ptNewGrid.y;

	UpdateTileInfoInMapLayer(pTile, eTileOpUpdate);

	UpdateTileVisual(&tileOld);
	UpdateTileVisual(pTile);

	return true;
}



bool MapLayer::AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID)
{
	int regionID = _pParentMap->GetRegionID(ptGrid);
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile* pTile = tiles[t];

		if(pTile->_ptGrid == ptGrid)
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
		pTile->_ptGrid			= ptGrid;
		pTile->_strResItemID	= strItemID;
		pTile->_strResGroup		= resGroup;
		pTile->_regionID		= regionID;

		tiles.push_back(pTile);

		UpdateTileInfoInMapLayer(pTile, eTileOpAdd);

		UpdateTileVisual(pTile);
	}

	return true;
}

bool MapLayer::RemoveTile(CPoint ptGrid)
{
	int regionID = _pParentMap->GetRegionID(ptGrid);
	if( regionID == -1 )
		return false;

	TileVectorType& tiles = _GroupTiles[regionID];

	if (tiles.size() == 0)
		return false;

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		STile* pTile = tiles[t];
		if(pTile->_ptGrid == ptGrid)
		{
			tiles.erase(tiles.begin() + t);

			UpdateTileInfoInMapLayer(pTile, eTileOpRemove);

			delete pTile;
			bFound = true;
		}
	}

	return bFound;
}

STile* MapLayer::GetTileInfo(CPoint ptGrid)
{
	int regionID = _pParentMap->GetRegionID(ptGrid);
	if( regionID == -1 )
		return 0;

	TileVectorType& tiles = _GroupTiles[regionID];

	if (tiles.size() == 0)
		return 0;

	bool bFound = false;
	for (size_t t = 0; t < tiles.size(); ++t)
	{
		if(tiles[t]->_ptGrid == ptGrid)
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
