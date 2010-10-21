#pragma once

#include "ResourceTile.h"

//原始图像资源在一张大图像中，子资源是作为其中小块;
//子资源的命名是编号0 - N-1转换成的String
class ResourceTileSingleImage : public PropertySys::SupportRTTI<ResourceTileSingleImage, ResourceTile>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTileSingleImage();
	~ResourceTileSingleImage();

	virtual bool		Load();
	virtual void		CreateImageList(CDC* pDC);
	virtual void		Draw(CDC* pDC, const CRect& curTile, const Cactus::String& strID);
	virtual CRect		GetResItemBoundingRect(const CRect& curTile, const Cactus::String& strID);

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strImageName;	//原始图像文件名
	int					_imageWidth;	//原始图像宽度
	int					_imageHeight;	//原始图像高度
	int					_tileWidth;		//小块的宽度
	int					_tileHeight;	//小块的高度

	CxImage				_image;			//读入内存的原始图像对象
};
