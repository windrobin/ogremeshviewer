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
	Map_xmlHandler(Map& map) : _map(map)
	{
		_iCurRegionGroupTile = -1;
	}
	virtual ~Map_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if ( currentElementMatch("tilemap/") )
		{
			//<tilemap version="1" name="map_xx" width="200" height="200" wUnitTile="64" hUnitTile="64" footnotes="请输入备注。" 
			//backgroundcolor="0" drawgrid="true" gridcolor="32768" curlayer="layer_xx" regionW="10" regionH="10">
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
			_map._eGridType				= EGridType( attributes.getValueAsInteger("type") );
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
			_iCurRegionGroupTile	= attributes.getValueAsInteger("region");
		}
		else if ( currentElementMatch("tilemap/layers/layer/tilegroup/tile/") )
		{
			//<tile posx="0" posy="0" id="0"/>
			STile* pTile = new STile;
			pTile->_ptGrid.x		= attributes.getValueAsInteger("posx");
			pTile->_ptGrid.y		= attributes.getValueAsInteger("posy");
			pTile->_strResGroup		= attributes.getValueAsString("resGroup");
			pTile->_strResItemID	= attributes.getValueAsString("resItemID");
			pTile->_regionID		= _iCurRegionGroupTile;
			_tiles.push_back(pTile);
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
			if (_iCurRegionGroupTile != -1)
			{
				_pCurLayer->_GroupTiles[_iCurRegionGroupTile] = _tiles;
				_tiles.clear();
				_iCurRegionGroupTile = -1;
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
	int				_iCurRegionGroupTile;
	TileVectorType	_tiles;
};

//---------------------------------------------------------------------------------------------------------

Map::Map()
: _iVersion(1)
, _pMapBackground(0)
, _iRegionWidth(10)
, _iRegionHeight(10)
, _colBKColor(0)
, _pCurLayer(0)
, _strCurLayerName("")
, _bDrawGrid(true)
, _colGridColor(RGB(0, 128, 0))
, _bDrawRegionGrid(true)
, _colRegionGridColor(RGB(192, 192, 0))
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

	pProp = M_RegisterPropertySimple(int, BackgroundColor, Map, Map, "背景颜色.", BaseProperty::eDefault, _colBKColor);
	pProp->SetValueSpecify(eValueColor, "");

	pProp = M_RegisterPropertySimple(bool, DrawGrid, Map, Map, "是否绘制网格.", BaseProperty::eDefault, _bDrawGrid);
	pProp = M_RegisterPropertySimple(int, GridColor, Map, Map, "网格颜色.", BaseProperty::eDefault, _colGridColor);
	pProp->SetValueSpecify(eValueColor, "");

	pProp = M_RegisterPropertySimple(bool, DrawRegionGrid, Map, Map, "是否绘制区域网格.", BaseProperty::eDefault, _bDrawRegionGrid);
	pProp = M_RegisterPropertySimple(int, RegionGridColor, Map, Map, "区域网格颜色.", BaseProperty::eDefault, _colRegionGridColor);
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

	CalculateRegionInfo();

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

	String str = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n";
	//os << str;	//这样会先写个长度
	os.Write(&str[0], str.size());

	XMLOutStream xmlOut(&os);

	xmlOut.NodeBegin("tilemap");
		xmlOut.AddAttribute("version", _iVersion);
		xmlOut.AddAttribute("name", _strName);
		xmlOut.AddAttribute("type", _eGridType);
		xmlOut.AddAttribute("width", _iWidthInTiles);
		xmlOut.AddAttribute("height", _iHeightInTiles);
		xmlOut.AddAttribute("wUnitTile", _iUnitTileWidth);
		xmlOut.AddAttribute("hUnitTile", _iUnitTileHeight);
		xmlOut.AddAttribute("footnotes", _strFootnotes);
		xmlOut.AddAttribute("backgroundcolor", _colBKColor);
		xmlOut.AddAttribute("drawgrid",	 _bDrawGrid);
		xmlOut.AddAttribute("gridcolor", _colGridColor);
		xmlOut.AddAttribute("regionW", _iRegionWidth);
		xmlOut.AddAttribute("regionH", _iRegionHeight);
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

			for (MapLayer::RegionTileMapType::iterator itG = pLayer->_GroupTiles.begin(); itG != pLayer->_GroupTiles.end(); ++itG)
			{
				xmlOut.NodeBegin("tilegroup");
					xmlOut.AddAttribute("region", itG->first);

				for (size_t t = 0; t < itG->second.size(); ++t)
				{
					STile* pTile = itG->second[t];
					xmlOut.NodeBegin("tile");
						xmlOut.AddAttribute("posx", pTile->_ptGrid.x);
						xmlOut.AddAttribute("posy", pTile->_ptGrid.y);
						xmlOut.AddAttribute("resGroup", pTile->_strResGroup);
						xmlOut.AddAttribute("resItemID", pTile->_strResItemID);
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

void Map::Draw(CDC* pDC, const CRect& rcView)
{
	pDC->FillSolidRect(0, 0, _iWidthInTiles * _iUnitTileWidth, _iHeightInTiles * _iUnitTileHeight, _colBKColor);

	//绘制网格
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

	//绘制MapLayer
	CRect rcDirty = rcView;
	rcDirty.InflateRect(_iRegionWidth * _iUnitTileWidth, _iRegionHeight * _iUnitTileHeight);

	IntVectorType Regions = GetIntersectRegions(rcDirty);
	if (Regions.size() == 0)
		return;

	for(MapLayerListType::iterator it = _layers.begin(); it != _layers.end(); ++it)
	{
		if (*it != _pCurLayer)
			(*it)->Draw(pDC, Regions);

		if (_pCurLayer)
			_pCurLayer->Draw(pDC, Regions);
	}

	//绘制区域网格
	if(_bDrawRegionGrid)
	{
		CPen pen(PS_SOLID, 1, _colRegionGridColor);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->SelectStockObject(NULL_BRUSH);

		for(RegionMapType::iterator it = _regions.begin(); it != _regions.end(); ++it)
		{
			CRect rc = it->second._rcPixel;
			rc.DeflateRect(1, 1, 1, 1);

			CString strTmp;
			strTmp.Format(_T("%d"), it->first);

			pDC->SetTextColor(_colRegionGridColor);
			pDC->SetBkMode(TRANSPARENT);

			if (_eGridType == eRectangle)
			{
				pDC->DrawText(strTmp, rc, DT_LEFT | DT_TOP);
				pDC->Rectangle(rc);
			}
			else
			{
				rc.DeflateRect(1, 1, 1, 1);

				CRect rcText = rc;
				rcText.left	= rc.left + rc.Width()/2 - 5;
				rcText.top	+= 10;

				pDC->DrawText(strTmp, rcText, DT_LEFT | DT_TOP);

				CPoint pts[4];
				pts[0] = CPoint(rc.left, rc.top + rc.Height()/2);
				pts[1] = CPoint(rc.left + rc.Width()/2, rc.top);
				pts[2] = CPoint(rc.right, rc.top + rc.Height()/2);
				pts[3] = CPoint(rc.left + rc.Width()/2, rc.bottom);

				pDC->Polygon(pts, 4);
			}
		}

		pDC->SelectObject(pOldPen);
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
		pMainFrame->SetCurLayer(_pCurLayer);
	}
	else
	{
		pMainFrame->SetCurLayer(0);
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
		pMainFrame->SetCurLayer(_pCurLayer);
	}

	CView* pView = pMainFrame->GetActiveView(); 
	pView->Invalidate();
}

int Map::GetRegionID(const CPoint& ptGrid)
{
	for(RegionMapType::iterator it = _regions.begin(); it != _regions.end(); ++it)
	{
		if (it->second._rcGrid.PtInRect(ptGrid))
		{
			return it->first;
		}
	}

	return -1;
}

bool Map::GetRegionGridRect(int ID, CRect& gridRect, CRect& pixelRect)
{
	RegionMapType::iterator it = _regions.find(ID);
	if (it == _regions.end())
	{
		return false;
	}

	gridRect	= it->second._rcGrid;
	pixelRect	= it->second._rcPixel;

	return true;
}

IntVectorType Map::GetIntersectRegions(const CRect& rcView)
{
	IntVectorType IDs;

	for(RegionMapType::iterator it = _regions.begin(); it != _regions.end(); ++it)
	{
		CRect rcRegion = it->second._rcPixel;
		CRect rcTmp;
		if( rcTmp.IntersectRect(rcRegion, rcView) )
		{
			IDs.push_back(it->first);
		}
	}

	return IDs;
}

IntVectorType Map::GetAdjacentRegions(int ID)
{
	IntVectorType ids;

	int id = ID - _iWRegionCount - 1;
	if (id >= 0)
		ids.push_back(id);

	id = ID - _iWRegionCount;
	if (id >= 0)
		ids.push_back(id);

	id = ID - _iWRegionCount + 1;
	if (id >= 0)
		ids.push_back(id);




	id = ID - 1;
	if (id >= 0)
		ids.push_back(id);

	id = ID + 1;
	if (id < _regions.size())
		ids.push_back(id);



	id = ID + _iWRegionCount - 1;
	if (id < _regions.size())
		ids.push_back(id);

	id = ID + _iWRegionCount;
	if (id < _regions.size())
		ids.push_back(id);

	id = ID + _iWRegionCount + 1;
	if (id < _regions.size())
		ids.push_back(id);

	return ids;
}


void Map::CalculateRegionInfo()
{
	_regions.clear();

	int W = (_iWidthInTiles + _iRegionWidth - 1)/ _iRegionWidth;
	int H = (_iHeightInTiles + _iRegionHeight - 1)/ _iRegionHeight;

	_iWRegionCount	= W;
	_iWRegionCount	= H;

	if (_eGridType == eRectangle)
	{
		for (int j = 0; j < H; ++j)
		{
			for (int i = 0; i < W; ++i)
			{
				SRegionInfo info;
				info._regionID	= j * W + i;
				info._rcGrid	= CRect(CPoint(i * _iRegionWidth, j * _iRegionHeight)
					, CSize(_iRegionWidth, _iRegionHeight)
					);
				info._rcPixel	= CRect(CPoint(i * _iRegionWidth * _iUnitTileWidth, j * _iRegionHeight * _iUnitTileHeight)
					, CSize(_iRegionWidth * _iUnitTileWidth, _iRegionHeight * _iUnitTileHeight)
					);

				_regions[info._regionID] = info;
			}
		}
	}
	else
	{
		for (int j = 0; j < H; ++j)
		{
			for (int i = 0; i < W; ++i)
			{
				SRegionInfo info;
				info._regionID	= j * W + i;
				info._rcGrid	= CRect(CPoint(i * _iRegionWidth, j * _iRegionHeight)
					, CSize(_iRegionWidth, _iRegionHeight)
					);

				CRect rcLeft	= GetPixelCoordRect(CPoint(i * _iRegionWidth , (j + 1) * _iRegionHeight - 1 ));
				CRect rcTop		= GetPixelCoordRect(CPoint(i * _iRegionWidth , j * _iRegionHeight));
				CRect rcRight	= GetPixelCoordRect(CPoint((i + 1) * _iRegionWidth - 1, j * _iRegionHeight));
				CRect rcBottom	= GetPixelCoordRect(CPoint((i + 1) * _iRegionWidth , (j + 1) * _iRegionHeight));

				CPoint ptPixel;
				ptPixel.x = rcLeft.TopLeft().x;
				ptPixel.y = rcTop.TopLeft().y;

				CSize szPixel;
				szPixel.cx = rcRight.BottomRight().x - rcLeft.TopLeft().x;
				szPixel.cy = rcBottom.TopLeft().y - rcTop.TopLeft().y;

				info._rcPixel	= CRect(ptPixel, szPixel);

				_regions[info._regionID] = info;
			}
		}
	}
}

