#pragma once

#include "ResourceType.h"

class ResourceBackground;
class ResourceTile;
class ResourceGameObjectGroup;

//��Դ������
class ResourceManager : public Cactus::Singleton<ResourceManager>
{
	friend class ResourceArt_xmlHandler;
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceTreeView;

public:
	ResourceManager();
	~ResourceManager();

	//������Դ���ú���Դ
	bool	Load(const Cactus::String& strRootPath);

	//����������Դ
	void	Reset();

	//��ȡ���ݸ�Ŀ¼
	Cactus::String	GetRootFolder(){ return _strRootFolder; }

	//ResourceTile������Դʮ����Ч
	bool			IsResourceTileIDValid(const Cactus::String& tile, const Cactus::String& strID);

	//��ȡ����Ϊtile��ResourceTile
	ResourceTile*	GetResourceTile(const Cactus::String& strName);

	//��ȡ����Ϊkey��Resource
	Resource*		GetResource(const Cactus::String& key);

protected:
	Cactus::String				_strRootFolder;			//���ݸ�Ŀ¼

	typedef	Cactus::map<Cactus::String, ResourceBackground*>::type		ResBackgroundType;
	ResBackgroundType			_ResBackgrounds;		//���еı�����Դ

	typedef	Cactus::map<Cactus::String, ResourceTile*>::type			ResTileType;
	ResTileType					_ResourceTiles;			//���е�ResourcTile��Դ

	typedef Cactus::map<Cactus::String, ResourceGameObjectGroup*>::type	ResGameObjectGroupMapType;
	ResGameObjectGroupMapType	_ResGameObjectGroups;	//���е�GameObjectGroup��Դ

	bool	_LoadResourceArt(const Cactus::String& strPathName);
	bool	_LoadResourceGameObject(const Cactus::String& strPathName);
};
