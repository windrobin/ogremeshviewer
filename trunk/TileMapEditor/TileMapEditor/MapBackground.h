#pragma once

#include "MapBase.h"

enum EPaintMode
{
	ePaintModeNormal,
	ePaintModeTiled,
	ePaintModeStrench,
	ePaintModeMax
};

class MapBackground : public PropertySys::SupportRTTI<MapBackground, MapBaseObject>
{
	friend class Map;
	friend class Map_xmlHandler;
public:
	MapBackground();
	~MapBackground();

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	virtual Cactus::String	GetObjectName(){ return "Background"; }

protected:
	Cactus::String		_strResKey;
	EPaintMode			_ePaintMode;
	bool				_bEnable;
	bool				_bVisible;
};
