#pragma once

enum EResourceType
{
	eResTypeArt,
	eResTypeGameObject,
	eResTypeMax
};


enum EGameObjectType
{
	eGameObjectNPC,
	eGameObjectMonster,
	eGameObjectFunctionPoint,
	eGameObjectEvent,
	eGameObjectMax
};

//资源组：拥有若干子资源
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
	virtual void			CreateImageList(CDC* pDC){}

	//子资源是否有效
	virtual bool			IsResItemValid(const Cactus::String& strName){ return false; }

	//获取图像列表
	CImageList*				GetImageList(){ return &_imageList; }

	//绘制编号为strID的子资源
	virtual void			Draw(CDC* pDC, int posCenterX, int posCenterY, const Cactus::String& strID){}

	//获取资源组名称
	Cactus::String			GetResourceName(){ return _strName; }

	//获取资源组类型
	EResourceType			GetResourceType(){ return _eResType; }

	//获取子资源的名称
	Cactus::StringVector*	GetCaptions(){ return &_captions; }

protected:
	Cactus::String			_strName;			//资源组名称
	CImageList				_imageList;			//用于ResouceListView的图像列表
	bool					_bHasImageList;		//图像列表是否已经创建
	Cactus::StringVector	_captions;			//子资源的名字数组
	int						_iIconSize;			//资源列表缩略图标的大小
	EResourceType			_eResType;			//资源组类型
};