#pragma once

#include "ResourceType.h"

class ResourceBackground : public PropertySys::SupportRTTI<ResourceBackground, Resource>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceBackground();
	~ResourceBackground();

	virtual bool		Load();
	virtual void		CreateImageList(CDC* pDC);
	virtual void		Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID){}

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strName;
	Cactus::String		_strImagePathName;
	int					_iWidth;
	int					_iHeight;

	CxImage				_image;

	CBitmap*			_pBmp;
};
