#include "StdAfx.h"
#include "Map.h"
#include "MapBackground.h"
#include "MapLayer.h"

#include "MainFrm.h"
#include "TileMapEditorView.h"

using namespace Cactus;
using namespace PropertySys;

//---------------------------------------------------------------------------------------------------------

class Map_xmlHandler : public Cactus::XMLHandler
{
public:
	Map_xmlHandler(Map& map) : _map(map){}
	virtual ~Map_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if ( currentElementMatch("tilemap/") )
		{
			//<tilemap version="1" name="test001" width="1024" height="1024" tilew="64" tileh="64" footnotes="这是一个测试地图文件。">
			_map._iVersion				= attributes.getValueAsInteger("version");
			_map._strName				= attributes.getValueAsString("name");
			_map._iWidthInTiles			= attributes.getValueAsInteger("width");
			_map._iHeightInTiles		= attributes.getValueAsInteger("height");
			_map._iUnitTileWidth		= attributes.getValueAsInteger("wUnitTile");
			_map._iUnitTileHeight		= attributes.getValueAsInteger("hUnitTile");
			_map._strFootnotes			= attributes.getValueAsString("footnotes");
			_map._colBKColor			= attributes.getValueAsInteger("backgroundcolor");
			_map._strCurLayerName		= attributes.getValueAsString("curlayer");
			_map._bDrawGrid				= attributes.getValueAsBool("drawgrid");
			_map._colGridColor			= attributes.getValueAsInteger("gridcolor");
		}
		else if ( currentElementMatch("tilemap/background/") )
		{
			//<background enable="true" reskey="bg01" paintmode="default"/>
			_map._pMapBackground = new MapBackground;
			_map._pMapBackground->_bEnable		= attributes.getValueAsBool("enable");
			_map._pMapBackground->_strResKey	= attributes.getValueAsString("reskey");
			_map._pMapBackground->_bVisible		= attributes.getValueAsBool("visible");

			String strTmp = attributes.getValueAsString("paintmode");
			if (strTmp == "default")
				_map._pMapBackground->_ePaintMode	= ePaintModeNormal;
			else if (strTmp == "tiled")
				_map._pMapBackground->_ePaintMode	= ePaintModeTiled;
			else if (strTmp == "stretch")
				_map._pMapBackground->_ePaintMode	= ePaintModeStrench;
		}
		//else if ( currentElementMatch("tilemap/background/layers/") )
		//{
		//}
		else if ( currentElementMatch("tilemap/layers/layer/") )
		{
			//<layer name="layer0" enable="true" width="1024" height="1024" tilew="64" tileh="64">

			_pCurLayer = new MapLayer;
			_pCurLayer->Init(attributes.getValueAsString("name"), &_map);
			_pCurLayer->_bEnable		= attributes.getValueAsBool("enable");
			_pCurLayer->_bVisible		= attributes.getValueAsBool("visible");

			_map._layers.push_back(_pCurLayer);

			if (_map._strCurLayerName == _pCurLayer->_strName)
			{
				_pCurLayer->_bVisible = true;
				_map.SetCurLayer(_pCurLayer);
			}
		}
		else if ( currentElementMatch("tilemap/layers/layer/tilegroup/") )
		{
			//<tilegroup reskey="tileres01">
			_strCurGroupTile	= attributes.getValueAsString("reskey");
		}
		else if ( currentElementMatch("tilemap/layers/layer/tilegroup/tile/") )
		{
			//<tile posx="0" posy="0" id="0"/>
			STile tile;
			tile._posX			= attributes.getValueAsInteger("posx");
			tile._posY			= attributes.getValueAsInteger("posy");
			tile._strResItemID	= attributes.getValueAsString("id");
			_tiles.push_back(tile);
		}
	}

	virtual void elementEnd(const Cactus::String& element)
	{
		//if ( currentElementMatch("tilemap/") )
		//{
		//}
		//else if ( currentElementMatch("tilemap/background/") )
		//{
		//}
		//else if ( currentElementMatch("tilemap/layers/") )
		//{
		//}
		//else 
		if ( currentElementMatch("tilemap/layers/layer/") )
		{
			_pCurLayer = 0;
		}
		else if ( currentElementMatch("tilemap/layers/layer/tilegroup/") )
		{
			if (_strCurGroupTile.size())
			{
				_pCurLayer->_GroupTiles[_strCurGroupTile] = _tiles;
				_tiles.clear();
				_strCurGroupTile = "";
			}
		}
		//else if ( currentElementMatch("tilemap/layers/layer/tilegroup/tile/") )
		//{
		//}
	}

	virtual void text(const Cactus::String& content){}

