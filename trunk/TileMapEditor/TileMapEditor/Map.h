#pragma once

class MapLayer;

class Map : public PropertySys::SupportRTTI<Map, PropertySys::RTTIObject>
{
public:
	Map();
	~Map();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strName;
	Cactus::String		_strFootnotes;
	int					_iWidth;
	int					_iHeight;
	int					_iTileWidthDefault;
	int					_iTileHeightDefault;

	typedef Cactus::list<MapLayer*>::type		MapLayerListType;
	MapLayerListType	_layers;
};
