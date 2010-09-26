#pragma once

enum GameObjectType
{
	eGameObjectNPC,
	eGameObjectMonster,
	eGameObjectFunctionPoint,
	eGameObjectEvent,
	eGameObjectMax
};

class Resource : public PropertySys::SupportRTTI<Resource, PropertySys::RTTIObject>
{
public:
	Resource()
		: _bHasImageList(false)
	{
	}
	virtual ~Resource(){}

	virtual	bool		Load(){ return false; }
	virtual void		CreateImageList(){}

	CImageList*			GetImageList(){ return &_imageList; }

protected:
	CImageList			_imageList;
	bool				_bHasImageList;

};