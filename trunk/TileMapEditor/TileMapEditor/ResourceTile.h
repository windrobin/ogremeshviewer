#pragma once

#include "ResourceType.h"

//ӵ�ж��ͼ������Դ
class ResourceTile: public PropertySys::SupportRTTI<ResourceTile, Resource>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTile();
	~ResourceTile();

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	//����Դ�Ƿ���Ч
	virtual bool			IsResItemValid(const Cactus::String& strName);

protected:
	int					_tilesCount;	//����Դ������

	typedef Cactus::map<Cactus::String, CBitmap*>::type		IDBitmapMapType;
	IDBitmapMapType		_BitmapTiles;	//��������Դͼ���б���м�CBitmap����
};

