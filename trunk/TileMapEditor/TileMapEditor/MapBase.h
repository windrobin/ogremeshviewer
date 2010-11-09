#pragma once

#include "GridTypes.h"

class MapBaseObject : public PropertySys::SupportRTTI<MapBaseObject, GridObject>
{
public:
	MapBaseObject(){}

	virtual bool		IsLayer(){ return false; }

	Cactus::String		GetObjectName(){ return _strName; }

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:

	Cactus::String		_strName;
};

typedef Cactus::vector<int>::type	IntVectorType;
