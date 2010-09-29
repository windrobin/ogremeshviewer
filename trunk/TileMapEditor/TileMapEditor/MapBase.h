#pragma once

class MapBaseObject : public PropertySys::SupportRTTI<MapBaseObject, PropertySys::RTTIObject>
{
public:
	MapBaseObject(){}

	virtual Cactus::String		GetObjectName(){ return ""; }

};