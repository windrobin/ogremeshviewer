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

#include "StdAfx.h"
#include "CameraController.h"
#include "OgreFramework.h"

using namespace Ogre;

CameraController::CameraController(Ogre::Camera *pCamera)
: _pCamera(pCamera)
, _bLookAtMode(true)
{
	OgreFramework::getSingletonPtr()->AddKeyBoardListener(this);
	OgreFramework::getSingletonPtr()->AddMouseListener(this);
	OgreFramework::getSingletonPtr()->AddUpdatable(this);

	m_MoveSpeed			= 0.75f;
	m_TranslateVector	= Vector3::ZERO;
}

CameraController::~CameraController()
{
	OgreFramework::getSingletonPtr()->RemoveKeyBoardListener(this);
	OgreFramework::getSingletonPtr()->RemoveMouseListener(this);
	OgreFramework::getSingletonPtr()->RemoveUpdatable(this);
}

// KeyListener
bool CameraController::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	return false;
}

bool CameraController::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return false;
}

// MouseListener
bool CameraController::mouseMoved(const OIS::MouseEvent &evt)
{
	if (evt.state.buttonDown(OIS::MB_Left))
	{
		if (_bLookAtMode)
		{
			// The algorithm is that one I used long time ago when in Ubisoft around Year 2001 for VIP PS2. So long...
			Vector3 vCamPos			= _pCamera->getPosition();
			Vector3 vCamDir			= vCamPos - OgreFramework::getSingleton().GetLookAt();
			Real	dirLength		= vCamDir.length();

			//Real fVolumeLength		= OgreFramework::getSingleton().GetVolumeLength();
			//Real fAdjust			= dirLength / fVolumeLength;

			Vector3 vOffset			= /*fAdjust * */_pCamera->getOrientation() * Vector3(evt.state.X.rel * -1.0f, evt.state.Y.rel * 1.0f, 0);
			Vector3 vNewDir			= vOffset + vCamDir;
			vNewDir.normalise();
			
			if ( fabs(vNewDir.dotProduct(Vector3::UNIT_Y)) < 0.98f )
			{
				Vector3 vNewPos		= vNewDir * dirLength + OgreFramework::getSingleton().GetLookAt();

				_pCamera->setPosition(vNewPos);
				StartLookAt();
			}
		}
		else
		{
			_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
			_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
		}

	}
	
	if (evt.state.Z.rel != 0)
	{
		Vector3 vDistance	= _pCamera->getPosition() - OgreFramework::getSingleton().GetLookAt();
		Vector3 vCamDir		= _pCamera->getDirection();

		Real fVolumeLength = OgreFramework::getSingleton().GetVolumeLength();
		Real delta;
		if (vDistance.length() > fVolumeLength)
		{
			delta = __max(fVolumeLength * 0.25, fabs(evt.state.Z.rel * 0.05f));
		}
		else
		{
			delta = __min(fVolumeLength * 0.05, fabs(evt.state.Z.rel * 0.05f));
		}
		if (evt.state.Z.rel < 0)
		{
			delta = -delta;
		}

		// Do not zoom too near to the mesh center
		Real fDistance = vDistance.length();
		if (vCamDir.dotProduct(Vector3::UNIT_Y) <= 0)
		{
			if (fDistance < fVolumeLength * 0.25 && evt.state.Z.rel > 0)
				return true;
		}
		else
		{
			if (fDistance * 2 > -fVolumeLength * 0.25 && evt.state.Z.rel > 0)
				return true;
		}

		Vector3 v = vCamDir * delta;
		_pCamera->move(v);
	}

	return true;
}

bool CameraController::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}
 
bool CameraController::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

void CameraController::StartLookAt()
{
	Vector3 vLookAt = OgreFramework::getSingleton().GetLookAt();
	_pCamera->lookAt(vLookAt);
}

void CameraController::Update(double deltaTime)
{
	if(!_bLookAtMode)
	{
		m_MoveScale = m_MoveSpeed   * deltaTime;

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
			m_TranslateVector.x = -m_MoveScale;

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
			m_TranslateVector.x = m_MoveScale;

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
			m_TranslateVector.z = -m_MoveScale;

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
			m_TranslateVector.z = m_MoveScale;

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)) 
			_pCamera->moveRelative(m_TranslateVector);
		else
			_pCamera->moveRelative(m_TranslateVector / 5);

		m_TranslateVector = Vector3::ZERO;
	}
}
