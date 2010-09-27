#pragma once

struct STile 
{
	int		_posX;
	int		_posY;
	int		_ID;
};

typedef Cactus::vector<STile>::type		TileVectorType;

class MapLayer : public PropertySys::SupportRTTI<MapLayer, PropertySys::RTTIObject>
{
	friend class Map;
	friend class Map_xmlHandler;
public:
	MapLayer();
	~MapLayer();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

	void				Draw(CDC* pDC);

protected:
	Cactus::String		_strName;
	int					_iWidth;
	int					_iHeight;
	int					_iTileWidth;
	int					_iTileHeight;
	bool				_bEnable;

	typedef Cactus::map<Cactus::String, TileVectorType>::type	TileGroupMapType;	//ResourceTile key
	TileGroupMapType	_GroupTiles;
};
