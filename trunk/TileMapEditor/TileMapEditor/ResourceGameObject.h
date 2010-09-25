#pragma once

#include "ResourceType.h"

class ResourceGameObject : public PropertySys::SupportRTTI<ResourceGameObject, PropertySys::RTTIObject>
{
	friend class ResourceManager;
	friend class ResourceGameObject_xmlHandler;
public:
	ResourceGameObject();
	~ResourceGameObject();

	bool				Load();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strName;
	Cactus::String		_strArtResKey;
	int					_ArtResID;

	GameObjectType		_eType;
};

