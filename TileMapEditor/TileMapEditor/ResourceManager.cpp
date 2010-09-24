#include "StdAfx.h"
#include "ResourceManager.h"

#include "ResourceBackground.h"
#include "ResourceTile.h"

#include "ResourceGameObject.h"

using namespace Cactus;

ResourceManager* Cactus::Singleton<ResourceManager>::s_pSingleton = 0;

//---------------------------------------------------------------------------------------------------------

class ResourceArt_xmlHandler : public Cactus::XMLHandler
{
public:
	ResourceArt_xmlHandler(ResourceManager& resMan) : _resManager(resMan){}
	virtual ~ResourceArt_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
	}

	virtual void elementEnd(const Cactus::String& element)
	{
	}

	virtual void text(const Cactus::String& content){}

	ResourceManager&	_resManager;
};

//---------------------------------------------------------------------------------------------------------

class ResourceGameObject_xmlHandler : public Cactus::XMLHandler
{
public:
	ResourceGameObject_xmlHandler(ResourceManager& resMan) : _resManager(resMan){}
	virtual ~ResourceGameObject_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{

	}

	virtual void elementEnd(const Cactus::String& element)
	{

	}

	virtual void text(const Cactus::String& content){}

	ResourceManager&	_resManager;
};

//---------------------------------------------------------------------------------------------------------

class ResourceGameEvent_xmlHandler : public Cactus::XMLHandler
{
public:
	ResourceGameEvent_xmlHandler(ResourceManager& resMan) : _resManager(resMan){}
	virtual ~ResourceGameEvent_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{

	}

	virtual void elementEnd(const Cactus::String& element)
	{

	}

	virtual void text(const Cactus::String& content){}

	ResourceManager&	_resManager;
};

//---------------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::_LoadResourceArt(const Cactus::String& strPathName)
{
	ResourceArt_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, strPathName, "");
	}
	catch (std::exception e)
	{
		LogN_Error( Logic, "ResourceManager::_LoadResourceArt, Load failed! File : " << strPathName );
		return false;
	}

	return true;
}

bool ResourceManager::_LoadResourceGameObject(const Cactus::String& strPathName)
{
	ResourceGameObject_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, strPathName, "");
	}
	catch (std::exception e)
	{
		LogN_Error( Logic, "ResourceManager::_LoadResourceGameObject, Load failed! File : " << strPathName );
		return false;
	}

	return true;
}

bool ResourceManager::_LoadResourceGameEvent(const Cactus::String& strPathName)
{
	ResourceGameEvent_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, strPathName, "");
	}
	catch (std::exception e)
	{
		LogN_Error( Logic, "ResourceManager::_LoadResourceGameEvent, Load failed! File : " << strPathName );
		return false;
	}

	return true;
}


bool ResourceManager::Load(const Cactus::String& strRootPath)
{
	Cactus::String strPathName;

	strPathName = strRootPath + "Editor/ResourceArt.xml";
	bool b = _LoadResourceArt(strPathName);
	if (!b)
	{
		Reset();
		return false;
	}

	strPathName = strRootPath + "Editor/ResourceGameObject.xml";
	b = _LoadResourceGameObject(strPathName);
	if (!b)
	{
		Reset();
		return false;
	}

	strPathName = strRootPath + "Editor/ResourceGameEvent.xml";
	b = _LoadResourceGameEvent(strPathName);
	if (!b)
	{
		Reset();
		return false;
	}

	return true;
}

void delete_resourceBackground(std::pair<Cactus::String, ResourceBackground*> p)
{
	delete p.second;
}

void delete_resourceTile(std::pair<Cactus::String, ResourceTile*> p)
{
	delete p.second;
}

void delete_resourceGameObject(std::pair<Cactus::String, ResourceGameObject*> p)
{
	delete p.second;
}

void ResourceManager::Reset()
{
	for_each(_ResBackgrounds.begin(), _ResBackgrounds.end(), delete_resourceBackground);
	_ResBackgrounds.clear();

	for_each(_ResTiles.begin(), _ResTiles.end(), delete_resourceTile);
	_ResTiles.clear();


	for_each(_ResGameObjectNPC.begin(), _ResGameObjectNPC.end(), delete_resourceGameObject);
	_ResGameObjectNPC.clear();

	for_each(_ResGameObjectMonster.begin(), _ResGameObjectMonster.end(), delete_resourceGameObject);
	_ResGameObjectMonster.clear();

	for_each(_ResGameObjectFunctionPoint.begin(), _ResGameObjectFunctionPoint.end(), delete_resourceGameObject);
	_ResGameObjectFunctionPoint.clear();

	for_each(_ResGameObjectEvent.begin(), _ResGameObjectEvent.end(), delete_resourceGameObject);
	_ResGameObjectEvent.clear();
}
