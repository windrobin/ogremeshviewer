#pragma once

enum EGridType
{
	eRectangle,		//矩形网格
	eDiamond,		//菱形网格
	eGridNone		//像素对齐
};

typedef Cactus::list<CPoint>::type	ObstacleListType;

class GridObject : public PropertySys::SupportRTTI<GridObject, PropertySys::RTTIObject>
{
public:
	GridObject();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

	/**根据像素坐标，获取网格坐标
	*	@param ptPixel	当前像素坐标
	*	@param ptGrid	返回的网格坐标
	*	@return 如果在地图区域内，函数成功，返回true，否则false
	*/
	bool			GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid);

	/**获取当前网格的包围矩形（以像素坐标计算）
	*	@param ptGrid	当前的网格坐标
	*/
	CRect			GetPixelCoordRect(const CPoint& ptGrid);

	int				GetPixelWidth(){ return _iWidthInTiles * _iUnitTileWidth; }
	int				GetPixelHeight(){ return _iHeightInTiles * _iUnitTileHeight; }

	EGridType		GetType(){ return _eGridType; }

	Cactus::String	GetObjectName(){ return _strName; }

	//绘制单位网格
	void			DrawGrid(CDC* pDC, CPoint ptGrid, COLORREF ref, int deflate);

public:
	int					_iWidthInTiles;			///宽度方向Tile数量
	int					_iHeightInTiles;		///高度方向Tile数量
	int					_iUnitTileWidth;		///单位Tile宽度，像素
	int					_iUnitTileHeight;		///单位Tile高度，像素
	EGridType			_eGridType;				///网格类型，井字格还是菱形
	Cactus::String		_strName;
};

typedef Cactus::vector<int>::type	IntVectorType;