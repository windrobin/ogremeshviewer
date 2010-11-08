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

	//获取名字为strGroupName的ResourceTileGroup
	ResourceTile*	GetResourceTileGroup(const Cactus::String& strGroupName);

	//获取名字为key的Resource
	Resource*		GetResourceArtGroup(const Cactus::String& key);

	/**获取名字为所有ResourceTile名字
	*/
	Cactus::StringVector	GetResourceTileNames();

	/**判断GameObjectGroup名字是否有效
	*/
	bool			IsGameObjectGroupNameValid(const Cactus::String& strName);

	/**增加一个名字为strName的游戏对象组
	*/
	bool			AddGameObjectGroup(ResourceGameObjectGroup* p);

	//获取名字为key的ResourceGameObjectGroup
	ResourceGameObjectGroup*		GetResourceGameObjectGroup(const Cactus::String& key);

	/**删除名字为strName的游戏对象组
	*/
	bool			RemoveGameObjectGroup(const Cactus::String& strName);

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
	void	_SaveResourceGameObject(const Cactus::String& strPathName);
};
