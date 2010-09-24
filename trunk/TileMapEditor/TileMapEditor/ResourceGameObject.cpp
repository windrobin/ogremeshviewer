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

	pProp = M_RegisterPropertySimple(Cactus::String, Name, ResourceGameObject, Resource, "资源名.", BaseProperty::eDefault, _strName);
	pProp = M_RegisterPropertySimple(int, Type, ResourceGameObject, Resource, "资源类型.", BaseProperty::eDefault, _eType);
	pProp->SetValueSpecify(eValueList, "NPC;Monster;FunctionPoint;Event");
	
	pProp = M_RegisterPropertySimple(Cactus::String, ArtResourceKey, ResourceGameObject, Resource, "依赖的图像资源名.", BaseProperty::eDefault, _strArtResKey);

	pProp = M_RegisterPropertySimple(int, ArtResID, ResourceGameObject, Resource, "图像资源中的ID.", BaseProperty::eDefault, _ArtResID);

}

void ResourceGameObject::OnPropertyChanged(const std::string& propName)
{
}
