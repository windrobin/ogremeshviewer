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

	virtual	bool			Load(){ return false; }
	virtual void			CreateImageList(CDC* pDC){}
	virtual void			Draw(CDC* pDC, int posX, int posY, const Cactus::String& strID){}
	virtual Cactus::String	GetResourceName(){ return ""; }

	CImageList*				GetImageList(){ return &_imageList; }

	Cactus::StringVector*	GetCaptions(){ return &_captions; }

protected:
	CImageList				_imageList;
	bool					_bHasImageList;
	Cactus::StringVector	_captions;
	int						_iIconSize;
};