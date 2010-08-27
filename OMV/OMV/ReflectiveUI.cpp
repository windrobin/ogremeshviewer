/*
-----------------------------------------------------------------------------
This source file is part of OMV(Ogre Mesh Viewer)
For the latest info, see http://code.google.com/p/ogremeshviewer/

Copyright (c) 2010 Zhang Kun(zhk.tiger@gmail.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

-----------------------------------------------------------------------------
This software also uses Microsoft Fluent UI.
License terms to copy, use or distribute the Fluent UI are available separately.  
To learn more about our Fluent UI licensing program, please visit 
http://msdn.microsoft.com/officeui.

Generally speaking, Fluent UI is free, if you do not use it make a contest software
like Office.
-----------------------------------------------------------------------------
*/
*/
#include "stdafx.h"
#include "ReflectiveUI.h"

#include "GString.h"

using namespace std;
using namespace PropertySys;

//-------------------------------------------------------------------------------------
void CPropertyGrid::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	if (_pPropertyChanged)
	{
		_pPropertyChanged->OnPropertyChanged(pProp);
	}
}

#if 0
// MFC bug, this does not work!
// Details : can not retrieve the data just input.
BOOL CPropertyGrid::ValidateItemData(CMFCPropertyGridProperty* pProp)
{
	if (_pPropertyChanged)
	{
		return _pPropertyChanged->ValidateItemData(pProp);
	}

	return FALSE;
}
#endif

//-------------------------------------------------------------------------------------
CReflectiveUI::CReflectiveUI()
{
}

template<class T>
void CReflectiveUI::AddPropertyToGrid(CMFCPropertyGridProperty* root, RTTIObject* pObject, BaseProperty* pProp)
{
	CMFCPropertyGridProperty* pGrid;

	TypedProperty<T>* pTypedProperty = (TypedProperty<T>*)pProp;
	EValueSpecify e = pProp->GetValueSpecify();
	const std::vector<std::string>&	Values = pProp->GetSpeValues();

	if ( e == eValueRange )
	{
		T s = TypeValueFromString<T>(Values[0]);
		T e = TypeValueFromString<T>(Values[1]);

		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)pTypedProperty->GetValue( pObject )
			, pTypedProperty->GetDescription().c_str()
			);

		pGrid->EnableSpinControl(TRUE, s, e);
	}
	else if ( e == eValueList )
	{
		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)pTypedProperty->GetValue( pObject )
			, pTypedProperty->GetDescription().c_str()
			);

		for (size_t t = 0; t < Values.size(); ++t)
		{
			pGrid->AddOption(Values[t].c_str());
		}
		pGrid->AllowEdit(FALSE);
	}
	else if ( e == eFilePathName )
	{
		pGrid = new CMFCPropertyGridFileProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)pTypedProperty->GetValue( pObject )
			, pTypedProperty->GetDescription().c_str()
			);
	}
	else if ( e == eValueColor )
	{
	}
	else
	{
		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)pTypedProperty->GetValue( pObject )
			, pTypedProperty->GetDescription().c_str()
			);
	}

	root->AddSubItem( pGrid );

	CReflectiveUI::SPropertyObject PO(pProp, pObject);
	_GridPropertyMap[pGrid] = PO;
}

template<class T>
void CReflectiveUI::AddPropertyToGrid_AsString(CMFCPropertyGridProperty* root, PropertySys::RTTIObject* pObject, PropertySys::BaseProperty* pProp)
{
	CMFCPropertyGridProperty* pGrid;

	TypedProperty<T>* pTypedProperty = (TypedProperty<T>*)pProp;
	EValueSpecify e = pProp->GetValueSpecify();
	const std::vector<std::string>&	Values = pProp->GetSpeValues();

	if ( e == eValueRange )
	{
	}
	else if ( e == eValueList )
	{
		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)(pTypedProperty->GetValueAsString( pObject ).c_str())
			, pTypedProperty->GetDescription().c_str()
			);

		for (size_t t = 0; t < Values.size(); ++t)
		{
			pGrid->AddOption(Values[t].c_str());
		}
		pGrid->AllowEdit(FALSE);
	}
	else if ( e == eFilePathName )
	{
		pGrid = new CMFCPropertyGridFileProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)(pTypedProperty->GetValueAsString( pObject ).c_str())
			, pTypedProperty->GetDescription().c_str()
			);
	}
	else if ( e == eValueColor )
	{
	}
	else
	{
		pGrid = new CMFCPropertyGridProperty(
			pTypedProperty->GetName().c_str()
			, (_variant_t)(pTypedProperty->GetValueAsString( pObject ).c_str())
			, pTypedProperty->GetDescription().c_str()
			);
	}

	root->AddSubItem( pGrid );

	CReflectiveUI::SPropertyObject PO(pProp, pObject);
	_GridPropertyMap[pGrid] = PO;
}


