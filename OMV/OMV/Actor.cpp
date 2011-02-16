/*
-----------------------------------------------------------------------------
This source file is part of Tiger Viewer(An Ogre Mesh Viewer)
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

#include "StdAfx.h"
#include "Actor.h"

#include "OgreFramework.h"

#include <Ogre/OgreTagPoint.h>

using namespace Ogre;

Actor::Actor(const Ogre::String& name)
{
	_strName	= name;
	_AnimSpeed	= 1.0;
	_pCurAnim	= 0;
	_bPaused	= false;

	_bShowAxes			= false;
	_bShowBone			= false;
	_bShowBoundingBox	= false;

	_axesBoneTagPoint		= 0;

	//the body
	_pBodyEntity	= OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(_strName, _strName);
	_pBodySceneNode	= OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode(_strName);
	_pBodySceneNode->attachObject(_pBodyEntity);

	//the body axes
	String axesName = _strName + "_" + "axes";
	_axesEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(axesName, "axes.mesh");
	SceneNode* pAxesNode = _pBodySceneNode->createChildSceneNode(axesName);
	pAxesNode->attachObject(_axesEntity);
	_axesEntity->setVisible(_bShowAxes);

	//the bone axes
	axesName = _strName + "_Bone_" + "axes";
	_axesBoneEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(axesName, "axes.mesh");

	AxisAlignedBox aabb = _pBodyEntity->getBoundingBox();
	_vInitCenter = aabb.getCenter();
	_fVolumeSize = aabb.getSize().length();

	_scaleAxes = _fVolumeSize * 0.01;		//for Axes mesh size is about 13, i want the axes to be 1/10 of the mesh
	pAxesNode->setScale(_scaleAxes, _scaleAxes, _scaleAxes);

	AddBoneVisualizer();

	OgreFramework::getSingletonPtr()->AddActor(this);
}

Actor::~Actor()
{
	for (EntityListType::reverse_iterator it = _Entitys.rbegin(); it != _Entitys.rend(); ++it)
	{
		(*it)->detachFromParent();
		OgreFramework::getSingletonPtr()->m_pSceneMgr->destroyEntity(*it);
	}

	_axesEntity->detachFromParent();
	OgreFramework::getSingletonPtr()->m_pSceneMgr->destroyEntity(_axesEntity);

	_axesBoneEntity->detachFromParent();
	OgreFramework::getSingletonPtr()->m_pSceneMgr->destroyEntity(_axesBoneEntity);

	_pBodyEntity->detachFromParent();
	OgreFramework::getSingletonPtr()->m_pSceneMgr->destroyEntity(_pBodyEntity);

	_pBodySceneNode->removeAndDestroyAllChildren();
	OgreFramework::getSingletonPtr()->m_pSceneMgr->destroySceneNode(_pBodySceneNode);
}

void Actor::Update(double deltaTime)
{
	if (_pCurAnim && !_bPaused)
	{
		_pCurAnim->addTime(deltaTime * 0.001 * _AnimSpeed);

		_UpdateBoneVisualizer();
	}
}

void Actor::SetVisible(bool b/* = true*/)
{
	_pBodySceneNode->setVisible(b);

	if (_axesBoneTagPoint)
		_axesBoneEntity->setVisible(b);
	
	if (!b)
	{
		_bShowBoundingBox = false;
		_pBodySceneNode->showBoundingBox(_bShowBoundingBox);

		StopAnim();
	}
}

void Actor::PlayAnim(const Ogre::String& anim)
{
	if (_pCurAnim)
	{
		_pCurAnim->setEnabled(false);
	}

	_pCurAnim = _pBodyEntity->getAnimationState(anim);
	_pCurAnim->setEnabled(true);
	_pCurAnim->setTimePosition(0);
}

void Actor::StopAnim()
{
	if (_pCurAnim)
	{
		_pCurAnim->setEnabled(false);
	}

}

void Actor::TogglePlayAnim()
{
	if (_pCurAnim)
	{
		_bPaused = !_bPaused;
	}

}

bool Actor::IsAnimPlaying(const Ogre::String& anim)
{
	AnimationState* pState = _pBodyEntity->getAnimationState(anim);

	if (pState == _pCurAnim)
	{
		return _pCurAnim->getEnabled();
	}
	
	return false;
}

void Actor::SetAnimSpeed(Ogre::Real speed)
{
	if (speed < 0)
	{
		return;
	}

	_AnimSpeed = speed;
}

Ogre::Real Actor::GetAnimSpeed()
{
	return _AnimSpeed;
}

