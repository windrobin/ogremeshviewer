#include "StdAfx.h"
#include "ResourceBackground.h"

using namespace Cactus;
using namespace PropertySys;

ResourceBackground::ResourceBackground()
: _iWidth(0)
, _iHeight(0)
{
}

ResourceBackground::~ResourceBackground()
{
}

void ResourceBackground::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, ResourceBackground, Resource, "图像文件.", BaseProperty::eReadOnly, _strName);

	pProp = M_RegisterPropertySimple(Cactus::String, ImagePathName, ResourceBackground, Resource, "图像文件.", BaseProperty::eDefault, _strImagePathName);
	pProp->SetValueSpecify(eFilePathName, "");

	pProp = M_RegisterPropertySimple(int, ImageWidth, ResourceBackground, Resource, "图像宽度.", BaseProperty::eReadOnly, _iWidth);
	pProp = M_RegisterPropertySimple(int, ImageHeight, ResourceBackground, Resource, "图像高度.", BaseProperty::eReadOnly, _iHeight);

	//pProp = M_RegisterPropertySimple(Ogre::String, FSAA, OgreFramework, Rendering, "Full screen anti aliasing.", BaseProperty::eDefault, _strFSAA);
	//pProp->SetValueSpecify(eValueList, "0;2;4;8;16");
}

void ResourceBackground::OnPropertyChanged(const std::string& propName)
{
}
