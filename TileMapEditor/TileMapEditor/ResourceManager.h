#pragma once

class ResourceBackground;
class ResourceTile;

class ResourceGameObject;

class ResourceManager : public Cactus::Singleton<ResourceManager>
{
	friend class ResourceArt_xmlHandler;
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceGameEvent_xmlHandler;
public:
	ResourceManager();
	~ResourceManager();

	bool	Load(const Cactus::String& strRootPath);
	void	Reset();

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

	bool	_LoadResourceArt(const Cactus::String& strPathName);
	bool	_LoadResourceGameObject(const Cactus::String& strPathName);
	bool	_LoadResourceGameEvent(const Cactus::String& strPathName);
};
