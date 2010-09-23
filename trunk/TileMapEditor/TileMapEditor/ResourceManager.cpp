#include "StdAfx.h"
#include "ResourceManager.h"

#include "ResourceBackground.h"
#include "ResourceTile.h"

#include "ResourceGameObject.h"

using namespace Cactus;

ResourceManager* ResourceManager::Singleton<ResourceManager>::s_pSingleton = 0;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}


bool ResourceManager::Load(const Cactus::String& strRootPath)
{

	return true;
}
