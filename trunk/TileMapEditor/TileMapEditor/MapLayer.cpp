#include "StdAfx.h"
#include "MapLayer.h"

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
}

void MapLayer::OnPropertyChanged(const std::string& propName)
{
}