void CReflectiveUI::BuildUIForObject(CMFCPropertyGridProperty* root, RTTIObject* pObject, const char* szName )
{
	CMFCPropertyGridProperty* pRoot = new CMFCPropertyGridProperty(szName);
	if (root)
	{
		root->AddSubItem( pRoot );
	}
	else
	{
		m_pGrid->AddProperty(pRoot);
	}

	std::vector<BaseProperty*> Properties;
	pObject->GetRTTI()->EnumProperties( Properties );

	for ( size_t i = 0; i < Properties.size(); i++ )
	{
		switch ( Properties[i]->GetTypeID() )
		{
		case eptBool:
			{
				AddPropertyToGrid<bool>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptByte:
			{
				AddPropertyToGrid<char>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptInt:
			{
				AddPropertyToGrid<int>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptShort:
			{
				AddPropertyToGrid<short>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptLong :
			{
				AddPropertyToGrid<long>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptPtr:
			{
				TypedProperty<RTTIObject*>* pTypedProperty = (TypedProperty<RTTIObject*>*)Properties[i];
				BuildUIForObject(pRoot
					, pTypedProperty->GetValue( pObject )
					, pTypedProperty->GetName().c_str()
					);
				break;
			}
		case eptOgreReal:
			{
				AddPropertyToGrid<Ogre::Real>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptOgreString:
			{
				AddPropertyToGrid_AsString<Ogre::String>(pRoot, pObject, Properties[i]);
				break;
			}
		case eptOgreVector2:
			{
				AddPropertyToGrid_AsString<Ogre::Vector2>(pRoot, pObject, Properties[i]);
			}
			break;
		case eptOgreVector3:
			{
				AddPropertyToGrid_AsString<Ogre::Vector3>(pRoot, pObject, Properties[i]);
			}
			break;
		case eptOgreVector4:
			{
				AddPropertyToGrid_AsString<Ogre::Vector4>(pRoot, pObject, Properties[i]);
			}
			break;
		case eptOgreColorValue:
			{
				TypedProperty<Ogre::ColourValue>* pTypedProperty = (TypedProperty<Ogre::ColourValue>*)Properties[i];
				Ogre::ColourValue val = pTypedProperty->GetValue(pObject);

				CMFCPropertyGridProperty* pGrid = new CMFCPropertyGridColorProperty(
					pTypedProperty->GetName().c_str()
					, val.getAsRGBA()
					, 0
					, pTypedProperty->GetDescription().c_str()
					);

				pRoot->AddSubItem( pGrid );

				CReflectiveUI::SPropertyObject PO(Properties[i], pObject);
				_GridPropertyMap[pGrid] = PO;
			}
			break;
		case eptOgreQuaternion:
			{
				AddPropertyToGrid_AsString<Ogre::Quaternion>(pRoot, pObject, Properties[i]);
			}
			break;
		case eptOgreMatrix3:
			{
				AddPropertyToGrid_AsString<Ogre::Matrix3>(pRoot, pObject, Properties[i]);
			}
			break;
		case eptOgreMatrix4:
			{
				AddPropertyToGrid_AsString<Ogre::Matrix4>(pRoot, pObject, Properties[i]);
			}
			break;
		};
	}

	pRoot->Expand();
}

void CReflectiveUI::OnPropertyChanged(CMFCPropertyGridProperty* pProp)
{
	//if (!ValidateItemData(pProp))
	//{
	//	return;
	//}

	CMFCPropertyGridProperty* pItem = pProp;

	const COleVariant variant = pItem->GetValue();

	assert( _GridPropertyMap.find(pItem) != _GridPropertyMap.end() );
	SPropertyObject PO = _GridPropertyMap[pItem];

	switch ( PO._pProperty->GetTypeID() )
	{
	case eptBool:
		{
			TypedProperty<bool>* pTypedProperty = (TypedProperty<bool>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().boolVal != 0);
			break;
		}
	case eptByte:
		{
			TypedProperty<char>* pTypedProperty = (TypedProperty<char>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().bVal );
			break;
		}
	case eptShort:
		{
			TypedProperty<short>* pTypedProperty = (TypedProperty<short>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().iVal );
			break;
		}
	case eptInt:
		{
			TypedProperty<int>* pTypedProperty = (TypedProperty<int>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().intVal );
			break;
		}
	case eptLong:
		{
			TypedProperty<long>* pTypedProperty = (TypedProperty<long>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().lVal );
			break;
		}
	case eptPtr:
		{
			break;
		}
	case eptOgreReal:
		{
			TypedProperty<Ogre::Real>* pTypedProperty = (TypedProperty<Ogre::Real>*)PO._pProperty;
			pTypedProperty->SetValue( PO._pObject, pItem->GetValue().fltVal );
			break;
		}
	case eptOgreString:
		{
			TypedProperty<Ogre::String>* pTypedProperty = (TypedProperty<Ogre::String>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
			break;
		}
	case eptOgreVector2:
		{
			TypedProperty<Ogre::Vector2>* pTypedProperty = (TypedProperty<Ogre::Vector2>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
		}
		break;
	case eptOgreVector3:
		{
			TypedProperty<Ogre::Vector3>* pTypedProperty = (TypedProperty<Ogre::Vector3>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
		}
		break;
	case eptOgreVector4:
		{
			TypedProperty<Ogre::Vector4>* pTypedProperty = (TypedProperty<Ogre::Vector4>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
		}
		break;
	case eptOgreColorValue:
		{
			TypedProperty<Ogre::ColourValue>* pTypedProperty = (TypedProperty<Ogre::ColourValue>*)PO._pProperty;
			Ogre::ColourValue val;
			val.setAsABGR(pItem->GetValue().intVal);
			pTypedProperty->SetValue( PO._pObject, val);
		}
		break;
	case eptOgreQuaternion:
		{
			TypedProperty<Ogre::Quaternion>* pTypedProperty = (TypedProperty<Ogre::Quaternion>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
		}
		break;
	case eptOgreMatrix3:
		{
			TypedProperty<Ogre::Matrix3>* pTypedProperty = (TypedProperty<Ogre::Matrix3>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
		}
		break;
	case eptOgreMatrix4:
		{
			TypedProperty<Ogre::Matrix4>* pTypedProperty = (TypedProperty<Ogre::Matrix4>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			pTypedProperty->SetValueFromString( PO._pObject, gStr.ToMbcs() );
		}
		break;
	};
}

#if 0
// MFC bug, this does not work!
// Details : can not retrieve the data just input.
bool CReflectiveUI::ValidateItemData(CMFCPropertyGridProperty* pProp)
{
	CMFCPropertyGridProperty* pItem = pProp;

	assert( _GridPropertyMap.find(pItem) != _GridPropertyMap.end() );
	SPropertyObject PO = _GridPropertyMap[pItem];

	bool bValid = true;

	switch ( PO._pProperty->GetTypeID() )
	{
	case eptBool:
		{
			TypedProperty<bool>* pTypedProperty = (TypedProperty<bool>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().boolVal != 0);
			break;
		}
	case eptByte:
		{
			TypedProperty<char>* pTypedProperty = (TypedProperty<char>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().bVal );
			break;
		}
	case eptShort:
		{
			TypedProperty<short>* pTypedProperty = (TypedProperty<short>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().iVal );

			break;
		}
	case eptInt:
		{
			TypedProperty<int>* pTypedProperty = (TypedProperty<int>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().intVal );

			break;
		}
	case eptLong:
		{
			TypedProperty<long>* pTypedProperty = (TypedProperty<long>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().lVal );
			break;
		}
	case eptPtr:
		{
			break;
		}
	case eptOgreReal:
		{
			TypedProperty<Ogre::Real>* pTypedProperty = (TypedProperty<Ogre::Real>*)PO._pProperty;
			bValid = pTypedProperty->ValidateValue( pItem->GetValue().fltVal );

			break;
		}
	case eptOgreString:
		{
			TypedProperty<Ogre::String>* pTypedProperty = (TypedProperty<Ogre::String>*)PO._pProperty;
			GString gStr(pItem->GetValue().bstrVal);
			bValid = pTypedProperty->ValidateValue( gStr.ToMbcs() );

			break;
		}
	};

	if (!bValid)
	{
		pItem->SetValue(pItem->GetOriginalValue());
	}

	return bValid;
}
#endif