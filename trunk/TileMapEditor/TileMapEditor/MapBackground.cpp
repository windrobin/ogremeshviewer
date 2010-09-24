#include "StdAfx.h"
#include "MapBackground.h"

using namespace Cactus;
using namespace PropertySys;

MapBackground::MapBackground()
: _ePaintMode(ePaintModeNormal)
{
}

MapBackground::~MapBackground()
{
}

void MapBackground::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, ResourceName, MapBackground, Map, "Background资源名.", BaseProperty::eDefault, _strResKey);

	pProp = M_RegisterPropertySimple(int, ImageWidth, MapBackground, Map, "绘制模式.", BaseProperty::eDefault, _ePaintMode);
	pProp->SetValueSpecify(eValueList, "0;1;2");

	pProp = M_RegisterPropertySimple(bool, Enable, MapBackground, Map, "是否激活，不激活将不会显示和导出.", BaseProperty::eDefault, _bEnable);
}

void MapBackground::OnPropertyChanged(const std::string& propName)
{
}