protected:

	Map&			_map;

	MapLayer*		_pCurLayer;
	String			_strCurGroupTile;
	TileVectorType	_tiles;
};

//---------------------------------------------------------------------------------------------------------

Map::Map()
: _iVersion(1)
, _pMapBackground(0)
, _iWidthInTiles(1024)
, _iHeightInTiles(1024)
, _iUnitTileWidth(64)
, _iUnitTileHeight(64)
, _colBKColor(0)
, _pCurLayer(0)
, _strCurLayerName("")
, _eMapType(eRectangle)
, _bDrawGrid(true)
, _colGridColor(RGB(0, 128, 0))
{
}

Map::~Map()
{
	if (_pMapBackground)
	{
		delete _pMapBackground;
	}
}

void Map::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(int, Version, Map, Map, "地图版本.", BaseProperty::eReadOnly, _iVersion);
	pProp = M_RegisterPropertySimple(Cactus::String, Footnotes, Map, Map, "地图备注.", BaseProperty::eDefault, _strFootnotes);

	pProp = M_RegisterPropertySimple(int, WidthInTiles, Map, Map, "地图宽度.", BaseProperty::eDefault, _iWidthInTiles);
	pProp = M_RegisterPropertySimple(int, HeightInTiles, Map, Map, "地图高度.", BaseProperty::eDefault, _iHeightInTiles);
	pProp = M_RegisterPropertySimple(int, UnitTileWidth, Map, Map, "最小Tile的宽度.", BaseProperty::eDefault, _iUnitTileHeight);
	pProp = M_RegisterPropertySimple(int, UnitTileHeight, Map, Map, "最小Tile的高度.", BaseProperty::eDefault, _iUnitTileHeight);

	pProp = M_RegisterPropertySimple(int, BackgroundColor, Map, Map, "背景颜色.", BaseProperty::eDefault, _colBKColor);
	pProp->SetValueSpecify(eValueColor, "");

	pProp = M_RegisterPropertySimple(bool, DrawGrid, Map, Map, "是否绘制网格.", BaseProperty::eDefault, _bDrawGrid);
	pProp = M_RegisterPropertySimple(int, GridColor, Map, Map, "网格颜色.", BaseProperty::eDefault, _colGridColor);
	pProp->SetValueSpecify(eValueColor, "");
}

void Map::OnPropertyChanged(const std::string& propName)
{
}

bool Map::Load(const Cactus::String& strPathName)
{
	Map_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, strPathName, "");
	}
	catch (std::exception e)
	{
		Log_Error( "Map::Load, Load failed! File : " << strPathName );
		return false;
	}

	return true;
}

void Map::Save(const Cactus::String& strPathName)
{
	DataOutStreamOS os;
	if( !os.Open(strPathName) )
	{
		Log_Error("Map::Save, can not write file : " << strPathName);
		return;
	}

	String str = "<?xml version=\"1.0\" encoding=\"gb2312\"?>";
	//os << str;	//这样会先写个长度
	os.Write(&str[0], str.size());

	XMLOutStream xmlOut(&os);

	xmlOut.NodeBegin("tilemap");
		xmlOut.AddAttribute("version", _iVersion);
		xmlOut.AddAttribute("name", _strName);
		xmlOut.AddAttribute("width", _iWidthInTiles);
		xmlOut.AddAttribute("height", _iHeightInTiles);
		xmlOut.AddAttribute("wUnitTile", _iUnitTileWidth);
		xmlOut.AddAttribute("hUnitTile", _iUnitTileHeight);
		xmlOut.AddAttribute("footnotes", _strFootnotes);
		xmlOut.AddAttribute("backgroundcolor", _colBKColor);
		xmlOut.AddAttribute("drawgrid",	 _bDrawGrid);
		xmlOut.AddAttribute("gridcolor", _colGridColor);
		if (_pCurLayer)
			xmlOut.AddAttribute("curlayer", _pCurLayer->GetObjectName());
		else
			xmlOut.AddAttribute("curlayer", "");

		if (_pMapBackground)
		{
			xmlOut.NodeBegin("background");
				xmlOut.AddAttribute("enable", _pMapBackground->_bEnable);
				xmlOut.AddAttribute("reskey", _pMapBackground->_bEnable);
				xmlOut.AddAttribute("visible", _pMapBackground->_bEnable);
				if (_pMapBackground->_ePaintMode == ePaintModeNormal)
					xmlOut.AddAttribute("paintmode", "default");
				else if (_pMapBackground->_ePaintMode == ePaintModeTiled)
					xmlOut.AddAttribute("paintmode", "tiled");
				else if (_pMapBackground->_ePaintMode == ePaintModeStrench)
					xmlOut.AddAttribute("paintmode", "stretch");

			xmlOut.NodeEnd("background");
		}

		xmlOut.NodeBegin("layers");
		for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
		{
			MapLayer* pLayer = (*it);

			xmlOut.NodeBegin("layer");
				xmlOut.AddAttribute("name", pLayer->_strName);
				xmlOut.AddAttribute("enable", pLayer->_bEnable);
				xmlOut.AddAttribute("visible", pLayer->_bVisible);

			for (MapLayer::TileGroupMapType::iterator itG = pLayer->_GroupTiles.begin(); itG != pLayer->_GroupTiles.end(); ++itG)
			{
				xmlOut.NodeBegin("tilegroup");
					xmlOut.AddAttribute("reskey", itG->first);

				for (size_t t = 0; t < itG->second.size(); ++t)
				{
					STile& tile = itG->second[t];
					xmlOut.NodeBegin("tile");
						xmlOut.AddAttribute("posx", tile._posX);
						xmlOut.AddAttribute("posy", tile._posY);
						xmlOut.AddAttribute("id", tile._strResItemID);
					xmlOut.NodeEnd("tile");
				}
				xmlOut.NodeEnd("tilegroup");
			}

			xmlOut.NodeEnd("layer");
		}
		xmlOut.NodeEnd("layers");

		xmlOut.NodeBegin("brush");
		xmlOut.NodeEnd("brush");

	xmlOut.NodeEnd("tilemap");


	xmlOut.Flush();
}


