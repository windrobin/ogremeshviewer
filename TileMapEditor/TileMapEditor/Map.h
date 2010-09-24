#pragma once

class MapLayer;

class Map : public PropertySys::SupportRTTI<Map, PropertySys::RTTIObject>
{
	friend class Map_xmlHandler;
public:
	Map();
	~Map();

	bool				Load(const Cactus::String& strPathName);

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
