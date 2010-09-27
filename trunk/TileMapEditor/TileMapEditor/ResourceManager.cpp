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

				if( p->Load() && _resManager._ResBackgrounds.find(p->_strName) == _resManager._ResBackgrounds.end() )
				{
					_resManager._ResBackgrounds[p->_strName] = p;
				}
				else
				{
					Log_Error("ResourceArt_xmlHandler::elementStart, ResourceBackground load failed! " << p->_strImagePathName);
					delete p;
				}
			}
			else if (_bInTiles)
			{
				String strType	= attributes.getValueAsString("type");
				if (strType == "image")
				{
					//<item type="image" name="tile1" file="art/tiles/tile1.png" tilew="32" tileh="32" count="80"/>
					ResourceTileSingleImage* p = new ResourceTileSingleImage;
					p->_strName			= attributes.getValueAsString("name");
					p->_tileWidth		= attributes.getValueAsInteger("tilew");
					p->_tileHeight		= attributes.getValueAsInteger("tileh");
					p->_tilesCount		= attributes.getValueAsInteger("count");
					p->_strImageName	= attributes.getValueAsString("file");

					if( p->Load() && _resManager._ResTiles.find(p->_strName) == _resManager._ResTiles.end() )
					{
						_resManager._ResTiles[p->_strName] = p;
					}
					else
					{
						Log_Error("ResourceArt_xmlHandler::elementStart, ResourceTileSingleImage load failed! " << p->_strName);
						delete p;
					}
				}
				else if (strType == "folder")
				{
					//<item type="folder" name="tile2" folder="art/tiles/tile2/" tilew="32" tileh="32" count="8" bits="2" ext=".png" />
					ResourceTileFolder* p = new ResourceTileFolder;
					p->_strName			= attributes.getValueAsString("name");
					p->_tileWidth		= attributes.getValueAsInteger("tilew");
					p->_tileHeight		= attributes.getValueAsInteger("tileh");
					p->_tilesCount		= attributes.getValueAsInteger("count");
					p->_strFolderName	= attributes.getValueAsString("folder");
					p->_strFileExt		= attributes.getValueAsString("ext");
					p->_iBits			= attributes.getValueAsInteger("bits");

					if( p->Load() && _resManager._ResTiles.find(p->_strName) == _resManager._ResTiles.end() )
					{
						_resManager._ResTiles[p->_strName] = p;
					}
					else
					{
						Log_Error("ResourceArt_xmlHandler::elementStart, ResourceTileFolder load failed! " << p->_strName);
						delete p;
					}
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
		pGameObjectGroup = 0;
	}
	virtual ~ResourceGameObject_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if (element == "npc")
		{
			_eType		= eGameObjectNPC;
			_strType	= element;
			pGameObjectGroup = new ResourceGameObjectGroup;
			pGameObjectGroup->_strArtResKey = attributes.getValueAsString("iconres");
		}
		else if (element == "monster")
		{
			_eType		= eGameObjectMonster;
			_strType	= element;
			pGameObjectGroup = new ResourceGameObjectGroup;
			pGameObjectGroup->_strArtResKey = attributes.getValueAsString("iconres");
		}
		else if (element == "funcpoint")
		{
			_eType		= eGameObjectFunctionPoint;
			_strType	= element;
			pGameObjectGroup = new ResourceGameObjectGroup;
			pGameObjectGroup->_strArtResKey = attributes.getValueAsString("iconres");
		}
		else if (element == "item" && _eType != eGameObjectMax)
		{
			//<item name="npc01" iconres="tile1" iconid="15" />
			ResourceGameObject* p = new ResourceGameObject;
			p->_strName			= attributes.getValueAsString("name");
			p->_ArtResID		= attributes.getValueAsInteger("iconid");
		
			p->_eType			= _eType;

			if (p->Load(pGameObjectGroup->_strArtResKey))
			{
				pGameObjectGroup->_ResGameObjects.push_back(p);
			}
			else
			{
				delete p;
			}
		}
	}

	virtual void elementEnd(const Cactus::String& element)
	{
		if (element == "npc"
			|| element == "monster"
			|| element == "funcpoint"
			)
		{
			if (pGameObjectGroup->Load())
				_resManager._ResGameObjectGroups[_strType] = pGameObjectGroup;
			else
				delete pGameObjectGroup;

			pGameObjectGroup = 0;
			_eType	= eGameObjectMax;
		}
	}

	virtual void text(const Cactus::String& content){}

private:

	ResourceManager&	_resManager;
	GameObjectType		_eType;
	String				_strType;
	ResourceGameObjectGroup*	pGameObjectGroup;
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
		Log_Error("ResourceManager::_LoadResourceArt, Load failed! File : " << strPathName );
		return false;
	}

	Log_Info("ResourceManager::_LoadResourceArt, Load OK.");

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
		Log_Error("ResourceManager::_LoadResourceGameObject, Load failed! File : " << strPathName );
		return false;
	}

	Log_Info("ResourceManager::_LoadResourceGameObject, Load OK.");

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
		Log_Error( "ResourceManager::_LoadResourceGameEvent, Load failed! File : " << strPathName );
		return false;
	}

	Log_Info("ResourceManager::_LoadResourceGameEvent, Load OK.");

	return true;
}

bool ResourceManager::Load(const Cactus::String& strRootPath)
{
	_strRootFolder	= strRootPath;

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

void delete_resourceGameObjectGroup(std::pair<Cactus::String, ResourceGameObjectGroup*> p)
{
	delete p.second;
}

void ResourceManager::Reset()
{
	for_each(_ResBackgrounds.begin(), _ResBackgrounds.end(), delete_resourceBackground);
	_ResBackgrounds.clear();

	for_each(_ResTiles.begin(), _ResTiles.end(), delete_resourceTile);
	_ResTiles.clear();

	for_each(_ResGameObjectGroups.begin(), _ResGameObjectGroups.end(), delete_resourceGameObjectGroup);
	_ResGameObjectGroups.clear();
}

bool ResourceManager::IsResTileIDValid(const Cactus::String& tile, int ID)
{
	ResTileType::iterator it = _ResTiles.find(tile);
	if (it != _ResTiles.end())
	{
		ResourceTile* pTile = it->second;
		return ID < pTile->_tilesCount;
	}

	return false;
}

ResourceTile* ResourceManager::GetResourceTile(const Cactus::String& tile)
{
	ResTileType::iterator it = _ResTiles.find(tile);
	if (it != _ResTiles.end())
	{
		return it->second;
	}

	return 0;
}

Resource* ResourceManager::GetResource(const Cactus::String& key)
{
	if (_ResBackgrounds.find(key) != _ResBackgrounds.end())
	{
		return _ResBackgrounds[key];
	}

	if (_ResTiles.find(key) != _ResTiles.end())
	{
		return _ResTiles[key];
	}

	if (_ResGameObjectGroups.find(key) != _ResGameObjectGroups.end())
	{
		return _ResGameObjectGroups[key];
	}
}
