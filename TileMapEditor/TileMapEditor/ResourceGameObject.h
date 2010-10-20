#pragma once

#include "ResourceType.h"

//��Ϸ������Դ
class ResourceGameObject
{
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceGameObjectGroup;
public:
	ResourceGameObject();
	~ResourceGameObject();

	//���ض���
	virtual bool		Load(const Cactus::String& strTile);

protected:
	Cactus::String		_strName;		//����
	Cactus::String		_ArtResID;		//��������ͼ����Դ����
	CPoint				_ptOffset;		//����λ��(xOffset, yOffset)��ͼ�����Ͻ��붨λ���ƫ���λ�����ڵĸ�����(0, 0)
	float				_xBaryCentric;	//�����赲��ľ�ֵ
	float				_yBaryCentric;

	typedef Cactus::list<CPoint>::type	ObstacleListType;
	ObstacleListType	_obstacle;		//���е��赲��

	EGameObjectType		_eType;			//GameObject����
};

//--------------------------------------------------------------------------------------------------------
//��Ϸ�����飬һ����Ϸ������ֻ������һ��ͼ����Դ����
class ResourceGameObjectGroup : public PropertySys::SupportRTTI<ResourceGameObjectGroup, Resource>
{
	friend class ResourceManager;
	friend class ResourceGameObject_xmlHandler;
public:
	ResourceGameObjectGroup();
	~ResourceGameObjectGroup();

	virtual bool			Load(){ return _ResGameObjects.size() != 0; }
	virtual void			CreateImageList(CDC* pDC);
	virtual void			Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

protected:
	typedef Cactus::list<ResourceGameObject*>::type		ResGameObjectListType;
	ResGameObjectListType	_ResGameObjects;

	Cactus::String			_strArtResKey;	//������ͼ����Դ����
	CPoint					_szUnitTile;	//��Ԫ��Ĵ�С
};


