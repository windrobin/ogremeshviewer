#pragma once

#include "ResourceType.h"

//游戏对象资源
class ResourceGameObject
{
	friend class ResourceGameObject_xmlHandler;
	friend class ResourceGameObjectGroup;
public:
	ResourceGameObject();
	~ResourceGameObject();

	//加载对象
	virtual bool		Load(const Cactus::String& strTile);

protected:
	Cactus::String		_strName;		//名字
	Cactus::String		_ArtResID;		//依赖的子图像资源名称
	CPoint				_ptOffset;		//对象定位在(xOffset, yOffset)，图像左上角离定位点的偏差，定位点所在的格子是(0, 0)
	float				_xBaryCentric;	//所有阻挡点的均值
	float				_yBaryCentric;

	typedef Cactus::list<CPoint>::type	ObstacleListType;
	ObstacleListType	_obstacle;		//所有的阻挡点

	//EGameObjectType		_eType;			//GameObject类型
};

//--------------------------------------------------------------------------------------------------------
/*
<!--
iconres是依赖的美术ResourceTile名称；
unitTileW和unitTileH用来检查是否和地图tile的尺寸是否匹配；
iconid是子资源名；
对象定位在(xOffset,yOffset)，图像左上角离定位点的偏差，定位点所在的格子是(0,0)；
重心坐标是所有阻挡点均值，float类型；
如果obstacle为空，对象不影响阻挡信息。
-->
*/
//游戏对象组，一个游戏对象组只能依赖一个图像资源对象
class ResourceGameObjectGroup : public PropertySys::SupportRTTI<ResourceGameObjectGroup, Resource>
{
	friend class ResourceManager;
	friend class ResourceGameObject_xmlHandler;
public:
	ResourceGameObjectGroup();
	~ResourceGameObjectGroup();

	virtual bool			Load(){ return _ResGameObjects.size() != 0; }
	virtual void			CreateImageList(CDC* pDC);
	virtual void			Draw(CDC* pDC, const CRect& curTile, const Cactus::String& strID);
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, const Cactus::String& strID);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

protected:
	typedef Cactus::list<ResourceGameObject*>::type		ResGameObjectListType;
	ResGameObjectListType	_ResGameObjects;

	Cactus::String			_strArtResKey;	//依赖的图像资源名称
	CPoint					_szUnitTile;	//单元格的大小
};


