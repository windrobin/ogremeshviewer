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

#pragma once

#include "Updatable.h"

namespace Ogre
{
	class TagPoint;
}

class Actor : public IUpdatable
{
public:
	Actor(const Ogre::String& name);
	~Actor();

	void			SetVisible(bool b/* = true*/);

	virtual void	Update(double deltaTime);

	void			PlayAnim(const Ogre::String& anim);
	void			StopAnim();
	void			TogglePlayAnim();
	bool			IsAnimPlaying(const Ogre::String& anim);
	void			SetAnimLoop(bool b = true);

	void			SetAnimSpeed(Ogre::Real speed);
	Ogre::Real		GetAnimSpeed();

	Ogre::String	GetName(){ return _strName; }

	void			AddBoneVisualizer();

	bool			IsShowAxes();
	void			ToggleAxes();

	bool			IsShowBone();
	void			ToggleBone();

	bool			IsShowBoundingBox();
	void			ToggleBoundingBox();

	void			ShowCertainBone(const Ogre::String& strBone);

	Ogre::Vector3	GetCenter();
	Ogre::Real		GetVolumeLength();

protected:
	Ogre::Entity*		_pBodyEntity;
	Ogre::SceneNode*	_pBodySceneNode;
	Ogre::String		_strName;

	Ogre::Entity*		_axesEntity;
	
	Ogre::Entity*		_axesBoneEntity;
	Ogre::TagPoint*		_axesBoneTagPoint;

	Ogre::Real			_scaleAxes;

	Ogre::AnimationState*	_pCurAnim;
	Ogre::Real				_AnimSpeed;
	bool					_bPaused;


	typedef Ogre::map<Ogre::TagPoint*, Ogre::Node*>::type	TagChildBoneMapType;
	TagChildBoneMapType		_BoneVisuals;
	void			_UpdateBoneVisualizer();

	typedef Ogre::list<Ogre::Entity*>::type					EntityListType;
	EntityListType			_Entitys;

	bool					_bShowAxes;
	bool					_bShowBone;
	bool					_bShowBoundingBox;

	Ogre::Vector3			_vInitCenter;
	Ogre::Real				_fVolumeSize;

};
