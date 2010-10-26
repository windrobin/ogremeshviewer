#pragma once

#include "MapBase.h"

struct STile 
{
	int				_posX;			///网格坐标X
	int				_posY;			///网格坐标Y
	int				_regionID;		///所在的RegionID
	Cactus::String	_strResGroup;	///使用的资源组
	Cactus::String	_strResItemID;	///使用的资源
};

typedef Cactus::vector<STile>::type		TileVectorType;

class MapLayer : public PropertySys::SupportRTTI<MapLayer, MapBaseObject>
{
	friend class Map;
	friend class Map_xmlHandler;

public:
	MapLayer();
	~MapLayer();

	void					Init(const Cactus::String& strName, Map* pParent);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	void					Draw(CDC* pDC, const IntVectorType& regions);

	bool					IsVisible(){ return _bVisible; }
	void					SetVisible(bool b){ _bVisible = b; }

	virtual Cactus::String	GetObjectName(){ return _strName; }
	virtual bool			IsLayer(){ return true; }

	bool					ToolHitTest(CPoint pt, int& gridX, int& gridY, CRect& rc);

	bool					AddOrModifyTile(int gridX, int gridY, const Cactus::String& resGroup, const Cactus::String& strItemID);
	bool					ClearTile(int gridX, int gridY);
	bool					GetTileInfo(int gridX, int gridY, STile& tile);

protected:
	Map*				_pParentMap;

	bool				_bEnable;
	bool				_bVisible;

	typedef Cactus::map<int, TileVectorType>::type		RegionTileMapType;
	RegionTileMapType	_GroupTiles;
};
