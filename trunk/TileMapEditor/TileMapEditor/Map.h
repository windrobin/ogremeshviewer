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

	void						Draw(CDC* pDC);
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

protected:
	Cactus::String		_strFootnotes;
	Cactus::String		_strCurLayerName;
	int					_iWidthInTiles;
	int					_iHeightInTiles;
	int					_iUnitTileWidth;
	int					_iUnitTileHeight;
	int					_iVersion;

	EMapType			_eMapType;

	COLORREF			_colBKColor;
	bool				_bDrawGrid;
	COLORREF			_colGridColor;

	MapBackground*		_pMapBackground;

	typedef Cactus::list<MapLayer*>::type		MapLayerListType;
	MapLayerListType	_layers;

	MapLayer*			_pCurLayer;
};
