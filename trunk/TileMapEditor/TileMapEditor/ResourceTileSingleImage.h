#pragma once

#include "ResourceTile.h"

class ResourceTileSingleImage : public PropertySys::SupportRTTI<ResourceTileSingleImage, ResourceTile>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTileSingleImage();
	~ResourceTileSingleImage();

	virtual bool		Load();
	virtual void		CreateImageList(CDC* pDC);
	virtual void		Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID);

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strImageName;
	int					_imageWidth;
	int					_imageHeight;
	int					_tileWidth;
	int					_tileHeight;

	CxImage				_image;
};
