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

	pProp = M_RegisterPropertySimple(Cactus::String, Name, MapLayer, Map, "Layer名.", BaseProperty::eDefault, _strName);

	pProp = M_RegisterPropertySimple(int, Width, MapLayer, Map, "宽度，不可超过所属Map的宽度.", BaseProperty::eDefault, _iWidth);
	pProp = M_RegisterPropertySimple(int, Height, MapLayer, Map, "高度，不可超过所属Map的高度.", BaseProperty::eDefault, _iHeight);
	pProp = M_RegisterPropertySimple(int, TileWidth, MapLayer, Map, "绘制模式.", BaseProperty::eDefault, _iTileWidth);
	pProp = M_RegisterPropertySimple(int, TileHeight, MapLayer, Map, "绘制模式.", BaseProperty::eDefault, _iTileHeight);
}

void MapLayer::OnPropertyChanged(const std::string& propName)
{
}