void Map::Reset()
{
	if (_pMapBackground)
	{
		delete _pMapBackground;
		_pMapBackground = 0;
	}

	for (MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		delete *it;
	}
}

void Map::Draw(CDC* pDC)
{
	pDC->FillSolidRect(0, 0, _iWidthInTiles * _iUnitTileWidth, _iHeightInTiles * _iUnitTileHeight, _colBKColor);

	if (_bDrawGrid)
	{
		CPen pen(PS_SOLID, 1, _colGridColor);
		CPen* pOldPen = pDC->SelectObject(&pen);

		int iMapW = GetPixelWidth();
		int iMapH = GetPixelHeight();

		if (GetType() == eRectangle)
		{
			//画横线
			for (int i = 0; i <= _iHeightInTiles; i++)
			{
				pDC->MoveTo(0, i * _iUnitTileHeight);
				pDC->LineTo(iMapW, i * _iUnitTileHeight);
			}

			//画竖线
			for (int i = 0; i <= _iWidthInTiles; i++)
			{
				pDC->MoveTo(i * _iUnitTileWidth, 0);
				pDC->LineTo(i * _iUnitTileWidth, iMapH);
			}
		}
		else
		{
			//画斜横线
			for (int i = 0; i <= _iHeightInTiles; i++)
			{
				pDC->MoveTo(iMapW/2 - i * _iUnitTileWidth / 2
					, i * _iUnitTileHeight / 2);

				pDC->LineTo(iMapW - i * _iUnitTileWidth / 2
					, iMapH/2 + i * _iUnitTileHeight / 2);
			}

			//画斜竖线
			for (int i = 0; i <= _iWidthInTiles; i++)
			{
				pDC->MoveTo(i * _iUnitTileWidth / 2
					, iMapH/2 + i * _iUnitTileHeight / 2);

				pDC->LineTo(iMapW/2 + i * _iUnitTileWidth / 2
					, i * _iUnitTileHeight / 2);
			}
		}

		pDC->SelectObject(pOldPen);
	}


	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		if (*it != _pCurLayer)
			(*it)->Draw(pDC);

		if (_pCurLayer)
			_pCurLayer->Draw(pDC);
	}

}

void Map::SetCurLayer(MapLayer* pLayer)
{
	MapLayer* pOldLayer = _pCurLayer;

	_pCurLayer = pLayer;

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	if (_pCurLayer)
	{
		_pCurLayer->SetVisible(true);
		pMainFrame->SetCurLayerName(_pCurLayer->GetObjectName());
	}
	else
	{
		pMainFrame->SetCurLayerName("");
	}

	if (pOldLayer || _pCurLayer)
	{
		CView* pView = pMainFrame->GetActiveView(); 
		if (pView)
			pView->Invalidate();
	}
}

bool Map::AddLayer(MapLayer* pLayer)
{
	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		if ( (*it)->_strName == pLayer->_strName )
			return false;
	}

	_layers.push_back(pLayer);

	if (_layers.size() == 1)
	{
		SetCurLayer(pLayer);
	}
	return true;
}

