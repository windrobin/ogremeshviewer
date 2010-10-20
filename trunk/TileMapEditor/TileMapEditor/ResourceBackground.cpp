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

	pProp = M_RegisterPropertySimple(Cactus::String, Name, ResourceBackground, Resource, "ͼ���ļ�.", BaseProperty::eReadOnly, _strName);

	pProp = M_RegisterPropertySimple(Cactus::String, ImagePathName, ResourceBackground, Resource, "ͼ���ļ�.", BaseProperty::eReadOnly, _strImagePathName);
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

	BOOL b = _imageList.Create(_iIconSize, _iIconSize, ILC_COLOR32, 0, 4);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	_pBmp = new CBitmap;
	_pBmp->CreateCompatibleBitmap(pDC, _iIconSize, _iIconSize);
	CBitmap* bmpOld = dcMem.SelectObject(_pBmp);

	dcMem.FillSolidRect(0, 0, _iIconSize, _iIconSize, RGB(255, 255, 255));

	int iW = _image.GetWidth();
	int iH = _image.GetHeight();

	if (__max(iW, iH) > _iIconSize)
	{
		if (iW > iH)
		{
			float fR = 1.0f * _iIconSize / iW;
			_image.Draw(dcMem.GetSafeHdc(), 0, 0, _iIconSize, iH * fR, 0, true);
		}
		else
		{
			float fR = 1.0f * _iIconSize / iH;
			_image.Draw(dcMem.GetSafeHdc(), (_iIconSize - fR * iW)/2, 0, fR * iW, _iIconSize, 0, true);
		}
	}
	else
	{
		_image.Draw(dcMem.GetSafeHdc(), (_iIconSize - iW)/2, 0, -1, -1, 0, true);
	}

	dcMem.SelectObject(bmpOld);

	//HBITMAP hBmp = _image.MakeBitmap(pDC->GetSafeHdc());
	//_pBmp = new CBitmap;
	//_pBmp->Attach(hBmp);

	_imageList.Add(_pBmp, (CBitmap*)0);
	_captions.push_back(_strName);

	_bHasImageList = true;
}