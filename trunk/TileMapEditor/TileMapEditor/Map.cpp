#include "StdAfx.h"
#include "Map.h"
#include "MapBackground.h"
#include "MapLayer.h"

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
			_map._iWidth				= attributes.getValueAsInteger("width");
			_map._iHeight				= attributes.getValueAsInteger("height");
			_map._iTileWidthDefault		= attributes.getValueAsInteger("tilew");
			_map._iTileHeightDefault	= attributes.getValueAsInteger("tileh");
			_map._strFootnotes			= attributes.getValueAsString("footnotes");
		}
		else if ( currentElementMatch("tilemap/background/") )
		{
			//<background enable="true" reskey="bg01" paintmode="default"/>
			_map._pMapBackground = new MapBackground;
			_map._pMapBackground->_bEnable		= attributes.getValueAsBool("enable");
			_map._pMapBackground->_strResKey	= attributes.getValueAsString("reskey");

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
			_pCurLayer->_strName		= attributes.getValueAsString("name");
			_pCurLayer->_bEnable		= attributes.getValueAsBool("enable");
			_pCurLayer->_iWidth			= attributes.getValueAsInteger("width");
			_pCurLayer->_iHeight		= attributes.getValueAsInteger("height");
			_pCurLayer->_iTileWidth		= attributes.getValueAsInteger("tilew");
			_pCurLayer->_iTileHeight	= attributes.getValueAsInteger("tileh");

			_map._layers.push_back(_pCurLayer);
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
			tile._posX	= attributes.getValueAsInteger("posx");
			tile._posY	= attributes.getValueAsInteger("posy");
			tile._strID	= attributes.getValueAsString("id");
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
: _pMapBackground(0)
, _iWidth(1024)
, _iHeight(1024)
, _iTileHeightDefault(64)
, _iTileWidthDefault(64)
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

	pProp = M_RegisterPropertySimple(Cactus::String, Name, Map, Map, "地图名.", BaseProperty::eDefault, _strName);

	pProp = M_RegisterPropertySimple(int, Version, Map, Map, "地图版本.", BaseProperty::eReadOnly, _iVersion);
	pProp = M_RegisterPropertySimple(Cactus::String, Footnotes, Map, Map, "地图备注.", BaseProperty::eDefault, _strFootnotes);

	pProp = M_RegisterPropertySimple(int, Width, Map, Map, "地图宽度.", BaseProperty::eDefault, _iWidth);
	pProp = M_RegisterPropertySimple(int, Height, Map, Map, "地图高度.", BaseProperty::eDefault, _iHeight);
	pProp = M_RegisterPropertySimple(int, DefaultTileWidth, Map, Map, "缺省的Tile宽度.", BaseProperty::eDefault, _iTileWidthDefault);
	pProp = M_RegisterPropertySimple(int, DefaultTileHeight, Map, Map, "缺省的Tile高.", BaseProperty::eDefault, _iTileHeightDefault);
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
	pDC->FillSolidRect(0, 0, _iWidth, _iHeight, 0);

	CPen pen(PS_SOLID, 1, RGB(128, 128, 128));
	CPen* pOldPen = pDC->SelectObject(&pen);

	int iGridWidth	= _iWidth / _iTileWidthDefault;
	int iGridHeight	= _iHeight / _iTileHeightDefault;
	for (int i = 0; i <= iGridWidth; i++)
	{
		pDC->MoveTo(0, i * _iTileWidthDefault);
		pDC->LineTo(_iHeight, i * _iTileWidthDefault);
	}
	for (int i = 0; i <= iGridHeight; i++)
	{
		pDC->MoveTo(i * _iTileHeightDefault, 0);
		pDC->LineTo(i * _iTileHeightDefault, _iWidth);
	}

	pDC->SelectObject(pOldPen);

	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		(*it)->Draw(pDC);
	}
}
