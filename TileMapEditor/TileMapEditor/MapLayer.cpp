#include "StdAfx.h"
#include "MapLayer.h"
#include "ResourceManager.h"
#include "Map.h"

using namespace Cactus;
using namespace PropertySys;

MapLayer::MapLayer()
: _bVisible(true)
, _bDrawGrid(true)
, _pParentMap(0)
{
}

MapLayer::~MapLayer()
{
}


void MapLayer::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, MapLayer, Map, "Layer名.", BaseProperty::eDefault, _strName);

	pProp = M_RegisterPropertySimple(int, WidthInTiles, MapLayer, Map, "宽度，以Map的Tile单位计算.", BaseProperty::eDefault, _iWidthInTiles);
	pProp = M_RegisterPropertySimple(int, HeightInTiles, MapLayer, Map, "高度，以Map的Tile单位计算.", BaseProperty::eDefault, _iHeightInTiles);
	pProp = M_RegisterPropertySimple(bool, Enable, MapLayer, Map, "是否激活，不激活将不会导出到游戏地图.", BaseProperty::eDefault, _bEnable);
	
	pProp = M_RegisterPropertySimple(bool, DrawGrid, MapLayer, Map, "是否绘制网格.", BaseProperty::eDefault, _bDrawGrid);
	//pProp = M_RegisterPropertySimple(int, GridColor, MapLayer, Map, "网格颜色.", BaseProperty::eDefault, _colGridColor);
	//pProp->SetValueSpecify(eValueColor, "");
}

void MapLayer::OnPropertyChanged(const std::string& propName)
{
	if (propName == "DrawGrid")
	{
		CView* pView = ((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView(); 
		pView->Invalidate(TRUE);
	}
}

void MapLayer::Draw(CDC* pDC)
{
	if (!_bVisible || !_pParentMap)
		return;

	int iTileW = _iWidthInTiles * _pParentMap->_iUnitTileWidth;
	int iTileH = _iHeightInTiles * _pParentMap->_iUnitTileHeight;

	for (TileGroupMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		Resource* pRes = ResourceManager::getSingleton().GetResource(it->first);
		if (pRes)
		{
			for (size_t t = 0; t < it->second.size(); ++t)
			{
				STile& tile = it->second[t];
				pRes->Draw(pDC, tile._posX * iTileW, tile._posY * iTileH, tile._strID);
			}
		}
	}

	if (_bDrawGrid)
	{
		CPen pen(PS_SOLID, 1, RGB(0, 128, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);

		int iMapW = _pParentMap->GetPixelWidth();
		int iMapH = _pParentMap->GetPixelHeight();

		//画横线
		for (int i = 0; i <= _pParentMap->_iHeightInTiles / _iHeightInTiles; i++)
		{
			pDC->MoveTo(0, i * _iHeightInTiles * _pParentMap->_iUnitTileHeight);
			pDC->LineTo(iMapW, i * _iHeightInTiles * _pParentMap->_iUnitTileHeight);
		}

		//画竖线
		for (int i = 0; i <= _pParentMap->_iWidthInTiles / _iWidthInTiles; i++)
		{
			pDC->MoveTo(i * _iWidthInTiles * _pParentMap->_iUnitTileWidth, 0);
			pDC->LineTo(i * _iWidthInTiles * _pParentMap->_iUnitTileWidth, iMapH);
		}

		pDC->SelectObject(pOldPen);
	}
}

bool MapLayer::ToolHitTest(CPoint pt, int& gridX, int& gridY, CRect& rc)
{
	int iMapW = _pParentMap->GetPixelWidth();
	int iMapH = _pParentMap->GetPixelHeight();

	if (pt.x >= iMapW || pt.y >= iMapH || pt.x <= 0 || pt.y <= 0)
	{
		return false;
	}

	int iTileW = _iWidthInTiles * _pParentMap->_iUnitTileWidth;
	int iTileH = _iHeightInTiles * _pParentMap->_iUnitTileHeight;

	gridX	= pt.x / iTileW;
	gridY	= pt.y / iTileH;

	rc = CRect(CPoint(gridX * iTileW, gridY * iTileH), CSize(iTileW, iTileH));
	return true;
}

bool MapLayer::ModifyTile(int gridX, int gridY, const Cactus::String& resKey, const Cactus::String& strID)
{
	bool bFound = false;

	for (TileGroupMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		for (size_t t = 0; t < it->second.size(); ++t)
		{
			STile& tile = it->second[t];
			if(tile._posX == gridX && tile._posY == gridY)
			{
				if(tile._strID == strID && resKey == it->first)
				{
					return false;
				}
				else if (resKey == it->first)
				{
					tile._strID = strID;
					return true;
				}
				else
				{
					it->second.erase(it->second.begin() + t);
				}
			}
		}
	}

	if (!bFound)
	{
		STile newTile;
		newTile._posX = gridX;
		newTile._posY = gridY;
		newTile._strID = strID;

		_GroupTiles[resKey].push_back(newTile);
	}

	return true;
}

bool MapLayer::ClearTile(int gridX, int gridY)
{
	bool bFound = false;

	for (TileGroupMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		for (size_t t = 0; t < it->second.size(); ++t)
		{
			STile& tile = it->second[t];
			if(tile._posX == gridX && tile._posY == gridY)
			{
				it->second.erase(it->second.begin() + t);
				bFound = true;
			}
		}
	}

	return bFound;
}

bool MapLayer::GetTileInfo(int gridX, int gridY, STile& tile, Cactus::String& resKey)
{
	bool bFound = false;

	for (TileGroupMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		for (size_t t = 0; t < it->second.size(); ++t)
		{
			tile = it->second[t];
			if(tile._posX == gridX && tile._posY == gridY)
			{
				resKey = it->first;
				bFound = true;
			}
		}
	}

	return bFound;
}
