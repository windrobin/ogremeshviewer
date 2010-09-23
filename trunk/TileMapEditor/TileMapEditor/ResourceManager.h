#pragma once

class ResourceBackground;
class ResourceTile;

class ResourceGameObject;

class ResourceManager : public Cactus::Singleton<ResourceManager>
{
public:
	ResourceManager();
	~ResourceManager();

	bool	Load(const Cactus::String& strRootPath);

protected:
	typedef	Cactus::map<Cactus::String, ResourceBackground*>::type	ResBackgroundType;
	ResBackgroundType	_ResBackgrounds;

	typedef	Cactus::map<Cactus::String, ResourceTile*>::type		ResTileType;
	ResTileType			_ResTiles;

	typedef	Cactus::map<Cactus::String, ResourceGameObject*>::type	ResGameObjectType;
	ResGameObjectType	_ResGameObjectNPC;
	ResGameObjectType	_ResGameObjectMonster;
	ResGameObjectType	_ResGameObjectFunctionPoint;
	ResGameObjectType	_ResGameObjectEvent;
};
