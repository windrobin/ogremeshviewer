#pragma once

#include "ResourceType.h"

//游戏对象资源
class ResourceGameObject
{
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceGameObjectGroup;
public:
	ResourceGameObject();
	~ResourceGameObject();

	//加载对象
	virtual bool		Load(const Cactus::String& strTile);

protected:
	Cactus::String		_strName;	//名字
	Cactus::String		_ArtResID;	//依赖的子图像资源名称

	EGameObjectType		_eType;		//类型
};

//--------------------------------------------------------------------------------------------------------
//游戏对象组，一个游戏对象组只能依赖一个图像资源对象
class ResourceGameObjectGroup : public PropertySys::SupportRTTI<ResourceGameObjectGroup, Resource>
{
	friend class ResourceManager;
	friend class ResourceGameObject_xmlHandler;
public:
	ResourceGameObjectGroup();
	~ResourceGameObjectGroup();

	virtual bool			Load(){ return _ResGameObjects.size() != 0; }
	virtual void			CreateImageList(CDC* pDC);
	virtual void			Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

protected:
	typedef Cactus::list<ResourceGameObject*>::type		ResGameObjectListType;
	ResGameObjectListType	_ResGameObjects;

	Cactus::String			_strArtResKey;	//依赖的图像资源名称
};


