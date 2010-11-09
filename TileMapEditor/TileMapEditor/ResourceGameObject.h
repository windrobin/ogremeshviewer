#pragma once

#include "ResourceType.h"

//��Ϸ������Դ
class ResourceGameObject
{
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceGameObjectGroup;
	friend class GameObjectEditor;
	friend class CDialogGameObject;
public:
	ResourceGameObject();
	~ResourceGameObject();

	//���ض���
	virtual bool		Load(const Cactus::String& strTile);
	void				Save(Cactus::XMLOutStream& xmlOut);
	void				CalculateBaryCentric();

protected:
	Cactus::String		_strName;		//����
	Cactus::String		_ArtResID;		//��������ͼ����Դ����
	CPoint				_ptOffset;		//����λ��(xOffset, yOffset)��ͼ�����Ͻ��붨λ���ƫ���λ�����ڵĸ�����(0, 0)
	float				_xBaryCentric;	//�����赲��ľ�ֵ
	float				_yBaryCentric;

	ObstacleListType	_obstacles;		//���е��赲��

	//EGameObjectType		_eType;			//GameObject����
};

//--------------------------------------------------------------------------------------------------------
/*
<!--
iconres������������ResourceTile���ƣ�
mapType��ʾ���ڵ�Map���ͣ�0�Ǿ��Σ�1�����Σ�
unitTileW��unitTileH��������Ƿ�͵�ͼtile�ĳߴ��Ƿ�ƥ�䣻
iconid������Դ����
����λ��(xOffset,yOffset)��ͼ�����Ͻ��붨λ���ƫ���λ�����ڵĸ�����(0,0)��
���������������赲���ֵ��float���ͣ�
���obstacleΪ�գ�����Ӱ���赲��Ϣ��
-->
*/
//��Ϸ�����飬һ����Ϸ������ֻ������һ��ͼ����Դ����
class ResourceGameObjectGroup : public PropertySys::SupportRTTI<ResourceGameObjectGroup, Resource>
{
	friend class ResourceManager;
	friend class ResourceGameObject_xmlHandler;
	friend class GameObjectEditor;
public:
	ResourceGameObjectGroup();
	ResourceGameObjectGroup(const Cactus::String& strName, const Cactus::String& strArtGroup, CPoint szTile, int iMapType);
	~ResourceGameObjectGroup();

	virtual bool			Load(){ return _ResGameObjects.size() != 0; }
	virtual void			CreateImageList(CDC* pDC, bool bForceRecreate = false);
	virtual void			Draw(CDC* pDC, const CRect& curTile, EGridType eGrid, const Cactus::String& strItemID);
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strItemID);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	ResourceGameObject*		GetGameObject(const Cactus::String& strItemID);

	void					Save(Cactus::XMLOutStream& xmlOut);

	bool					UpdateGameObject(ResourceGameObject* pGO);
	bool					AddGameObject(ResourceGameObject* pGO);

protected:
	typedef Cactus::list<ResourceGameObject*>::type		ResGameObjectListType;
	ResGameObjectListType	_ResGameObjects;

	Cactus::String			_strArtResKey;	//������ͼ����Դ����
	CPoint					_szUnitTile;	//��Ԫ��Ĵ�С
	int						_iMapType;		//���õĵ�ͼ����
};


