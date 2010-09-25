#include "StdAfx.h"
#include "ResourceBackground.h"
#include "ResourceManager.h"

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

	pProp = M_RegisterPropertySimple(Cactus::String, Name, ResourceBackground, Resource, "ͼ���ļ�.", BaseProperty::eReadOnly, _strName);

	pProp = M_RegisterPropertySimple(Cactus::String, ImagePathName, ResourceBackground, Resource, "ͼ���ļ�.", BaseProperty::eDefault, _strImagePathName);
	pProp->SetValueSpecify(eFilePathName, "");

	pProp = M_RegisterPropertySimple(int, ImageWidth, ResourceBackground, Resource, "ͼ�����.", BaseProperty::eReadOnly, _iWidth);
	pProp = M_RegisterPropertySimple(int, ImageHeight, ResourceBackground, Resource, "ͼ��߶�.", BaseProperty::eReadOnly, _iHeight);

}

void ResourceBackground::OnPropertyChanged(const std::string& propName)
{
}


bool ResourceBackground::Load()
{
	String strFull = ResourceManager::getSingleton().GetRootFolder() + _strImagePathName;

	if( _image.Load(_strImagePathName.c_str()) )
	{
		_iWidth		= _image.GetWidth();
		_iHeight	= _image.GetHeight();

		return true;
	}

	return true;
}