void Actor::SetAnimLoop(bool b/* = true*/)
{
	if (_pCurAnim)
	{
		_pCurAnim->setLoop(b);
	}

}

void Actor::AddBoneVisualizer()
{
	SkeletonInstance* pSkeletonInstance = _pBodyEntity->getSkeleton();

	if (!pSkeletonInstance)
	{
		return;
	}

	Skeleton::BoneIterator it = pSkeletonInstance->getBoneIterator();
	while (it.hasMoreElements())
	{
		Bone* pBone = it.getNext();

		Bone::ChildNodeIterator cit = pBone->getChildIterator();
		int iCount = 0;
		while (cit.hasMoreElements())
		{
			Node* pChild = cit.getNext();
			iCount++;

			String strName = pBone->getName() + "_" + pChild->getName();
			Ogre::Entity* ent = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(strName, "bone.mesh");
			TagPoint* pTag	= _pBodyEntity->attachObjectToBone(pBone->getName(), ent);

			ent->setVisible(_bShowBone);

			_Entitys.push_back(ent);

			_BoneVisuals[pTag] = pChild;
		}

		if (iCount == 0)
		{
			Ogre::Entity* ent = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(pBone->getName(), "bone.mesh");
			TagPoint* pTag	= _pBodyEntity->attachObjectToBone(pBone->getName(), ent);

			ent->setVisible(_bShowBone);

			_Entitys.push_back(ent);

			_BoneVisuals[pTag] = 0;
		}
	}

	_UpdateBoneVisualizer();
}

void Actor::_UpdateBoneVisualizer()
{

	for (TagChildBoneMapType::iterator it = _BoneVisuals.begin(); it != _BoneVisuals.end(); ++it)
	{
		TagPoint* pTag		= it->first;
		Node* pChild		= it->second;

		if (pChild)
		{
			Vector3 vChild		= pChild->getPosition();
			Vector3 targetDir	= vChild.normalisedCopy();

			const Quaternion& currentOrient = pTag->getOrientation();
			Vector3 currentDir				= currentOrient * Vector3::UNIT_Y;

			Quaternion rotQuat				= currentDir.getRotationTo(targetDir);
			Quaternion	targetOrientation	= rotQuat * currentOrient;

			pTag->setOrientation(targetOrientation);

			Vector3 vOffset = targetOrientation.UnitInverse() * vChild;
			
			Real xzScale	= __min(1, fabs(vOffset.y) * 0.1);
			pTag->setScale(xzScale, fabs(vOffset.y), xzScale);
		}
		else
		{
			Real yScale		= __min(_fVolumeSize * 0.05, 5);
			Real xzScale	= __min(1, yScale * 0.1);
			pTag->setScale(xzScale, yScale, xzScale);
		}
	}
}

bool Actor::IsShowAxes()
{
	return _bShowAxes;
}

void Actor::ToggleAxes()
{
	_bShowAxes = !_bShowAxes;

	_axesEntity->setVisible(_bShowAxes);
}

bool Actor::IsShowBone()
{
	return _bShowBone;
}

void Actor::ToggleBone()
{
	_bShowBone = !_bShowBone;

	for (EntityListType::reverse_iterator it = _Entitys.rbegin(); it != _Entitys.rend(); ++it)
	{
		(*it)->setVisible(_bShowBone);
	}
}

bool Actor::IsShowBoundingBox()
{
	return _bShowBoundingBox;
}

void Actor::ToggleBoundingBox()
{
	_bShowBoundingBox = !_bShowBoundingBox;

	_pBodySceneNode->showBoundingBox(_bShowBoundingBox);
}

void Actor::ShowCertainBone(const Ogre::String& strBone)
{
	if (!_bShowBone)
	{
		return;
	}

	SkeletonInstance* pSkeletonInstance = _pBodyEntity->getSkeleton();

	if (!pSkeletonInstance)
	{
		return;
	}

	Bone* pBone = pSkeletonInstance->getBone(strBone);
	if (!pBone)
	{
		return;
	}

	if (_axesBoneTagPoint)
		_pBodyEntity->detachObjectFromBone(_axesBoneEntity);

	_axesBoneTagPoint = _pBodyEntity->attachObjectToBone(pBone->getName(), _axesBoneEntity);
	_axesBoneTagPoint->setScale(_scaleAxes, _scaleAxes, _scaleAxes);

	//OgreFramework::getSingleton().m_pLog->logMessage(strBone);
}

Ogre::Vector3 Actor::GetCenter()
{
	return _vInitCenter;
}

Ogre::Real Actor::GetVolumeLength()
{
	return _fVolumeSize;
}
