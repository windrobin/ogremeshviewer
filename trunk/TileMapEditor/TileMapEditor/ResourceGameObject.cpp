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
	//�ж���������ͼ����Դʮ����Ч
	return ResourceManager::getSingleton().IsResourceTileIDValid(strTile, _ArtResID);
}

//--------------------------------------------------------------------------------------------------------
ResourceGameObjectGroup::ResourceGameObjectGroup()
{
	_eResType	= eResTypeGameObject;
	_szUnitTile	= CPoint(64, 64);
}

ResourceGameObjectGroup::ResourceGameObjectGroup(const Cactus::String& strName, const Cactus::String& strArtGroup, CPoint szTile)
{
	_eResType	= eResTypeGameObject;

	_strName		= strName;
	_strArtResKey	= strArtGroup;
	_szUnitTile		= szTile;
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

	pProp = M_RegisterPropertySimple(Cactus::String, ArtResKey, ResourceGameObjectGroup, Resource, "ͼ�����Դ.", BaseProperty::eReadOnly, _strArtResKey);
	pProp = M_RegisterPropertySimple(CPoint, UnitTileSize, ResourceGameObjectGroup, Resource, "��Ԫ��Ĵ�С.", BaseProperty::eReadOnly, _szUnitTile);
}

void ResourceGameObjectGroup::OnPropertyChanged(const std::string& propName)
{
}

void ResourceGameObjectGroup::CreateImageList(CDC* pDC)
{
	if (_bHasImageList)
		return;

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		pResTile->CreateImageList(pDC);
		CImageList* pImageList = pResTile->GetImageList();

		//��ResourceTile��Դ����һ��
		_imageList.Create(pImageList);

		Cactus::StringVector* pCaps = pResTile->GetCaptions();

		size_t iIndex = 0;
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if ((*it)->_ArtResID != (*pCaps)[iIndex])
			{
				for (size_t t = 0; t < (*pCaps).size(); ++t)	//�ҵ����õ�ͼƬ����
				{
					if( (*it)->_ArtResID == (*pCaps)[t] )
					{
						_imageList.Copy(iIndex, t, ILCF_SWAP);	//����λ��
						break;
					}
				}
			}

			iIndex++;

			_captions.push_back( (*it)->_strName );
		}

		int iCount = _imageList.GetImageCount();

		for (int i = iIndex; i < iCount; ++i)
		{
			_imageList.Remove(i);
		}
	}

	_bHasImageList = true;
}

void ResourceGameObjectGroup::Draw(CDC* pDC, const CRect& curTile, const Cactus::String& strItemID)
{
	CreateImageList(pDC);

	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if( (*it)->_strName == strItemID )
			{
				pResTile->Draw(pDC, curTile, (*it)->_ArtResID);
				break;
			}
		}
	}
}

CRect ResourceGameObjectGroup::GetResItemBoundingRect(const CRect& curTile, const Cactus::String& strItemID)
{
	ResourceTile* pResTile = ResourceManager::getSingleton().GetResourceTileGroup(_strArtResKey);
	if (pResTile)
	{
		for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
		{
			if( (*it)->_strName == strItemID )
			{
				return pResTile->GetResItemBoundingRect(curTile, (*it)->_ArtResID);
			}
		}
	}

	return Resource::GetResItemBoundingRect(curTile, strItemID);
}

ResourceGameObject* ResourceGameObjectGroup::GetGameObject(const Cactus::String& strItemID)
{
	for (ResGameObjectListType::iterator it = _ResGameObjects.begin(); it != _ResGameObjects.end(); ++it)
	{
		if( (*it)->_strName == strItemID )
		{
			return *it;
		}
	}

	return 0;
}
