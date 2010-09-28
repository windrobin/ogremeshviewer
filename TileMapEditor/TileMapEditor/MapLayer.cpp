#include "StdAfx.h"
#include "MapLayer.h"
#include "ResourceManager.h"

using namespace Cactus;
using namespace PropertySys;

MapLayer::MapLayer()
{
	_bVisible = true;
}

MapLayer::~MapLayer()
{
}


void MapLayer::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Cactus::String, Name, MapLayer, Map, "Layer名.", BaseProperty::eDefault, _strName);

	pProp = M_RegisterPropertySimple(int, Width, MapLayer, Map, "宽度，不可超过所属Map的宽度.", BaseProperty::eDefault, _iWidth);
	pProp = M_RegisterPropertySimple(int, Height, MapLayer, Map, "高度，不可超过所属Map的高度.", BaseProperty::eDefault, _iHeight);
	pProp = M_RegisterPropertySimple(int, TileWidth, MapLayer, Map, "绘制模式.", BaseProperty::eDefault, _iTileWidth);
	pProp = M_RegisterPropertySimple(int, TileHeight, MapLayer, Map, "绘制模式.", BaseProperty::eDefault, _iTileHeight);
	pProp = M_RegisterPropertySimple(bool, Enable, MapLayer, Map, "是否激活，不激活将不会显示和导出.", BaseProperty::eDefault, _bEnable);
	
}

void MapLayer::OnPropertyChanged(const std::string& propName)
{
}

void MapLayer::Draw(CDC* pDC)
{
	if (!_bEnable)
		return;

	CPen pen(PS_SOLID, 1, RGB(0, 128, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	int iGridWidth	= _iWidth / _iTileWidth;
	int iGridHeight	= _iHeight / _iTileHeight;
	for (int i = 0; i <= iGridWidth; i++)
	{
		pDC->MoveTo(0, i * _iTileWidth);
		pDC->LineTo(_iHeight, i * _iTileHeight);
	}
	for (int i = 0; i <= iGridHeight; i++)
	{
		pDC->MoveTo(i * _iTileWidth, 0);
		pDC->LineTo(i * _iTileHeight, _iWidth);
	}

	pDC->SelectObject(pOldPen);


	for (TileGroupMapType::iterator it = _GroupTiles.begin(); it != _GroupTiles.end(); ++it)
	{
		Resource* pRes = ResourceManager::getSingleton().GetResource(it->first);
		if (pRes)
		{
			for (size_t t = 0; t < it->second.size(); ++t)
			{
				STile& tile = it->second[t];
				pRes->Draw(pDC, tile._posX * _iTileWidth, tile._posY * _iTileHeight, tile._strID);
			}
		}
	}
}
