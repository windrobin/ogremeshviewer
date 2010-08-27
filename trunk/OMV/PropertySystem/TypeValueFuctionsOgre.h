/*=============================================================================
*	Copyright (C) 2006-2010, Zhang Kun. All Rights Reserved.
*
*	File name:
*			TypeValueFunctionsOgre.h
*	Description:
*			
*	Revision history:
*			2010-6-22	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

namespace PropertySys
{
	//---------------------------------------------------------------------------------------
	template<>
	inline std::string TypeValueToString(const Ogre::String& val)
	{
#if OGRE_WCHAR_T_STRINGS
		return L"";		//TODO
#else
		return val;
#endif
	}

	template<>
	inline std::string TypeValueToString(const Ogre::Vector2& val)
	{
		std::ostringstream os;
		os << val.x << ", " << val.y;
		return os.str();
	}

	template<>
	inline std::string TypeValueToString(const Ogre::Vector3& val)
	{
		std::ostringstream os;
		os << val.x << ", " << val.y << ", " << val.z;
		return os.str();
	}

	template<>
	inline std::string TypeValueToString(const Ogre::Vector4& val)
	{
		std::ostringstream os;
		os << val.x << ", " << val.y << ", " << val.z << ", " << val.w;
		return os.str();
	}

	template<>
	inline std::string TypeValueToString(const Ogre::ColourValue& val)
	{
		std::ostringstream os;
		os << val.r << ", " << val.g << ", " << val.b << ", " << val.a;
		return os.str();
	}

	template<>
	inline std::string TypeValueToString(const Ogre::Quaternion& val)
	{
		std::ostringstream os;
		os << val.w << ", " << val.x << ", " << val.y << ", " << val.z;
		return os.str();
	}

	template<>
	inline std::string TypeValueToString(const Ogre::Matrix3& val)
	{
		std::ostringstream os;
		os  << val[0][0] << ", " << val[0][1] << ", " << val[0][2] << ", " 
			<< val[1][0] << ", " << val[1][1] << ", " << val[1][2] << ", " 
			<< val[2][0] << ", " << val[2][1] << ", " << val[2][2];
		return os.str();
	}

	template<>
	inline std::string TypeValueToString(const Ogre::Matrix4& val)
	{
		std::ostringstream os;
		os  << val[0][0] << ", " << val[0][1] << ", " << val[0][2] << val[0][3] << ", " 
			<< val[1][0] << ", " << val[1][1] << ", " << val[1][2] << val[1][3] << ", " 
			<< val[2][0] << ", " << val[2][1] << ", " << val[2][2] << val[2][3] << ", "
			<< val[3][0] << ", " << val[3][1] << ", " << val[3][2] << val[3][3];
		return os.str();
	}

	//---------------------------------------------------------------------------------------
	template<>
	inline Ogre::Real TypeValueFromString(const std::string& strVal)
	{
		return (Ogre::Real)atof(strVal.c_str());
	}

	template<>
	inline Ogre::String TypeValueFromString(const std::string& strVal)
	{
#if OGRE_WCHAR_T_STRINGS
		return L"";		//TODO
#else
		return strVal;
#endif
	}

	template<>
	inline Ogre::Vector2 TypeValueFromString(const std::string& strVal)
	{
		Ogre::Real x, y;
		sscanf_s(strVal.c_str(), "%f,%f", &x, &y);
		return Ogre::Vector2(x, y);
	}

	template<>
	inline Ogre::Vector3 TypeValueFromString(const std::string& strVal)
	{
		Ogre::Real x, y, z;
		sscanf_s(strVal.c_str(), "%f,%f,%f", &x, &y, &z);
		return Ogre::Vector3(x, y, z);
	}

	template<>
	inline Ogre::Vector4 TypeValueFromString(const std::string& strVal)
	{
		Ogre::Real x, y, z, w;
		sscanf_s(strVal.c_str(), "%f,%f,%f,%f", &x, &y, &z, &w);
		return Ogre::Vector4(x, y, z, w);
	}

	template<>
	inline Ogre::ColourValue TypeValueFromString(const std::string& strVal)
	{
		float r, g, b, a;
		sscanf_s(strVal.c_str(), "%f,%f,%f,%f", &r, &g, &b, &a);
		return Ogre::ColourValue(r, g, b, a);
	}

	template<>
	inline Ogre::Quaternion TypeValueFromString(const std::string& strVal)
	{
		float w, x, y, z;
		sscanf_s(strVal.c_str(), "%f,%f,%f,%f", &w, &x, &y, &z);
		return Ogre::Quaternion(w, x, y, z);
	}

	template<>
	inline Ogre::Matrix3 TypeValueFromString(const std::string& strVal)
	{
		Ogre::Matrix3 m;
		sscanf_s(strVal.c_str(), "%f,%f,%f,%f,%f,%f,%f,%f,%f"
			, &m[0][0], &m[0][1], &m[0][2]
			, &m[1][0], &m[1][1], &m[1][2]
			, &m[2][0], &m[2][1], &m[2][2]
		);
		return m;
	}

	template<>
	inline Ogre::Matrix4 TypeValueFromString(const std::string& strVal)
	{
		Ogre::Matrix4 m;
		sscanf_s(strVal.c_str(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f"
			, &m[0][0], &m[0][1], &m[0][2], &m[0][3]
			, &m[1][0], &m[1][1], &m[1][2], &m[1][3]
			, &m[2][0], &m[2][1], &m[2][2], &m[2][3]
			, &m[3][0], &m[3][1], &m[3][2], &m[3][3]
			);

		return m;
	}

}
