#include "StdAfx.h"
#include "Map.h"

using namespace Cactus;
using namespace PropertySys;

//---------------------------------------------------------------------------------------------------------

class Map_xmlHandler : public Cactus::XMLHandler
{
public:
	Map_xmlHandler(Map& map) : _map(map){}
	virtual ~Map_xmlHandler(){}

	virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
	{

	}

	virtual void elementEnd(const Cactus::String& element)
	{

	}

	virtual void text(const Cactus::String& content){}

	Map&	_map;
};

//---------------------------------------------------------------------------------------------------------

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

	pProp = M_RegisterPropertySimple(Cactus::String, Name, Map, Map, "地图名.", BaseProperty::eDefault, _strName);
	pProp = M_RegisterPropertySimple(Cactus::String, Footnotes, Map, Map, "地图备注.", BaseProperty::eDefault, _strFootnotes);

	pProp = M_RegisterPropertySimple(int, Width, Map, Map, "地图宽度.", BaseProperty::eDefault, _iWidth);
	pProp = M_RegisterPropertySimple(int, Height, Map, Map, "地图高度.", BaseProperty::eDefault, _iHeight);
	pProp = M_RegisterPropertySimple(int, DefaultTileWidth, Map, Map, "缺省的Tile宽度.", BaseProperty::eDefault, _iTileWidthDefault);
	pProp = M_RegisterPropertySimple(int, DefaultTileHeight, Map, Map, "缺省的Tile高.", BaseProperty::eDefault, _iTileHeightDefault);

	//pProp = M_RegisterPropertySimple(Ogre::String, FSAA, OgreFramework, Rendering, "Full screen anti aliasing.", BaseProperty::eDefault, _strFSAA);
	//pProp->SetValueSpecify(eValueList, "0;2;4;8;16");
}

void Map::OnPropertyChanged(const std::string& propName)
{
}

bool Map::Load(const Cactus::String& strPathName)
{
	Map_xmlHandler handler(*this);

	try
	{
		XMLParser xmlParser(handler, strPathName, "");
	}
	catch (std::exception e)
	{
		LogN_Error( Logic, "Map::Load, Load failed! File : " << strPathName );
		return false;
	}

	return true;
}
