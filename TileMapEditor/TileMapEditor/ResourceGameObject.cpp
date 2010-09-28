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

void ResourceGameObjectGroup::CreateImageList(CDC* pDC)
{
	if (_bHasImageList)
		return;

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTile(_strArtResKey);
	if (pResTile)
	{
		pResTile->CreateImageList(pDC);
		CImageList* pImageList = pResTile->GetImageList();

		_imageList.Create(pImageList);

		Cactus::map<int, ResourceGameObject*>::type IDGamObject;
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			IDGamObject[(*it)->_ArtResID] = *it;
		}

		int iIndex = 0;
		for (Cactus::map<int, ResourceGameObject*>::type::iterator it = IDGamObject.begin(); it != IDGamObject.end(); ++it)
		{
			if (it->first != iIndex)
			{
				_imageList.Copy(iIndex, it->first, ILCF_SWAP);
			}

			iIndex++;

			_captions.push_back( it->second->_strName );
		}

		int iCount = _imageList.GetImageCount();

		for (int i = iIndex; i < iCount; ++i)
		{
			_imageList.Remove(i);
		}
	}

	_bHasImageList = true;
}

void ResourceGameObjectGroup::Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID)
{
	CreateImageList(pDC);

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTile(_strArtResKey);
	if (pResTile)
	{
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if( (*it)->_strName == strID )
			{
				Cactus::ostringstream os;
				os << (*it)->_ArtResID;
				pResTile->Draw(pDC, posX, posY, os.str());

				break;
			}
		}
	}
}
