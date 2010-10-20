#pragma once

#include "MapBase.h"

class MapLayer;
class MapBackground;

class Map : public PropertySys::SupportRTTI<Map, MapBaseObject>
{
	friend class Map_xmlHandler;
	friend class CTileMapEditorView;
	friend class CTileMapEditorDoc;
	friend class MapView;
	friend class MapLayer;

public:
	Map();
	~Map();

	void						Draw(CDC* pDC);
	bool						Load(const Cactus::String& strPathName);
	void						Save(const Cactus::String& strPathName);
	void						Reset();

	static void					RegisterReflection();
	virtual void				OnPropertyChanged(const std::string& propName);

	virtual Cactus::String		GetObjectName(){ return _strName; }

	//设置当前Layer，如果Layer不可见，设置为可见，并负责刷新Ribbon和View
	void						SetCurLayer(MapLayer* pLayer);
	MapLayer*					GetCurLayer(){ return _pCurLayer; }

	//如果是唯一的Layer，将被设置为当前的
	bool						AddLayer(MapLayer* pLayer);

	void						RemoveLayer(MapLayer* pLayer);

	//bMakeCurrent只有在bShow为true时才有用
	void						ShowLayer(MapLayer* pLayer, bool bShow, bool bMakeCurrent = false);

	int							GetPixelWidth(){ return _iWidthInTiles * _iUnitTileWidth; }
	int							GetPixelHeight(){ return _iHeightInTiles * _iUnitTileHeight; }

	EMapType					GetType(){ return _eMapType; }
	
	bool						GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid);
	CRect						GetPixelCoordRect(const CPoint& ptGrid);

protected:
	Cactus::String		_strName;
	Cactus::String		_strFootnotes;
	Cactus::String		_strCurLayerName;
	int					_iWidthInTiles;
	int					_iHeightInTiles;
	int					_iUnitTileWidth;
	int					_iUnitTileHeight;
	int					_iVersion;

	EMapType			_eMapType;

	COLORREF			_colBKColor;

	MapBackground*		_pMapBackground;

	typedef Cactus::list<MapLayer*>::type		MapLayerListType;
	MapLayerListType	_layers;

	MapLayer*			_pCurLayer;
};
