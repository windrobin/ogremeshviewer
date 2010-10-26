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

	/**��ͼ�Ļ��ư��յز������У�һ���������ٻ���һ�㡣
	*	ֻ�����뵱ǰ�ӿ��ཻ������
	*	ͬһ�ز��л��Ƶ��������������������
	*/
	void						Draw(CDC* pDC, const CRect& rcView);
	bool						Load(const Cactus::String& strPathName);
	void						Save(const Cactus::String& strPathName);
	void						Reset();

	static void					RegisterReflection();
	virtual void				OnPropertyChanged(const std::string& propName);

	virtual Cactus::String		GetObjectName(){ return _strName; }

	//���õ�ǰLayer�����Layer���ɼ�������Ϊ�ɼ���������ˢ��Ribbon��View
	void						SetCurLayer(MapLayer* pLayer);
	MapLayer*					GetCurLayer(){ return _pCurLayer; }

	//�����Ψһ��Layer����������Ϊ��ǰ��
	bool						AddLayer(MapLayer* pLayer);

	void						RemoveLayer(MapLayer* pLayer);

	//bMakeCurrentֻ����bShowΪtrueʱ������
	void						ShowLayer(MapLayer* pLayer, bool bShow, bool bMakeCurrent = false);

	int							GetPixelWidth(){ return _iWidthInTiles * _iUnitTileWidth; }
	int							GetPixelHeight(){ return _iHeightInTiles * _iUnitTileHeight; }

	EMapType					GetType(){ return _eMapType; }
	
	/**�����������꣬��ȡ��������
	*	@param ptPixel	��ǰ��������
	*	@param ptGrid	���ص���������
	*	@return ����ڵ�ͼ�����ڣ������ɹ�������true������false
	*/
	bool						GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid);

	/**��ȡ��ǰ����İ�Χ���Σ�������������㣩
	*	@param ptGrid	��ǰ����������
	*/
	CRect						GetPixelCoordRect(const CPoint& ptGrid);

	/**����������Ϣ
	*/
	void						CalculateRegionInfo();


	/**��ȡ��ǰ�������ڵ�����
	*	@param ptGrid	��ǰ����������
	*	@param rcRegion	�������������Χ
	*	@return -1 Ϊ��Ч��
	*/
	int							GetRegionID(const CPoint& ptGrid);

	/**��ȡ���ΪID����������Χ
	*	@param ID			�����ID
	*	@param gridRegion	�������������Χ
	*	@param gridRegion	������������ط�Χ
	*/
	bool						GetRegionGridRect(int ID, CRect& gridRect, CRect& pixelRect);

	/**��ȡ��ǰ���ط�Χ�ཻ�Ͱ�������������
	*	@param rcView	��ǰ�����ط�Χ
	*/
	IntVectorType				GetIntersectRegions(const CRect& rcView);

protected:
	Cactus::String		_strFootnotes;		///��ͼ��ע
	Cactus::String		_strCurLayerName;	///��ǰ�ز�����
	int					_iWidthInTiles;		///��ȷ���Tile����
	int					_iHeightInTiles;	///�߶ȷ���Tile����
	int					_iUnitTileWidth;	///��λTile��ȣ�����
	int					_iUnitTileHeight;	///��λTile�߶ȣ�����
	int					_iVersion;			///��ͼ�汾
	EMapType			_eMapType;			///��ͼ���ͣ����ָ�������

	int					_iRegionWidth;		///Region��ȣ�һ��Region���Ӧ��Ϊ��ʾ���ڿ�ߵ�1/2
	int					_iRegionHeight;		///Region�߶ȣ�һ��Region���Ӧ��Ϊ��ʾ���ڿ�ߵ�1/2

	COLORREF			_colBKColor;		///������ɫ
	bool				_bDrawGrid;			///�Ƿ��������
	COLORREF			_colGridColor;		///������ɫ

	MapBackground*		_pMapBackground;	///����ͼƬ����ʱ��֧��

	typedef Cactus::list<MapLayer*>::type		MapLayerListType;
	MapLayerListType	_layers;

	MapLayer*			_pCurLayer;			///��ǰ�ز�

	
	struct SRegionInfo 
	{
		int		_regionID;	///Region�ı��
		CRect	_rcGrid;	///Region��Grid��Χ
		CRect	_rcPixel;	///Region��Pixel��Χ
	};
	typedef Cactus::map<int, SRegionInfo>::type		RegionMapType;
	RegionMapType		_regions;
};
