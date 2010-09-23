#pragma once

//---------------------------------------------------------------------------------------------------------

class ResourceTile : public PropertySys::SupportRTTI<ResourceTile, PropertySys::RTTIObject>
{
	friend class ResourceManager;
public:
	ResourceTile();
	~ResourceTile();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strName;
	int					_tileWidth;
	int					_tileHeight;
	int					_tilesCount;
};

//---------------------------------------------------------------------------------------------------------

class ResourceTileSingleImage : public PropertySys::SupportRTTI<ResourceTileSingleImage, ResourceTile>
{
	friend class ResourceManager;
public:
	ResourceTileSingleImage();
	~ResourceTileSingleImage();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strImageName;
	int					_imageWidth;
	int					_imageHeight;
};

//---------------------------------------------------------------------------------------------------------

class ResourceTileFolder : public PropertySys::SupportRTTI<ResourceTileFolder, ResourceTile>
{
	friend class ResourceManager;
public:
	ResourceTileFolder();
	~ResourceTileFolder();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strFolderName;
	Cactus::String		_strFileExt;
};