#pragma once

class MapLayer;
class MapBackground;

class Map : public PropertySys::SupportRTTI<Map, PropertySys::RTTIObject>
{
	friend class Map_xmlHandler;
	friend class CTileMapEditorView;
	friend class CTileMapEditorDoc;
public:
	Map();
	~Map();

	void				Draw(CDC* pDC);
	bool				Load(const Cactus::String& strPathName);
	void				Save(const Cactus::String& strPathName);
	void				Reset();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strName;
	Cactus::String		_strFootnotes;
	int					_iWidth;
	int					_iHeight;
	int					_iTileWidthDefault;
	int					_iTileHeightDefault;
	int					_iVersion;

	bool				_bDrawGrid;
	//COLORREF			_colGridColor;
	COLORREF			_colBKColor;

	MapBackground*		_pMapBackground;

	typedef Cactus::list<MapLayer*>::type		MapLayerListType;
	MapLayerListType	_layers;
};
