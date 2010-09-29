#pragma once

enum EPaintMode
{
	ePaintModeNormal,
	ePaintModeTiled,
	ePaintModeStrench,
	ePaintModeMax
};

class MapBackground : public PropertySys::SupportRTTI<MapBackground, PropertySys::RTTIObject>
{
	friend class Map;
	friend class Map_xmlHandler;
public:
	MapBackground();
	~MapBackground();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strResKey;
	EPaintMode			_ePaintMode;
	bool				_bEnable;
	bool				_bVisible;
};
