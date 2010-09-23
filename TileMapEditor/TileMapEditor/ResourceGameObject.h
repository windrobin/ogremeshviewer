#pragma once

enum GameObjectType
{
	eGameObjectNPC,
	eGameObjectMonster,
	eGameObjectFunctionPoint,
	eGameObjectEvent,
	eGameObjectMax
};

class ResourceGameObject : public PropertySys::SupportRTTI<ResourceGameObject, PropertySys::RTTIObject>
{
	friend class ResourceManager;
public:
	ResourceGameObject();
	~ResourceGameObject();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strName;
	Cactus::String		_strArtResKey;
	int					_ArtResID;

	GameObjectType		_eType;
};

