#include "StdAfx.h"
#include "ResourceGameObject.h"
#include "ResourceManager.h"
#include "ResourceTile.h"

using namespace Cactus;
using namespace PropertySys;

ResourceGameObject::ResourceGameObject()
{
}

ResourceGameObject::~ResourceGameObject()
{
}

bool ResourceGameObject::Load(const Cactus::String& strTile)
{
	return ResourceManager::getSingleton().IsResTileIDValid(strTile, _ArtResID);
}

//--------------------------------------------------------------------------------------------------------
ResourceGameObjectGroup::ResourceGameObjectGroup()
{
}

ResourceGameObjectGroup::~ResourceGameObjectGroup()
{
	for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
	{
		delete *it;
	}
	_ResGameObjects.clear();
}

void ResourceGameObjectGroup::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);
	
	pProp = M_RegisterPropertySimple(Cactus::String, GroupName, ResourceGameObjectGroup, Resource, "游戏对象组名.", BaseProperty::eReadOnly, _strGroupName);
	pProp = M_RegisterPropertySimple(Cactus::String, ArtResKey, ResourceGameObjectGroup, Resource, "图标的来源.", BaseProperty::eReadOnly, _strArtResKey);
}

void ResourceGameObjectGroup::OnPropertyChanged(const std::string& propName)
{
}

void ResourceGameObjectGroup::CreateImageList()
{
	if (_bHasImageList)
		return;

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTile(_strArtResKey);
	if (pResTile)
	{
		pResTile->CreateImageList();
		CImageList* pImageList = pResTile->GetImageList();

		_imageList.Create(pImageList);
		_imageList.SetImageCount(0);

		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			_imageList.Copy(_imageList.GetImageCount(), pImageList, (*it)->_ArtResID, ILCF_MOVE);
		}
	}

	_bHasImageList = true;
}
