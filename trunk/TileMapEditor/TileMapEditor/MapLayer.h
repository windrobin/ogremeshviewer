#pragma once

#include "MapBase.h"
#include "GridTypes.h"

struct STile 
{
	STile()
	{
		_bSelected = false;
	}

	CPoint			_ptGrid;		///��������
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
	*	@param	ptGrid	���ص���������
	*	@param	rcPixel	���ص���������ط�Χ
	*/
	bool					ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel);

	/**��ȡ��ǰ���ص��Tile
	*	@return	����ж�����ص�һ�������򷵻�0
	*/
	STile*					TileHitTest(CPoint ptPixel, CPoint& ptGrid);

	/**�ڵ�ǰ�����������ӻ��޸�STile����������ͼ
	*/
	bool					AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID);

	bool					MoveTile(STile* pTile, CPoint ptNewGrid);

	/**ɾ����ǰ���������STile����������ͼ
	*/
	bool					RemoveTile(CPoint ptGrid);

	/**��ȡ��ǰ���������STile��Ϣ
	*/
	STile*					GetTileInfo(CPoint ptGrid);

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
