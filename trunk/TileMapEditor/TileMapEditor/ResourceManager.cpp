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
	ResourceArt_xmlHandler(ResourceManager& resMan) : _resManager(resMan)
	{
		_bInBackground	= false;
		_bInTiles		= false;
	}
	virtual ~ResourceArt_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if (element == "background")
		{
			_bInBackground	= true;
		}
		else if (element == "tiles")
		{
			_bInTiles		= true;
		}
		else if (element == "item")
		{
			if (_bInBackground)
			{
				//<item name="bg1" res="art/background/bg1.png"/>
				ResourceBackground* p = new ResourceBackground;
				p->_strName				= attributes.getValueAsString("name");
				p->_strImagePathName	= attributes.getValueAsString("res");

				_resManager._ResBackgrounds[p->_strName] = p;
			}
			else if (_bInTiles)
			{
				String strType	= attributes.getValueAsString("type");
				if (strType == "image")
				{
					//<item type="image" name="tile1" res="art/tiles/tile1.png" tilew="32" tileh="32" count="80"/>
					ResourceTileSingleImage* p = new ResourceTileSingleImage;
					p->_strName			= attributes.getValueAsString("name");
					p->_tileWidth		= attributes.getValueAsInteger("tilew");
					p->_tileHeight		= attributes.getValueAsInteger("tileh");
					p->_tilesCount		= attributes.getValueAsInteger("count");
					p->_strImageName	= attributes.getValueAsString("res");
					_resManager._ResTiles[p->_strName] = p;
				}
				else if (strType == "folder")
				{
					//<item type="folder" name="tile2" res="art/tiles/tile2/" tilew="32" tileh="32" count="80" ext="png" />
					ResourceTileFolder* p = new ResourceTileFolder;
					p->_strName			= attributes.getValueAsString("name");
					p->_tileWidth		= attributes.getValueAsInteger("tilew");
					p->_tileHeight		= attributes.getValueAsInteger("tileh");
					p->_tilesCount		= attributes.getValueAsInteger("count");
					p->_strFolderName	= attributes.getValueAsString("res");
					p->_strFileExt		= attributes.getValueAsString("ext");
					_resManager._ResTiles[p->_strName] = p;
				}
				else
				{
				}
			}
		}
	}

	virtual void elementEnd(const Cactus::String& element)
	{
		if (element == "background")
		{
			_bInBackground	= false;
		}
		else if (element == "tiles")
		{
			_bInTiles		= false;
		}
	}

	virtual void text(const Cactus::String& content){}

private:
	ResourceManager&	_resManager;
	bool				_bInBackground;
	bool				_bInTiles;
};

//---------------------------------------------------------------------------------------------------------

class ResourceGameObject_xmlHandler : public Cactus::XMLHandler
{
public:
	ResourceGameObject_xmlHandler(ResourceManager& resMan) : _resManager(resMan)
	{
		_eType	= eGameObjectMax;
	}
	virtual ~ResourceGameObject_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if (element == "npc")
		{
			_eType	= eGameObjectNPC;
		}
		else if (element == "monster")
		{
			_eType	= eGameObjectMonster;
		}
		else if (element == "funcpoint")
		{
			_eType	= eGameObjectFunctionPoint;
		}
		else if (element == "item" && _eType != eGameObjectMax)
		{
			//<item name="npc01" iconres="tile1" iconid="15" />
			ResourceGameObject* p = new ResourceGameObject;
			p->_strName			= attributes.getValueAsString("name");
			p->_strArtResKey	= attributes.getValueAsString("iconres");
			p->_ArtResID		= attributes.getValueAsInteger("iconid");
		
			p->_eType			= _eType;

			_resManager._ResGameObject[_eType][p->_strName] = p;
		}
	}

	virtual void elementEnd(const Cactus::String& element)
	{
		if (element == "npc")
		{
			_eType	= eGameObjectMax;
		}
		else if (element == "monster")
		{
			_eType	= eGameObjectMax;
		}
		else if (element == "funcpoint")
		{
			_eType	= eGameObjectMax;
		}
	}

	virtual void text(const Cactus::String& content){}

private:

	ResourceManager&	_resManager;
	GameObjectType		_eType;
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
	Reset();
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


	for(int i = 0; i < eGameObjectMax; ++i)
	{
		for_each(_ResGameObject[i].begin(), _ResGameObject[i].end(), delete_resourceGameObject);
		_ResGameObject[i].clear();
	}
}
