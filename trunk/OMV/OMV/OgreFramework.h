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

#ifndef OGRE_FRAMEWORK_HPP
#define OGRE_FRAMEWORK_HPP

#include "Updatable.h"

class CameraController;
class Actor;

class OgreFramework : public PropertySys::SupportRTTI<OgreFramework, PropertySys::RTTIObject>
	, public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
	OgreFramework();
	~OgreFramework();

	void setupDemoScene();
	bool initOgre(Ogre::String wndTitle, HWND hwnd = 0);
	bool update();

	CameraController*	GetCameraController(){ return _pCameraController; }

	Ogre::Vector3		GetLookAt();
	Ogre::Real			GetVolumeLength();

	void	AddActor(Actor* p);
	Actor*	GetActor(const Ogre::String& name);
	void	SetCurrentActor(const Ogre::String& name);
	Actor*	GetCurrentActor();

	bool	IsFPSEnabled();
	void	ToggleFPS();

	bool	IsWireframeEnabled();
	void	ToggleWireframe();

	// KeyListener
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	// MouseListener
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id); 
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	// Operations
	void	AddKeyBoardListener(OIS::KeyListener* listener);
	void	RemoveKeyBoardListener(OIS::KeyListener* listener);

	void	AddMouseListener(OIS::MouseListener* listener);
	void	RemoveMouseListener(OIS::MouseListener* listener);

	void	AddUpdatable(IUpdatable* p);
	void	RemoveUpdatable(IUpdatable* p);

	Ogre::Root*				m_pRoot;
	Ogre::SceneManager*		m_pSceneMgr;
	Ogre::RenderWindow*		m_pRenderWnd;
	Ogre::Camera*			m_pCamera;
	Ogre::Viewport*			m_pViewport;
	Ogre::Log*				m_pLog;
	Ogre::Timer*			m_pTimer;

	OIS::InputManager*		m_pInputMgr;
	OIS::Keyboard*			m_pKeyboard;
	OIS::Mouse*				m_pMouse;

	bool					_bRenderWndHasMouse;

	//-------------------------------
	typedef Ogre::map<Ogre::String, Ogre::MeshPtr>::type	MeshMapType;
	MeshMapType				_meshes;


	static void			RegisterReflection();
	virtual void		OnPropertyChanged(const std::string& propName);

private:

	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);

	HWND	_hWnd;

	void updateOgre(double timeSinceLastFrame);

	OgreBites::SdkTrayManager*	    m_pTrayMgr;
	Ogre::FrameEvent                m_FrameEvent;
	int								m_iNumScreenShots;

	double					_timeSinceLastFrame;
	double					_startTime;

	//-------------------------------
	typedef Ogre::list<OIS::KeyListener*>::type		KeyBoardListenerListType;
	typedef Ogre::list<OIS::MouseListener*>::type	MouseListenerType;

	KeyBoardListenerListType	_keyboardListeners;
	MouseListenerType			_mouseListeners;

	OIS::MouseEvent				_adjustMousePosition(const OIS::MouseEvent &evt);

	//-------------------------------
	typedef Ogre::list<IUpdatable*>::type			UpdatableListType;
	UpdatableListType			_updatables;

	CameraController*			_pCameraController;

	//-------------------------------
	typedef Ogre::map<Ogre::String, Actor*>::type	NameActorMapType;
	NameActorMapType			_Actors;

	Ogre::String				_strCurActorName;


	//-------------------------------
	Ogre::Entity*		_gridEntity;
	Ogre::SceneNode*	_pGridNode;
	void	_setupGrid(int size, int unitSize);


	//-------------------------------
	Ogre::ColourValue	_ambientColor;
	Ogre::ColourValue	_backgroundColor;

	int					_gridSize;
	int					_gridUnitSize;
	bool				_bShowGrid;

	Ogre::String		_textureFiltering;
};


#endif 
