#pragma once

#include "MapBase.h"
#include "GridTypes.h"

struct STile 
{
	STile()
	{
		_bSelected = false;
	}

	CPoint			_ptGrid;		///网格坐标
	int				_regionID;		///所在的RegionID
	Cactus::String	_strResGroup;	///使用的资源组
	Cactus::String	_strResItemID;	///使用的资源
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

	/**获取当前像素点的网格坐标和网格像素范围
	*	@param	ptPixel	当前像素坐标
	*	@param	ptGrid	返回的网格坐标
	*	@param	rcPixel	返回的网格的像素范围
	*/
	bool					ToolHitTest(CPoint ptPixel, CPoint& ptGrid, CRect& rcPixel);

	/**获取当前像素点的Tile
	*	@return	如果有多个返回第一个，无则返回0
	*/
	STile*					TileHitTest(CPoint ptPixel, CPoint& ptGrid);

	/**在当前网格坐标增加或修改STile，并更新视图
	*/
	bool					AddOrUpdateTile(CPoint ptGrid, const Cactus::String& resGroup, const Cactus::String& strItemID);

	bool					MoveTile(STile* pTile, CPoint ptNewGrid);

	/**删除当前网格坐标的STile，并更新视图
	*/
	bool					RemoveTile(CPoint ptGrid);

	/**获取当前网格坐标的STile信息
	*/
	STile*					GetTileInfo(CPoint ptGrid);

	enum ETileOp
	{
		eTileOpAdd,
		eTileOpRemove,
		eTileOpUpdate
	};
	/**填充地层面板中STile列表
	*/
	void					FillMapLayerList();
	/**填充地层面板中STile列表
	*/
	void					UpdateTileInfoInMapLayer(STile* pTile, ETileOp e = eTileOpAdd);

	/**更新制定STile视图绘制
	*/
	void					UpdateTileVisual(STile* pTile, bool bEnsureVisible = false);

protected:
	Map*				_pParentMap;

	bool				_bEnable;
	bool				_bVisible;

	typedef Cactus::map<int, TileVectorType>::type		RegionTileMapType;
	RegionTileMapType	_GroupTiles;
};
