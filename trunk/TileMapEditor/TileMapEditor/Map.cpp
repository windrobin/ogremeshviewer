#include "StdAfx.h"
#include "Map.h"

using namespace Cactus;
using namespace PropertySys;

Map::Map()
{
}

Map::~Map()
{
}

void Map::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, Map, Map, "��ͼ��.", BaseProperty::eDefault, _strName);
	pProp = M_RegisterPropertySimple(Cactus::String, Footnotes, Map, Map, "��ͼ��ע.", BaseProperty::eDefault, _strFootnotes);

	pProp = M_RegisterPropertySimple(int, Width, Map, Map, "��ͼ���.", BaseProperty::eDefault, _iWidth);
	pProp = M_RegisterPropertySimple(int, Height, Map, Map, "��ͼ�߶�.", BaseProperty::eDefault, _iHeight);
	pProp = M_RegisterPropertySimple(int, DefaultTileWidth, Map, Map, "ȱʡ��Tile���.", BaseProperty::eDefault, _iTileWidthDefault);
	pProp = M_RegisterPropertySimple(int, DefaultTileHeight, Map, Map, "ȱʡ��Tile��.", BaseProperty::eDefault, _iTileHeightDefault);

	//pProp = M_RegisterPropertySimple(Ogre::String, FSAA, OgreFramework, Rendering, "Full screen anti aliasing.", BaseProperty::eDefault, _strFSAA);
	//pProp->SetValueSpecify(eValueList, "0;2;4;8;16");
}

void Map::OnPropertyChanged(const std::string& propName)
{
}
