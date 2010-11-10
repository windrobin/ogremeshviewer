#pragma once

enum EGridType
{
	eRectangle,		//��������
	eDiamond,		//��������
	eGridNone		//���ض���
};

typedef Cactus::list<CPoint>::type	ObstacleListType;

class GridObject : public PropertySys::SupportRTTI<GridObject, PropertySys::RTTIObject>
{
public:
	GridObject();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

	/**�����������꣬��ȡ��������
	*	@param ptPixel	��ǰ��������
	*	@param ptGrid	���ص���������
	*	@return ����ڵ�ͼ�����ڣ������ɹ�������true������false
	*/
	bool			GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid);

	/**��ȡ��ǰ����İ�Χ���Σ�������������㣩
	*	@param ptGrid	��ǰ����������
	*/
	CRect			GetPixelCoordRect(const CPoint& ptGrid);

	int				GetPixelWidth(){ return _iWidthInTiles * _iUnitTileWidth; }
	int				GetPixelHeight(){ return _iHeightInTiles * _iUnitTileHeight; }

	EGridType		GetType(){ return _eGridType; }

	Cactus::String	GetObjectName(){ return _strName; }

	//���Ƶ�λ����
	void			DrawGrid(CDC* pDC, CPoint ptGrid, COLORREF ref, int deflate);

public:
	int					_iWidthInTiles;			///��ȷ���Tile����
	int					_iHeightInTiles;		///�߶ȷ���Tile����
	int					_iUnitTileWidth;		///��λTile��ȣ�����
	int					_iUnitTileHeight;		///��λTile�߶ȣ�����
	EGridType			_eGridType;				///�������ͣ����ָ�������
	Cactus::String		_strName;
};

typedef Cactus::vector<int>::type	IntVectorType;