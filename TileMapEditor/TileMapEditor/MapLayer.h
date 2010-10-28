#pragma once

#include "MapBase.h"

struct STile 
{
	STile()
	{
		_bSelected = false;
	}

	int				_posX;			///��������X
	int				_posY;			///��������Y
	int				_regionID;		///���ڵ�RegionID
	Cactus::String	_strResGroup;	///ʹ�õ���Դ��
	Cactus::String	_strResItemID;	///ʹ�õ���Դ
	bool			_bSelected;
};

typedef Cactus::vector<STile*>::type		TileVectorType;

class MapLayer : public PropertySys::SupportRTTI<MapLayer, MapBaseObject>
{
	friend class Map;
	friend class Map_xmlHandler;

public:
	MapLayer();
	~MapLayer();

	void					Init(const Cactus::String& strName, Map* pParent);

	static void				RegisterReflection();
	virtual void			OnPropertyChanged(const std::string& propName);

	void					Draw(CDC* pDC, const IntVectorType& regions);

	bool					IsVisible(){ return _bVisible; }
	void					SetVisible(bool b){ _bVisible = b; }

	virtual Cactus::String	GetObjectName(){ return _strName; }
	virtual bool			IsLayer(){ return true; }

	/**��ȡ��ǰ���ص������������������ط�Χ
	*	@param	ptPixel	��ǰ��������
	*	@param	girdX	���ص�����X����
	*	@param	girdY	���ص�����Y����
	*	@param	rcPixel	���ص���������ط�Χ
	*/
	bool					ToolHitTest(CPoint ptPixel, int& gridX, int& gridY, CRect& rcPixel);

	/**��ȡ��ǰ���ص��Tile
	*	@return	����ж�����ص�һ�������򷵻�0
	*/
	STile*					TileHitTest(CPoint ptPixel);

	/**�ڵ�ǰ�����������ӻ��޸�STile����������ͼ
	*/
	bool					AddOrUpdateTile(int gridX, int gridY, const Cactus::String& resGroup, const Cactus::String& strItemID);

	/**ɾ����ǰ���������STile����������ͼ
	*/
	bool					RemoveTile(int gridX, int gridY);

	/**��ȡ��ǰ���������STile��Ϣ
	*/
	STile*					GetTileInfo(int gridX, int gridY);

	enum ETileOp
	{
		eTileOpAdd,
		eTileOpRemove,
		eTileOpUpdate
	};
	/**���ز������STile�б�
	*/
	void					FillMapLayerList();
	/**���ز������STile�б�
	*/
	void					UpdateTileInfoInMapLayer(STile* pTile, ETileOp e = eTileOpAdd);

	/**�����ƶ�STile��ͼ����
	*/
	void					UpdateTileVisual(STile* pTile, bool bEnsureVisible = false);

protected:
	Map*				_pParentMap;

	bool				_bEnable;
	bool				_bVisible;

	typedef Cactus::map<int, TileVectorType>::type		RegionTileMapType;
	RegionTileMapType	_GroupTiles;
};
