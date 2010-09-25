#pragma once

#include "ResourceType.h"

class ResourceBackground;
class ResourceTile;

class ResourceGameObject;

class ResourceManager : public Cactus::Singleton<ResourceManager>
{
	friend class ResourceArt_xmlHandler;
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceGameEvent_xmlHandler;
	friend class ResourceTreeView;
public:
	ResourceManager();
	~ResourceManager();

	bool	Load(const Cactus::String& strRootPath);
	void	Reset();

	Cactus::String	GetRootFolder(){ return _strRootFolder; }

	bool	IsResTileIDValid(const Cactus::String& tile, int ID);

protected:
	Cactus::String			_strRootFolder;

	typedef	Cactus::map<Cactus::String, ResourceBackground*>::type	ResBackgroundType;
	ResBackgroundType		_ResBackgrounds;

	typedef	Cactus::map<Cactus::String, ResourceTile*>::type		ResTileType;
	ResTileType				_ResTiles;

	typedef Cactus::list<ResourceGameObject*>::type					ResGameObjectListType;
	ResGameObjectListType	_ResGameObjects[eGameObjectMax];

	bool	_LoadResourceArt(const Cactus::String& strPathName);
	bool	_LoadResourceGameObject(const Cactus::String& strPathName);
	bool	_LoadResourceGameEvent(const Cactus::String& strPathName);
};
