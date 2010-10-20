#include "StdAfx.h"
#include "ResourceTile.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

ResourceTile::ResourceTile()
{
	_bHasImageList = false;
}

ResourceTile::~ResourceTile()
{
	for (IDBitmapMapType::iterator it = _BitmapTiles.begin(); it != _BitmapTiles.end(); ++it)
	{
		if (it->second)
		{
			it->second->DeleteObject();
			delete it->second;
		}
	}
}

void ResourceTile::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, ResourceName, ResourceTile, Resource, "资源的名称.", BaseProperty::eDefault, _strName);
	pProp = M_RegisterPropertySimple(int, TileCount, ResourceTile, Resource, "tile数量.", BaseProperty::eDefault, _tilesCount);
}

void ResourceTile::OnPropertyChanged(const std::string& propName)
{
}

bool ResourceTile::IsResItemValid(const Cactus::String& strName)
{
	return ( _BitmapTiles.find(strName) != _BitmapTiles.end() );
}
