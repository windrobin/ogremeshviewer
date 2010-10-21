#pragma once

#include "ResourceTile.h"

//ԭʼͼ����Դ��һ�Ŵ�ͼ���У�����Դ����Ϊ����С��;
//����Դ�������Ǳ��0 - N-1ת���ɵ�String
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
	Cactus::String		_strImageName;	//ԭʼͼ���ļ���
	int					_imageWidth;	//ԭʼͼ����
	int					_imageHeight;	//ԭʼͼ��߶�
	int					_tileWidth;		//С��Ŀ��
	int					_tileHeight;	//С��ĸ߶�

	CxImage				_image;			//�����ڴ��ԭʼͼ�����
};
