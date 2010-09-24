#include "StdAfx.h"
#include "ResourceGameObject.h"

using namespace PropertySys;

ResourceGameObject::ResourceGameObject()
{
}

ResourceGameObject::~ResourceGameObject()
{
}

void ResourceGameObject::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, ResourceGameObject, Resource, "��Դ��.", BaseProperty::eDefault, _strName);
	pProp = M_RegisterPropertySimple(int, Type, ResourceGameObject, Resource, "��Դ����.", BaseProperty::eDefault, _eType);
	pProp->SetValueSpecify(eValueList, "NPC;Monster;FunctionPoint;Event");
	
	pProp = M_RegisterPropertySimple(Cactus::String, ArtResourceKey, ResourceGameObject, Resource, "������ͼ����Դ��.", BaseProperty::eDefault, _strArtResKey);

	pProp = M_RegisterPropertySimple(int, ArtResID, ResourceGameObject, Resource, "ͼ����Դ�е�ID.", BaseProperty::eDefault, _ArtResID);

	//pProp = M_RegisterPropertySimple(Ogre::String, FSAA, OgreFramework, Rendering, "Full screen anti aliasing.", BaseProperty::eDefault, _strFSAA);
	//pProp->SetValueSpecify(eValueList, "0;2;4;8;16");
}

void ResourceGameObject::OnPropertyChanged(const std::string& propName)
{
}