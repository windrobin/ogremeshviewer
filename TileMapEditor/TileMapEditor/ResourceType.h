#pragma once

#include "TileMapEditorTypes.h"

enum EResourceType
{
	eResTypeArt,
	eResTypeGameObject,
	eResTypeMax
};

/**
*	资源组基类：一个资源组拥有若干子资源
*/
class Resource : public PropertySys::SupportRTTI<Resource, PropertySys::RTTIObject>
{
public:
	Resource()
		: _bHasImageList(false)
	{
	}
	virtual ~Resource(){}

	//加载资源组
	virtual	bool			Load(){ return false; }

	//创建用于ResouceListView需要的图像列表
	virtual void			CreateImageList(CDC* pDC, bool bForceRecreate = false){}

	/** 子资源是否有效
	*	@param strName	子资源名
	*	@return true是有效
	*/ 
	virtual bool			IsResItemValid(const Cactus::String& strName){ return false; }

	//获取图像列表
	CImageList*				GetImageList(){ return &_imageList; }

	/** 绘制编号为strID的子资源
	*	@param pDC		绘制的设备
	*	@param curTile	当前Tile的包围矩形
	*	@param eGrid	当前网格类型
	*	@param strID	子资源名
	*/ 
	virtual void			Draw(CDC* pDC, const CRect& curTile, EGridType eGrid, const Cactus::String& strID){}

	//获取资源组名称
	Cactus::String			GetResourceName(){ return _strName; }

	//获取资源组类型
	EResourceType			GetResourceType(){ return _eResType; }

	//获取子资源的名称
	Cactus::StringVector*	GetCaptions(){ return &_captions; }

	/**获取子资源的包围区域
	*	@param curTile	当前Tile的包围矩形
	*	@param eGrid	当前网格类型
	*	@param strID	子资源名
	*/
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strID){ return CRect(0, 0, 1, 1); }

protected:
	Cactus::String			_strName;			//资源组名称
	CImageList				_imageList;			//用于ResouceListView的图像列表
	bool					_bHasImageList;		//图像列表是否已经创建
	Cactus::StringVector	_captions;			//子资源的名字数组
	int						_iIconSize;			//资源列表缩略图标的大小
	EResourceType			_eResType;			//资源组类型
};