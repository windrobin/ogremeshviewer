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
public:
	MapLayer();
	~MapLayer();

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	void					Draw(CDC* pDC);

	bool					IsVisible(){ return _bVisible; }
	void					SetVisible(bool b){ _bVisible = b; }

	virtual Cactus::String	GetObjectName(){ return _strName; }

protected:
	Cactus::String		_strName;
	int					_iWidth;
	int					_iHeight;
	int					_iTileWidth;
	int					_iTileHeight;
	bool				_bEnable;

	bool				_bDrawGrid;
	//COLORREF			_colGridColor;

	typedef Cactus::map<Cactus::String, TileVectorType>::type	TileGroupMapType;	//ResourceTile key
	TileGroupMapType	_GroupTiles;

	bool				_bVisible;
};
