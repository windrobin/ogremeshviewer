#pragma once

#include "ResourceType.h"

class ResourceTile: public PropertySys::SupportRTTI<ResourceTile, Resource>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTile();
	~ResourceTile();

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	virtual Cactus::String	GetResourceName(){ return _strName; }

protected:
	Cactus::String		_strName;
	int					_tilesCount;

	typedef Cactus::map<Cactus::String, CBitmap*>::type		IDBitmapMapType;
	IDBitmapMapType		_BitmapTiles;
};

