#pragma once

#include "ResourceTile.h"

//ԭʼ����Դͼ����һ����Ŀ¼�У�����ͼ���������ͬ�ĺ�׺�����ǿ����в�ͬ��С
class ResourceTileFolder : public PropertySys::SupportRTTI<ResourceTileFolder, ResourceTile>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTileFolder();
	~ResourceTileFolder();

	virtual bool		Load();
	virtual void		CreateImageList(CDC* pDC);
	virtual void		Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID);

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strFolderName;		//Ŀ¼��
	Cactus::String		_strFileExt;		//��׺��������'.'

	typedef Cactus::map<Cactus::String, CxImage*>::type	IDImageMapType;
	IDImageMapType		_images;			//�����ڴ����������Դͼ�����
};