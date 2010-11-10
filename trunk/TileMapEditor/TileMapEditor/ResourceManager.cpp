#include "StdAfx.h"
#include "ResourceManager.h"

#include "ResourceBackground.h"
#include "ResourceTile.h"
#include "ResourceTileSingleImage.h"
#include "ResourceTileFolder.h"
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
				p->_iIconSize			= attributes.getValueAsInteger("iconsize");

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
					p->_iIconSize		= attributes.getValueAsInteger("iconsize");

					if( p->Load() && _resManager._ResourceTiles.find(p->_strName) == _resManager._ResourceTiles.end() )
					{
						_resManager._ResourceTiles[p->_strName] = p;
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
					p->_strFolderName	= attributes.getValueAsString("folder");
					p->_strFileExt		= attributes.getValueAsString("ext");
					p->_iIconSize		= attributes.getValueAsInteger("iconsize");

					if( p->Load() && _resManager._ResourceTiles.find(p->_strName) == _resManager._ResourceTiles.end() )
					{
						_resManager._ResourceTiles[p->_strName] = p;
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
		pGameObjectGroup	= 0;
		pGameObject			= 0;
	}
	virtual ~ResourceGameObject_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{
		if ( element == "group")
		{
			//<group name="npc" iconres="tile2" unitTileW="100" unitTileH="50">
			pGameObjectGroup = new ResourceGameObjectGroup;
			pGameObjectGroup->_strName		= attributes.getValueAsString("name");
			pGameObjectGroup->_strArtResKey = attributes.getValueAsString("iconres");
			pGameObjectGroup->_eGridType	= EGridType(attributes.getValueAsInteger("mapType"));
			pGameObjectGroup->_szUnitTile.x = attributes.getValueAsInteger("unitTileW");
			pGameObjectGroup->_szUnitTile.y = attributes.getValueAsInteger("unitTileH");
		}
		else if (element == "item")
		{
			//<item name="npc01" iconid="000" xOffset="25" yOffset="16" xBarycentric="0" yBarycentric="0" >
			//	<obstacle x="0" y="0"/>
			//	<obstacle x="0" y="1"/>
			//	<obstacle x="0" y="-1"/>
			//</item>
			ResourceGameObject* p = new ResourceGameObject;
			p->_strName			= attributes.getValueAsString("name");
			p->_ArtResID		= attributes.getValueAsString("iconid");
			p->_ptOffset.x		= attributes.getValueAsInteger("xOffset");
			p->_ptOffset.y		= attributes.getValueAsInteger("yOffset");
			p->_xBaryCentric	= attributes.getValueAsFloat("xBarycentric");
			p->_yBaryCentric	= attributes.getValueAsFloat("yBarycentric");
			p->_strAIType		= attributes.getValueAsString("AIType");
		
			if (p->Load(pGameObjectGroup->_strArtResKey))
			{
				pGameObjectGroup->_ResGameObjects.push_back(p);

				pGameObject = p;
			}
			else
			{
				delete p;
			}
		}
		else if (element == "obstacle")
		{
			if (pGameObject)
			{
				CPoint pt;
				pt.x	= attributes.getValueAsInteger("x");
				pt.y	= attributes.getValueAsInteger("y");
				pGameObject->_obstacles.push_back(pt);
			}
		}
	}

	virtual void elementEnd(const Cactus::String& element)
	{
		if (element == "group")
		{
			if (pGameObjectGroup->Load())
				_resManager._ResGameObjectGroups[pGameObjectGroup->_strName] = pGameObjectGroup;
			else
				delete pGameObjectGroup;

			pGameObjectGroup = 0;
		}
		else if (element == "item")
		{
			pGameObject = 0;
		}
	}

	virtual void text(const Cactus::String& content){}

private:

	ResourceManager&			_resManager;
	ResourceGameObjectGroup*	pGameObjectGroup;
	ResourceGameObject*			pGameObject;
};

//---------------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	String strPathName = _strRootFolder + "Editor/ResourceGameObject.xml";

	_SaveResourceGameObject(strPathName);

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

	for_each(_ResourceTiles.begin(), _ResourceTiles.end(), delete_resourceTile);
	_ResourceTiles.clear();

	for_each(_ResGameObjectGroups.begin(), _ResGameObjectGroups.end(), delete_resourceGameObjectGroup);
	_ResGameObjectGroups.clear();
}

bool ResourceManager::IsResourceTileIDValid(const Cactus::String& tile, const Cactus::String& strID)
{
	ResTileType::iterator it = _ResourceTiles.find(tile);
	if (it != _ResourceTiles.end())
	{
		ResourceTile* pTile = it->second;
		return pTile->IsResItemValid(strID);
	}

	return false;
}

ResourceTile* ResourceManager::GetResourceTileGroup(const Cactus::String& strGroupName)
{
	ResTileType::iterator it = _ResourceTiles.find(strGroupName);
	if (it != _ResourceTiles.end())
	{
		return it->second;
	}

	return 0;
}

Resource* ResourceManager::GetResourceArtGroup(const Cactus::String& key)
{
	if (_ResBackgrounds.find(key) != _ResBackgrounds.end())
	{
		return _ResBackgrounds[key];
	}

	if (_ResourceTiles.find(key) != _ResourceTiles.end())
	{
		return _ResourceTiles[key];
	}

	return 0;
}

Resource* ResourceManager::GetResourceGameObjectGroup(const Cactus::String& key)
{
	if (_ResGameObjectGroups.find(key) != _ResGameObjectGroups.end())
	{
		return _ResGameObjectGroups[key];
	}

	return 0;
}

Cactus::StringVector ResourceManager::GetResourceTileNames()
{
	StringVector vs;

	for (ResTileType::iterator it = _ResourceTiles.begin(); it != _ResourceTiles.end(); ++it)
	{
		vs.push_back(it->first);
	}

	return vs;
}

bool ResourceManager::IsGameObjectGroupNameValid(const Cactus::String& strName)
{
	return (_ResGameObjectGroups.find(strName) == _ResGameObjectGroups.end());
}

bool ResourceManager::AddGameObjectGroup(ResourceGameObjectGroup* p)
{
	_ResGameObjectGroups[p->_strName] = p;

	return true;
}


bool ResourceManager::RemoveGameObjectGroup(const Cactus::String& strName)
{

	ResGameObjectGroupMapType::iterator it = _ResGameObjectGroups.find(strName);
	if(it != _ResGameObjectGroups.end())
	{
		delete it->second;
		_ResGameObjectGroups.erase(it);
		
		return true;
	}

	return false;
}

void ResourceManager::_SaveResourceGameObject(const Cactus::String& strPathName)
{
	DataOutStreamOS os;
	if( !os.Open(strPathName) )
	{
		Log_Error("ResourceManager::_SaveResourceGameObject, can not write file : " << strPathName);
		return;
	}

	String str = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n";
	//os << str;	//这样会先写个长度
	os.Write(&str[0], str.size());

	XMLOutStream xmlOut(&os);

	xmlOut.NodeBegin("resoucegameobject");

	ResGameObjectGroupMapType::iterator it = _ResGameObjectGroups.begin();
	for(; it != _ResGameObjectGroups.end(); ++it)
	{
		ResourceGameObjectGroup* pGOGroup = it->second;
		pGOGroup->Save(xmlOut);
	}
	xmlOut.NodeEnd("resoucegameobject");

	xmlOut.Flush();
}