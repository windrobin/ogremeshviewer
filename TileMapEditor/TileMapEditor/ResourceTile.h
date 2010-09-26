#pragma once

//---------------------------------------------------------------------------------------------------------

class ResourceTile : public PropertySys::SupportRTTI<ResourceTile, PropertySys::RTTIObject>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTile();
	~ResourceTile();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

	virtual void		CreateImageList(){}
	CImageList*			GetImageList(){ return &_imageList; }

protected:
	Cactus::String		_strName;
	int					_tileWidth;
	int					_tileHeight;
	int					_tilesCount;

	CImageList			_imageList;
	bool				_bHasImageList;

	typedef Cactus::list<CBitmap*>::type	BitmapListType;
	BitmapListType		_BitmapTiles;
};

//---------------------------------------------------------------------------------------------------------

class ResourceTileSingleImage : public PropertySys::SupportRTTI<ResourceTileSingleImage, ResourceTile>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTileSingleImage();
	~ResourceTileSingleImage();

	bool				Load();
	virtual void		CreateImageList();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strImageName;
	int					_imageWidth;
	int					_imageHeight;

	CxImage				_image;
};

//---------------------------------------------------------------------------------------------------------

class ResourceTileFolder : public PropertySys::SupportRTTI<ResourceTileFolder, ResourceTile>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceTileFolder();
	~ResourceTileFolder();

	bool				Load();
	virtual void		CreateImageList();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strFolderName;
	Cactus::String		_strFileExt;
	int					_iBits;

	typedef Cactus::map<int, CxImage*>::type	IDImageMapType;
	IDImageMapType		_images;
};