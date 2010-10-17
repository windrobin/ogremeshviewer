#pragma once

#include "MapBase.h"

struct STile 
{
	int				_posX;
	int				_posY;
	Cactus::String	_strID;
};

typedef Cactus::vector<STile>::type		TileVectorType;

class MapLayer : public PropertySys::SupportRTTI<MapLayer, MapBaseObject>
{
	friend class Map;
	friend class Map_xmlHandler;
	friend class MapView;
public:
	MapLayer();
	~MapLayer();

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	void					Draw(CDC* pDC);

	bool					IsVisible(){ return _bVisible; }
	void					SetVisible(bool b){ _bVisible = b; }

	virtual Cactus::String	GetObjectName(){ return _strName; }
	virtual bool			IsLayer(){ return true; }

	bool					ToolHitTest(CPoint pt, int& gridX, int& gridY, CRect& rc);

	bool					ModifyTile(int gridX, int gridY, const Cactus::String& resKey, const Cactus::String& strID);
	bool					ClearTile(int gridX, int gridY);
	bool					GetTileInfo(int gridX, int gridY, STile& tile, Cactus::String& resKey);

protected:
	Map*				_pParentMap;

	Cactus::String		_strName;
	int					_iWidthInTiles;			//in map UnitTile
	int					_iHeightInTiles;		//in map UnitTile
	bool				_bEnable;

	bool				_bDrawGrid;
	//COLORREF			_colGridColor;

	typedef Cactus::map<Cactus::String, TileVectorType>::type	TileGroupMapType;	//ResourceTile key
	TileGroupMapType	_GroupTiles;

	bool				_bVisible;
};
