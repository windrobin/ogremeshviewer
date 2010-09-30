#pragma once

#include "ResourceType.h"

class ResourceGameObject
{
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceGameObjectGroup;
public:
	ResourceGameObject();
	~ResourceGameObject();

	virtual bool		Load(const Cactus::String& strTile);

protected:
	Cactus::String		_strName;
	int					_ArtResID;

	GameObjectType		_eType;
};

//--------------------------------------------------------------------------------------------------------

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
	virtual Cactus::String	GetResourceName(){ return _strGroupName; }

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

protected:
	typedef Cactus::list<ResourceGameObject*>::type		ResGameObjectListType;
	ResGameObjectListType	_ResGameObjects;

	Cactus::String			_strGroupName;
	Cactus::String			_strArtResKey;
};


