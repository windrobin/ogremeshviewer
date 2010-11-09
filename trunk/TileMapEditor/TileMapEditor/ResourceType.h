#pragma once

#include "TileMapEditorTypes.h"

enum EResourceType
{
	eResTypeArt,
	eResTypeGameObject,
	eResTypeMax
};

/**
*	��Դ����ࣺһ����Դ��ӵ����������Դ
*/
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
	virtual void			CreateImageList(CDC* pDC, bool bForceRecreate = false){}

	/** ����Դ�Ƿ���Ч
	*	@param strName	����Դ��
	*	@return true����Ч
	*/ 
	virtual bool			IsResItemValid(const Cactus::String& strName){ return false; }

	//��ȡͼ���б�
	CImageList*				GetImageList(){ return &_imageList; }

	/** ���Ʊ��ΪstrID������Դ
	*	@param pDC		���Ƶ��豸
	*	@param curTile	��ǰTile�İ�Χ����
	*	@param eGrid	��ǰ��������
	*	@param strID	����Դ��
	*/ 
	virtual void			Draw(CDC* pDC, const CRect& curTile, EGridType eGrid, const Cactus::String& strID){}

	//��ȡ��Դ������
	Cactus::String			GetResourceName(){ return _strName; }

	//��ȡ��Դ������
	EResourceType			GetResourceType(){ return _eResType; }

	//��ȡ����Դ������
	Cactus::StringVector*	GetCaptions(){ return &_captions; }

	/**��ȡ����Դ�İ�Χ����
	*	@param curTile	��ǰTile�İ�Χ����
	*	@param eGrid	��ǰ��������
	*	@param strID	����Դ��
	*/
	virtual CRect			GetResItemBoundingRect(const CRect& curTile, EGridType eGrid, const Cactus::String& strID){ return CRect(0, 0, 1, 1); }

protected:
	Cactus::String			_strName;			//��Դ������
	CImageList				_imageList;			//����ResouceListView��ͼ���б�
	bool					_bHasImageList;		//ͼ���б��Ƿ��Ѿ�����
	Cactus::StringVector	_captions;			//����Դ����������
	int						_iIconSize;			//��Դ�б�����ͼ��Ĵ�С
	EResourceType			_eResType;			//��Դ������
};