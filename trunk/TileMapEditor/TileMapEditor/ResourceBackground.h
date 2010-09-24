#pragma once

class ResourceBackground : public PropertySys::SupportRTTI<ResourceBackground, PropertySys::RTTIObject>
{
	friend class ResourceManager;
	friend class ResourceArt_xmlHandler;
public:
	ResourceBackground();
	~ResourceBackground();

	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

protected:
	Cactus::String		_strName;
	Cactus::String		_strImagePathName;
	int					_iWidth;
	int					_iHeight;
};
