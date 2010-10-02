#include "StdAfx.h"
#include "MapLayer.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

MapLayer::MapLayer()
: _bVisible(true)
, _bDrawGrid(true)
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

	pProp = M_RegisterPropertySimple(int, Width, MapLayer, Map, "宽度，不可超过所属Map的宽度.", BaseProperty::eDefault, _iWidth);
	pProp = M_RegisterPropertySimple(int, Height, MapLayer, Map, "高度，不可超过所属Map的高度.", BaseProperty::eDefault, _iHeight);
	pProp = M_RegisterPropertySimple(int, TileWidth, MapLayer, Map, "绘制模式.", BaseProperty::eDefault, _iTileWidth);
	pProp = M_RegisterPropertySimple(int, TileHeight, MapLayer, Map, "绘制模式.", BaseProperty::eDefault, _iTileHeight);
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
	if (!_bVisible)
		return;

	for (TileGroupMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		Resource* pRes = ResourceManager::getSingleton().GetResource(it->first);
		if (pRes)
		{
			for (size_t t = 0; t < it->second.size(); ++t)
			{
				STile& tile = it->second[t];
				pRes->Draw(pDC, tile._posX * _iTileWidth, tile._posY * _iTileHeight, tile._strID);
			}
		}
	}

	if (_bDrawGrid)
	{
		CPen pen(PS_SOLID, 1, RGB(0, 128, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);

		int iGridWidth	= _iWidth / _iTileWidth;
		int iGridHeight	= _iHeight / _iTileHeight;
		for (int i = 0; i <= iGridWidth; i++)
		{
			pDC->MoveTo(0, i * _iTileWidth);
			pDC->LineTo(_iHeight, i * _iTileHeight);
		}
		for (int i = 0; i <= iGridHeight; i++)
		{
			pDC->MoveTo(i * _iTileWidth, 0);
			pDC->LineTo(i * _iTileHeight, _iWidth);
		}

		pDC->SelectObject(pOldPen);
	}
}

bool MapLayer::ToolHitTest(CPoint pt, int& gridX, int& gridY, CRect& rc)
{
	if (pt.x >= _iWidth || pt.y >= _iHeight || pt.x <= 0 || pt.y <= 0)
	{
		return false;
	}

	gridX	= pt.x / _iTileWidth;
	gridY	= pt.y / _iTileHeight;

	rc = CRect(CPoint(gridX * _iTileWidth, gridY * _iTileHeight), CSize(_iTileWidth, _iTileHeight));
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
