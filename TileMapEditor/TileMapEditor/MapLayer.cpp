#include "StdAfx.h"
#include "MapLayer.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

MapLayer::MapLayer()
{
}

MapLayer::~MapLayer()
{
}


void MapLayer::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, MapLayer, Map, "Layer��.", BaseProperty::eDefault, _strName);

	pProp = M_RegisterPropertySimple(int, Width, MapLayer, Map, "��ȣ����ɳ�������Map�Ŀ��.", BaseProperty::eDefault, _iWidth);
	pProp = M_RegisterPropertySimple(int, Height, MapLayer, Map, "�߶ȣ����ɳ�������Map�ĸ߶�.", BaseProperty::eDefault, _iHeight);
	pProp = M_RegisterPropertySimple(int, TileWidth, MapLayer, Map, "����ģʽ.", BaseProperty::eDefault, _iTileWidth);
	pProp = M_RegisterPropertySimple(int, TileHeight, MapLayer, Map, "����ģʽ.", BaseProperty::eDefault, _iTileHeight);
	pProp = M_RegisterPropertySimple(bool, Enable, MapLayer, Map, "�Ƿ񼤻�����������ʾ�͵���.", BaseProperty::eDefault, _bEnable);
	
}

void MapLayer::OnPropertyChanged(const std::string& propName)
{
}

void MapLayer::Draw(CDC* pDC)
{
	if (!_bEnable)
		return;

	for (TileGroupMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		Resource* pRes = ResourceManager::getSingleton().GetResource(it->first);
		if (pRes)
		{
			//pRes->Draw();
		}
	}
}
