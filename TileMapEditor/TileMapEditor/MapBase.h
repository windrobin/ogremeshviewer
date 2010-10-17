#pragma once

enum EMapType
{
	eRectangle,
	eDiamond
};

class MapBaseObject : public PropertySys::SupportRTTI<MapBaseObject, PropertySys::RTTIObject>
{
public:
	MapBaseObject(){}

	virtual Cactus::String		GetObjectName(){ return ""; }

	virtual bool				IsLayer(){ return false; }

};