void Map::RemoveLayer(MapLayer* pLayer)
{
	bool bIsCurrent = (_pCurLayer == pLayer);

	MapLayerListType::iterator it = find(_layers.begin(), _layers.end(), pLayer);
	if (it != _layers.end())
	{
		delete *it;
		_layers.erase(it);
	}
	else
	{
		return;
	}

	if (bIsCurrent)
	{
		SetCurLayer(0);
	}
	else
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		CView* pView = pMainFrame->GetActiveView(); 
		pView->Invalidate();
	}
}

void Map::ShowLayer(MapLayer* pLayer, bool bShow, bool bMakeCurrent)
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );

	bool bIsCurrent = (_pCurLayer == pLayer);

	if (pLayer->IsVisible() == bShow)
	{
		return;
	}

	pLayer->SetVisible(bShow);

	if (bIsCurrent && !bShow)
	{
		SetCurLayer(0);
	}
	else if (bMakeCurrent || _layers.size() == 1)
	{
		_pCurLayer = pLayer;
		pMainFrame->SetCurLayerName(_pCurLayer->GetObjectName());
	}

	CView* pView = pMainFrame->GetActiveView(); 
	pView->Invalidate();
}

bool Map::GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid)
{
	if (ptPixel.x > GetPixelWidth() || ptPixel.y > GetPixelHeight() || ptPixel.x < 0 || ptPixel.y < 0)
	{
		return false;
	}

	if (GetType() == eRectangle)
	{
		ptGrid.x	= ptPixel.x / _iUnitTileWidth;
		ptGrid.y	= ptPixel.y / _iUnitTileHeight;

		return true;
	}

/*
	int xOffset = (GetPixelWidth() - _iUnitTileWidth)/2;

	ptGrid.x	= ptPixel.y/_iUnitTileHeight + (ptPixel.x - xOffset) /_iUnitTileWidth;
	ptGrid.y	= ptPixel.y/_iUnitTileHeight - (ptPixel.x - xOffset) /_iUnitTileWidth;

	return (ptGrid.x >= 0 && ptGrid.y >= 0
		&& ptGrid.x < _iWidthInTiles && ptGrid.y < _iHeightInTiles
		);
*/

/*======================================
k = H/W;

x[0, 0.5W]
y >= -kx + 0.5H;
y <= kx + 0.5H;

x[0.5W, W]
y >= kx - 0.5H;
y <= -kx + 1.5H;

//a, b为grid坐标；x, y为像素坐标；
a - b = 2x/W;
a + b = 2y/H;
======================================*/
	bool bInRegion = false;

	int iMapW = GetPixelWidth();
	int iMapH = GetPixelHeight();

	int y = 0;
	float k = 1.0f * iMapH / iMapW;
	if (ptPixel.x <= iMapW/2)
	{
		if ( (ptPixel.y >= -k * ptPixel.x + iMapH/2) && (ptPixel.y <= k * ptPixel.x + iMapH/2) )
			bInRegion = true;
	}
	else
	{
		if ( (ptPixel.y >= k * ptPixel.x - iMapH/2) && (ptPixel.y <= -k * ptPixel.x + 1.5 * iMapH) )
			bInRegion = true;
	}

	if (bInRegion)
	{
		//计算y
		//y = kx + b;	b[-0.5H, 0.5H]
		float b = 1.0f * -iMapH/2;
		ptGrid.y = -1;
		while(ptPixel.y > k * ptPixel.x + b)
		{
			b += _iUnitTileHeight;
			ptGrid.y++;
		}

		//计算x
		//y = -kx + b;	b[0.5H, 1.5H]
		b = 1.0f * iMapH/2;
		ptGrid.x = -1;
		while(ptPixel.y > -k * ptPixel.x + b)
		{
			b += _iUnitTileHeight;
			ptGrid.x++;
		}
	}

	return bInRegion;
}

CRect Map::GetPixelCoordRect(const CPoint& ptGrid)
{
	if (GetType() == eRectangle)
	{
		return CRect(CPoint(ptGrid.x * _iUnitTileWidth, ptGrid.y * _iUnitTileHeight), CSize(_iUnitTileWidth, _iUnitTileHeight));
	}
	else
	{
		int xOffset = (GetPixelWidth() - _iUnitTileWidth)/2;

		int xLeft	= xOffset + (ptGrid.x - ptGrid.y) * _iUnitTileWidth / 2;
		int yTop	= (ptGrid.x + ptGrid.y) * _iUnitTileHeight / 2;

		return CRect(CPoint(xLeft, yTop), CSize(_iUnitTileWidth, _iUnitTileHeight));
	}
}
