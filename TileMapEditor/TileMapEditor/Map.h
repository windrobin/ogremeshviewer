#pragma once

#include "MapBase.h"

class MapLayer;
class MapBackground;

class Map : public PropertySys::SupportRTTI<Map, MapBaseObject>
{
	friend class Map_xmlHandler;
	friend class CTileMapEditorView;
	friend class CTileMapEditorDoc;
	friend class MapView;

public:
	Map();
	~Map();

	/**地图的绘制按照地层来进行，一层绘制完成再绘制一层。
	*	只绘制与当前视口相交的区域。
	*	同一地层中绘制的所有区域进行总体排序。
	*/
	void						Draw(CDC* pDC, const CRect& rcView);
	bool						Load(const Cactus::String& strPathName);
	void						Save(const Cactus::String& strPathName);
	void						Reset();

	static void					RegisterReflection();
	virtual void				OnPropertyChanged(const std::string& propName);

	virtual Cactus::String		GetObjectName(){ return _strName; }

	//设置当前Layer，如果Layer不可见，设置为可见，并负责刷新Ribbon和View
	void						SetCurLayer(MapLayer* pLayer);
	MapLayer*					GetCurLayer(){ return _pCurLayer; }

	//如果是唯一的Layer，将被设置为当前的
	bool						AddLayer(MapLayer* pLayer);

	void						RemoveLayer(MapLayer* pLayer);

	//bMakeCurrent只有在bShow为true时才有用
	void						ShowLayer(MapLayer* pLayer, bool bShow, bool bMakeCurrent = false);

	int							GetPixelWidth(){ return _iWidthInTiles * _iUnitTileWidth; }
	int							GetPixelHeight(){ return _iHeightInTiles * _iUnitTileHeight; }

	EMapType					GetType(){ return _eMapType; }
	
	/**根据像素坐标，获取网格坐标
	*	@param ptPixel	当前像素坐标
	*	@param ptGrid	返回的网格坐标
	*	@return 如果在地图区域内，函数成功，返回true，否则false
	*/
	bool						GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid);

	/**获取当前网格的包围矩形（以像素坐标计算）
	*	@param ptGrid	当前的网格坐标
	*/
	CRect						GetPixelCoordRect(const CPoint& ptGrid);

	/**计算区域信息
	*/
	void						CalculateRegionInfo();


	/**获取当前网格所在的区域
	*	@param ptGrid	当前的网格坐标
	*	@param rcRegion	区域包含的网格范围
	*	@return -1 为无效点
	*/
	int							GetRegionID(const CPoint& ptGrid);

	/**获取编号为ID的网格网格范围
	*	@param ID			区域的ID
	*	@param gridRegion	区域包含的网格范围
	*	@param gridRegion	区域包含的像素范围
	*/
	bool						GetRegionGridRect(int ID, CRect& gridRect, CRect& pixelRect);

	/**获取当前像素范围相交和包含的所有区域
	*	@param rcView	当前的像素范围
	*/
	IntVectorType				GetIntersectRegions(const CRect& rcView);

protected:
	Cactus::String		_strFootnotes;		///地图备注
	Cactus::String		_strCurLayerName;	///当前地层名字
	int					_iWidthInTiles;		///宽度方向Tile数量
	int					_iHeightInTiles;	///高度方向Tile数量
	int					_iUnitTileWidth;	///单位Tile宽度，像素
	int					_iUnitTileHeight;	///单位Tile高度，像素
	int					_iVersion;			///地图版本
	EMapType			_eMapType;			///地图类型，井字格还是菱形

	int					_iRegionWidth;		///Region宽度，一个Region宽高应该为显示窗口宽高的1/2
	int					_iRegionHeight;		///Region高度，一个Region宽高应该为显示窗口宽高的1/2

	COLORREF			_colBKColor;		///背景颜色
	bool				_bDrawGrid;			///是否绘制网格
	COLORREF			_colGridColor;		///网格颜色

	MapBackground*		_pMapBackground;	///背景图片，暂时不支持

	typedef Cactus::list<MapLayer*>::type		MapLayerListType;
	MapLayerListType	_layers;

	MapLayer*			_pCurLayer;			///当前地层

	
	struct SRegionInfo 
	{
		int		_regionID;	///Region的编号
		CRect	_rcGrid;	///Region的Grid范围
		CRect	_rcPixel;	///Region的Pixel范围
	};
	typedef Cactus::map<int, SRegionInfo>::type		RegionMapType;
	RegionMapType		_regions;
};
