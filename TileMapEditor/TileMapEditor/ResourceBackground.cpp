#include "StdAfx.h"
#include "ResourceBackground.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

ResourceBackground::ResourceBackground()
: _iWidth(0)
, _iHeight(0)
, _pBmp(0)
{
}

ResourceBackground::~ResourceBackground()
{
	if (_pBmp)
	{
		_pBmp->DeleteObject();
		delete _pBmp;
	}
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

}

void ResourceBackground::OnPropertyChanged(const std::string& propName)
{
}

bool ResourceBackground::Load()
{
	String strFull = ResourceManager::getSingleton().GetRootFolder() + _strImagePathName;

	if( _image.Load(strFull.c_str()) )
	{
		_iWidth		= _image.GetWidth();
		_iHeight	= _image.GetHeight();

		return true;
	}

	return false;
}

void ResourceBackground::CreateImageList(CDC* pDC)
{
	if (_bHasImageList)
		return;

	//_image.GetColorType()

	BOOL b = _imageList.Create(_iWidth, _iHeight, ILC_COLOR32, 0, 4);

	HBITMAP hBmp = _image.MakeBitmap(pDC->GetSafeHdc());
	_pBmp = new CBitmap;
	_pBmp->Attach(hBmp);

	_imageList.Add(_pBmp, RGB(0, 0, 0));
	_captions.push_back(_strName);

	_bHasImageList = true;
}