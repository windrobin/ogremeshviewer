#pragma once

#include "ResourceType.h"

class ResourceBackground;
class ResourceTile;
class ResourceGameObjectGroup;

//资源管理器
class ResourceManager : public Cactus::Singleton<ResourceManager>
{
	friend class ResourceArt_xmlHandler;
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceTreeView;

public:
	ResourceManager();
	~ResourceManager();

	//加载资源配置和资源
	bool	Load(const Cactus::String& strRootPath);

	//重置所有资源
	void	Reset();

	//获取数据根目录
	Cactus::String	GetRootFolder(){ return _strRootFolder; }

	//ResourceTile的子资源十分有效
	bool			IsResourceTileIDValid(const Cactus::String& tile, const Cactus::String& strID);

	//获取名字为tile的ResourceTile
	ResourceTile*	GetResourceTile(const Cactus::String& strName);

	//获取名字为key的Resource
	Resource*		GetResource(const Cactus::String& key);

protected:
	Cactus::String				_strRootFolder;			//数据根目录

	typedef	Cactus::map<Cactus::String, ResourceBackground*>::type		ResBackgroundType;
	ResBackgroundType			_ResBackgrounds;		//所有的背景资源

	typedef	Cactus::map<Cactus::String, ResourceTile*>::type			ResTileType;
	ResTileType					_ResourceTiles;			//所有的ResourcTile资源

	typedef Cactus::map<Cactus::String, ResourceGameObjectGroup*>::type	ResGameObjectGroupMapType;
	ResGameObjectGroupMapType	_ResGameObjectGroups;	//所有的GameObjectGroup资源

	bool	_LoadResourceArt(const Cactus::String& strPathName);
	bool	_LoadResourceGameObject(const Cactus::String& strPathName);
};
