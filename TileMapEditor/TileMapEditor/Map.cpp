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
			//<tilemap version="1" name="test001" width="1024" height="1024" tilew="64" tileh="64" footnotes="����һ�����Ե�ͼ�ļ���">
			_map._iVersion				= attributes.getValueAsInteger("version");
			_map._strName				= attributes.getValueAsString("name");
			_map._iWidth				= attributes.getValueAsInteger("width");
			_map._iHeight				= attributes.getValueAsInteger("height");
			_map._iTileWidthDefault		= attributes.getValueAsInteger("tilew");
			_map._iTileHeightDefault	= attributes.getValueAsInteger("tileh");
			_map._strFootnotes			= attributes.getValueAsString("footnotes");
			_map._bDrawGrid				= attributes.getValueAsBool("drawgrid");
			_map._colBKColor			= attributes.getValueAsInteger("backgroundcolor");
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
			_pCurLayer->_strName		= attributes.getValueAsString("name");
			_pCurLayer->_bEnable		= attributes.getValueAsBool("enable");
			_pCurLayer->_iWidth			= attributes.getValueAsInteger("width");
			_pCurLayer->_iHeight		= attributes.getValueAsInteger("height");
			_pCurLayer->_iTileWidth		= attributes.getValueAsInteger("tilew");
			_pCurLayer->_iTileHeight	= attributes.getValueAsInteger("tileh");
			_pCurLayer->_bDrawGrid		= attributes.getValueAsBool("drawgrid");
			_pCurLayer->_bVisible		= attributes.getValueAsBool("visible");


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
: _iVersion(1)
, _pMapBackground(0)
, _iWidth(1024)
, _iHeight(1024)
, _iTileHeightDefault(64)
, _iTileWidthDefault(64)
, _bDrawGrid(true)
, _colBKColor(0)
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

	pProp = M_RegisterPropertySimple(Cactus::String, Name, Map, Map, "��ͼ��.", BaseProperty::eDefault, _strName);

	pProp = M_RegisterPropertySimple(int, Version, Map, Map, "��ͼ�汾.", BaseProperty::eReadOnly, _iVersion);
	pProp = M_RegisterPropertySimple(Cactus::String, Footnotes, Map, Map, "��ͼ��ע.", BaseProperty::eDefault, _strFootnotes);

	pProp = M_RegisterPropertySimple(int, Width, Map, Map, "��ͼ����.", BaseProperty::eDefault, _iWidth);
	pProp = M_RegisterPropertySimple(int, Height, Map, Map, "��ͼ�߶�.", BaseProperty::eDefault, _iHeight);
	pProp = M_RegisterPropertySimple(int, DefaultTileWidth, Map, Map, "ȱʡ��Tile����.", BaseProperty::eDefault, _iTileWidthDefault);
	pProp = M_RegisterPropertySimple(int, DefaultTileHeight, Map, Map, "ȱʡ��Tile��.", BaseProperty::eDefault, _iTileHeightDefault);

	pProp = M_RegisterPropertySimple(int, BackgroundColor, Map, Map, "������ɫ.", BaseProperty::eDefault, _colBKColor);
	pProp->SetValueSpecify(eValueColor, "");

	pProp = M_RegisterPropertySimple(bool, DrawGrid, Map, Map, "�Ƿ��������.", BaseProperty::eDefault, _bDrawGrid);
	//pProp = M_RegisterPropertySimple(int, GridColor, Map, Map, "������ɫ.", BaseProperty::eDefault, _colGridColor);
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

	XMLOutStream xmlOut(&os);

	xmlOut.NodeBegin("tilemap");
		xmlOut.AddAttribute("version", _iVersion);
		xmlOut.AddAttribute("name", _strName);
		xmlOut.AddAttribute("width", _iWidth);
		xmlOut.AddAttribute("height", _iHeight);
		xmlOut.AddAttribute("tilew", _iTileWidthDefault);
		xmlOut.AddAttribute("tileh", _iTileHeightDefault);
		xmlOut.AddAttribute("footnotes", _strFootnotes);
		xmlOut.AddAttribute("drawgrid", _bDrawGrid);
		xmlOut.AddAttribute("backgroundcolor", _colBKColor);

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
				xmlOut.AddAttribute("width", pLayer->_iWidth);
				xmlOut.AddAttribute("height", pLayer->_iHeight);
				xmlOut.AddAttribute("tilew", pLayer->_iTileWidth);
				xmlOut.AddAttribute("tileh", pLayer->_iTileHeight);
				xmlOut.AddAttribute("drawgrid", pLayer->_bDrawGrid);
				xmlOut.AddAttribute("visible", pLayer->_bVisible);

			for (MapLayer::TileGroupMapType::iterator itG = pLayer->_GroupTiles.begin(); itG != pLayer->_GroupTiles.end(); ++itG)
			{
				for (size_t t = 0; t < itG->second.size(); ++t)
				{
					STile& tile = itG->second[t];
					xmlOut.NodeBegin("tile");
						xmlOut.AddAttribute("posx", tile._posX);
						xmlOut.AddAttribute("posy", tile._posY);
						xmlOut.AddAttribute("id", tile._strID);
					xmlOut.NodeEnd("tile");
				}
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
	pDC->FillSolidRect(0, 0, _iWidth, _iHeight, _colBKColor);

	if (_bDrawGrid)
	{
		CPen pen(PS_DOT, 1, RGB(128, 128, 128));
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
	}

	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		(*it)->Draw(pDC);
	}
}