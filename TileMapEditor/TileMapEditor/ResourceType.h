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

//��Դ�飺ӵ����������Դ
class Resource : public PropertySys::SupportRTTI<Resource, PropertySys::RTTIObject>
{
public:
	Resource()
		: _bHasImageList(false)
	{
	}
	virtual ~Resource(){}

	//������Դ��
	virtual	bool			Load(){ return false; }

	//��������ResouceListView��Ҫ��ͼ���б�
	virtual void			CreateImageList(CDC* pDC){}

	//����Դ�Ƿ���Ч
	virtual bool			IsResItemValid(const Cactus::String& strName){ return false; }

	//��ȡͼ���б�
	CImageList*				GetImageList(){ return &_imageList; }

	//���Ʊ��ΪstrID������Դ
	virtual void			Draw(CDC* pDC, int posCenterX, int posCenterY, const Cactus::String& strID){}

	//��ȡ��Դ������
	Cactus::String			GetResourceName(){ return _strName; }

	//��ȡ��Դ������
	EResourceType			GetResourceType(){ return _eResType; }

	//��ȡ����Դ������
	Cactus::StringVector*	GetCaptions(){ return &_captions; }

protected:
	Cactus::String			_strName;			//��Դ������
	CImageList				_imageList;			//����ResouceListView��ͼ���б�
	bool					_bHasImageList;		//ͼ���б��Ƿ��Ѿ�����
	Cactus::StringVector	_captions;			//����Դ����������
	int						_iIconSize;			//��Դ�б�����ͼ��Ĵ�С
	EResourceType			_eResType;			//��Դ������
};