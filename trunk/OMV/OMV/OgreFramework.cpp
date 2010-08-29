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

#include "stdafx.h"
#include "OgreFramework.h"
#include "CameraController.h"

#include "Actor.h"

using namespace Ogre;
using namespace PropertySys;

template<> OgreFramework* Ogre::Singleton<OgreFramework>::ms_Singleton = 0;

OgreFramework::OgreFramework()
{
	m_iNumScreenShots	= 0;

	m_pRoot				= 0;
	m_pSceneMgr			= 0;
	m_pRenderWnd		= 0;
	m_pCamera			= 0;
	m_pViewport			= 0;
	m_pLog				= 0;
	m_pTimer			= 0;

	m_pInputMgr			= 0;
	m_pKeyboard			= 0;
	m_pMouse			= 0;

	m_pTrayMgr          = 0;
	m_FrameEvent        = Ogre::FrameEvent();

	_timeSinceLastFrame	= 0;
	_startTime			= 0;

	_pCameraController	= 0;
	_bRenderWndHasMouse	= false;


	_ambientColor		= ColourValue(0.8f, 0.8f, 0.8f, 0.8f);
	_backgroundColor	= ColourValue(0, 0, 0, 0);

	_gridSize			= 500;
	_gridUnitSize		= 25;
	_bShowGrid			= true;

	_textureFiltering	= "Bilinear";
	_strFSAA			= "0";

	_gridEntity			= 0;
	_pGridNode			= 0;


	Ogre::LogManager* logMgr = new Ogre::LogManager();
	m_pLog = Ogre::LogManager::getSingleton().createLog("OMV.log", true, true, false);
	m_pLog->setDebugOutputEnabled(true);

}

OgreFramework::~OgreFramework()
{
	delete _pCameraController;

	for (NameActorMapType::iterator it = _Actors.begin(); it != _Actors.end(); ++it)
	{
		delete it->second;
	}

	if(m_pInputMgr)
		OIS::InputManager::destroyInputSystem(m_pInputMgr);

	delete m_pRoot;
}

bool OgreFramework::initOgre(Ogre::String wndTitle, HWND hwnd)
{
	if (!hwnd)
	{
		return false;
	}

	_hWnd = hwnd;

#if _DEBUG
	m_pRoot = new Ogre::Root("plugins_d.cfg", "Ogre.cfg");
#else
	m_pRoot = new Ogre::Root("plugins.cfg", "Ogre.cfg");
#endif

//#if _DEBUG
//
//	m_pRoot->loadPlugin("RenderSystem_Direct3D9_d");
//	m_pRoot->loadPlugin("Plugin_ParticleFX_d");
//	//m_pRoot->loadPlugin("Plugin_CgProgramManager_d");
//
//#else 
//
//	m_pRoot->loadPlugin("RenderSystem_Direct3D9");
//	m_pRoot->loadPlugin("Plugin_ParticleFX");
//	//m_pRoot->loadPlugin("Plugin_CgProgramManager");
//
//#endif

	if (!m_pRoot->restoreConfig())
	{
		//m_pRoot->showConfigDialog();

		RenderSystemList rList = m_pRoot->getAvailableRenderers();
		if (rList.size() == 0)
			return false;

		m_pRoot->setRenderSystem( *(rList.begin()) );

		try
		{
			m_pRoot->saveConfig();	//crash on windows vista or above if no administrator right
		}
		catch (...)
		{
			
		}
	}

	m_pRenderWnd = m_pRoot->initialise(false, wndTitle);

	Ogre::ConfigOptionMap	optionMap = m_pRoot->getRenderSystem()->getConfigOptions();
	if (optionMap.find("FSAA") != optionMap.end())
	{
		_strFSAA = optionMap["FSAA"].currentValue;
	}

	NameValuePairList opts;
	opts["externalWindowHandle"] = StringConverter::toString((Ogre::uint)_hWnd);
	//opts["vsync"]	= "true"; 
	opts["FSAA"]	= _strFSAA; 

	m_pRenderWnd = m_pRoot->createRenderWindow(
		wndTitle,
		800, 600,
		false, &opts
		);


	m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));


	m_pCamera = m_pSceneMgr->createCamera("Camera");
	m_pCamera->setPosition(Vector3(0, 120, 120));
	m_pCamera->lookAt(Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(1);

	_pCameraController = new CameraController(m_pCamera);

	
	m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
	m_pViewport->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));
	m_pViewport->setCamera(m_pCamera);

	
	OIS::ParamList paramList;
	unsigned long hWnd = (unsigned long)AfxGetMainWnd()->GetSafeHwnd();  
	//m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);
	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
    paramList.insert(OIS::ParamList::value_type("w32_mouse", "DISCL_FOREGROUND"));
    paramList.insert(OIS::ParamList::value_type("w32_mouse", "DISCL_NONEXCLUSIVE"));
    paramList.insert(OIS::ParamList::value_type("w32_keyboard", "DISCL_FOREGROUND"));
    paramList.insert(OIS::ParamList::value_type("w32_keyboard", "DISCL_NONEXCLUSIVE"));
	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);


	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pKeyboard->setEventCallback(this);

	m_pMouse	= static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
	m_pMouse->setEventCallback(this);

	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	m_pTimer = new Ogre::Timer();
	m_pTimer->reset();

	m_pTrayMgr = new OgreBites::SdkTrayManager("TrayMgr", m_pRenderWnd, m_pMouse, this);
	//m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	//m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	m_pTrayMgr->hideCursor();

	m_pRenderWnd->setActive(true);


	/*
	BaseProperty *pProp = GetClassRTTI()->GetPropertyByName("FSAA");
	Ogre::ConfigOptionMap	optionMap = m_pRoot->getRenderSystem()->getConfigOptions();
	Ogre::StringVector		fsaaVector = optionMap["FSAA"].possibleValues;
	Ogre::String			strOpts;
	for (size_t t = 0; t < fsaaVector.size(); ++t)
	{
		strOpts += fsaaVector[t];

		if (t != fsaaVector.size() - 1)
		{
			strOpts += ";";
		}
	}
	pProp->SetValueSpecify(eValueList, strOpts);
	*/


	return true;
}

void OgreFramework::setupDemoScene()
{
	m_pSceneMgr->setAmbientLight(_ambientColor);

	//m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
	//m_pSceneMgr->createLight("Light")->setPosition(75,75,75);
	//Ogre::Entity* axesEntity = m_pSceneMgr->createEntity("axes", "axes.mesh");
	//m_pSceneMgr->getRootSceneNode()->createChildSceneNode("axesNode")->attachObject(axesEntity);

	_setupGrid(_gridSize, _gridUnitSize);
}

void OgreFramework::_setupGrid(int size, int unitSize)
{
	if (size <= 100 || unitSize <= 0)
	{
		return;
	}

	if (size > 2000)
		size = 2000;

	if (unitSize >= size)
	{
		unitSize = size / 2;
	}

	if (_gridEntity)
	{
		_gridEntity->detachFromParent();
		m_pSceneMgr->destroyEntity(_gridEntity);
	}

	Ogre::ManualObject mo("Grid");
	int	halfX		= size/2;
	int	halfZ		= size/2;
	int	gridSize	= unitSize;
	mo.begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);

	Ogre::ColourValue	hi(0.6f, 0.6f, 0.6f, 0.6f);
	Ogre::ColourValue	low(0.3f, 0.3f, 0.3f, 0.3f);
	int iCount = 0;
	for (int i = -halfX/gridSize; i <= halfX/gridSize; ++i)
	{
		mo.position(i*gridSize, 0, -halfZ);
		if (i % 10 != 0)
			mo.colour(low);
		else
			mo.colour(hi);
		mo.index(iCount++);

		mo.position(i*gridSize, 0, halfZ);
		if (i % 10 != 0)
			mo.colour(low);
		else
			mo.colour(hi);
		mo.index(iCount++);
	}

	for (int j = -halfZ/gridSize; j <= halfZ/gridSize; ++j)
	{
		mo.position(-halfX, 0, j*gridSize);
		if (j % 10 != 0)
			mo.colour(low);
		else
			mo.colour(hi);
		mo.index(iCount++);

		mo.position(halfX, 0, j*gridSize);
		if (j % 10 != 0)
			mo.colour(low);
		else
			mo.colour(hi);
		mo.index(iCount++);
	}

	mo.end();
	MeshPtr gridMesh = MeshManager::getSingleton().getByName("GridGroundMesh");
	if (!gridMesh.isNull())
	{
		MeshManager::getSingleton().remove(gridMesh->getHandle());
	}
	mo.convertToMesh("GridGroundMesh");

	_gridEntity	= m_pSceneMgr->createEntity("GridGround", "GridGroundMesh");
	if (!_pGridNode)
	{
		_pGridNode	= m_pSceneMgr->getRootSceneNode()->createChildSceneNode("GridNode");
	}
	_pGridNode->attachObject(_gridEntity);
}



void OgreFramework::updateOgre(double timeSinceLastFrame)
{
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
}

bool OgreFramework::update()
{
	if(m_pRenderWnd->isActive())
	{
		_startTime = m_pTimer->getMillisecondsCPU();

		m_pKeyboard->capture();
		m_pMouse->capture();

		for(UpdatableListType::iterator it = _updatables.begin();
			it != _updatables.end(); ++it)
		{
			(*it)->Update(_timeSinceLastFrame);
		}

		for (NameActorMapType::iterator it = _Actors.begin(); it != _Actors.end(); ++it)
		{
			it->second->Update(_timeSinceLastFrame);
		}

		updateOgre(_timeSinceLastFrame);
		m_pRoot->renderOneFrame();

		_timeSinceLastFrame = m_pTimer->getMillisecondsCPU() - _startTime;

		return true;
	}

	return false;
}

bool OgreFramework::IsFPSEnabled()
{
	return m_pTrayMgr->areFrameStatsVisible();
}

void OgreFramework::ToggleFPS()
{
	if(m_pTrayMgr->areFrameStatsVisible())
	{
		//m_pTrayMgr->hideLogo();
		m_pTrayMgr->hideFrameStats();
	}
	else
	{
		//m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
		m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	}
}

bool OgreFramework::IsWireframeEnabled()
{
	return (m_pCamera->getPolygonMode() == PM_WIREFRAME);
}

void OgreFramework::ToggleWireframe()
{
	//static int mode = 0;

	//if(mode == 2)
	//{
	//	m_pCamera->setPolygonMode(PM_SOLID);
	//	mode = 0;
	//}
	//else if(mode == 0)
	//{
	//	m_pCamera->setPolygonMode(PM_WIREFRAME);
	//	mode = 1;
	//}
	//else if(mode == 1)
	//{
	//	m_pCamera->setPolygonMode(PM_POINTS);
	//	mode = 2;
	//}

	if ( IsWireframeEnabled() )
		m_pCamera->setPolygonMode(PM_SOLID);
	else
		m_pCamera->setPolygonMode(PM_WIREFRAME);
}


//------------------------------------------------------------------------------------
void OgreFramework::AddKeyBoardListener(OIS::KeyListener* listener)
{
	_keyboardListeners.push_back(listener);
}

void OgreFramework::RemoveKeyBoardListener(OIS::KeyListener* listener)
{
	KeyBoardListenerListType::iterator it = std::find(_keyboardListeners.begin(), _keyboardListeners.end(), listener);
	if (it != _keyboardListeners.end())
	{
		_keyboardListeners.erase(it);
	}
}

void OgreFramework::AddMouseListener(OIS::MouseListener* listener)
{
	_mouseListeners.push_back(listener);
}

void OgreFramework::RemoveMouseListener(OIS::MouseListener* listener)
{
	MouseListenerType::iterator it = std::find(_mouseListeners.begin(), _mouseListeners.end(), listener);
	if (it != _mouseListeners.end())
	{
		_mouseListeners.erase(it);
	}
}


void OgreFramework::AddUpdatable(IUpdatable* p)
{
	_updatables.push_back(p);
}

void OgreFramework::RemoveUpdatable(IUpdatable* p)
{
	UpdatableListType::iterator it = std::find(_updatables.begin(), _updatables.end(), p);
	if (it != _updatables.end())
	{
		_updatables.erase(it);
	}
}


bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(_hWnd && ::GetFocus() != _hWnd)
		return true;

	if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		m_pRenderWnd->writeContentsToTimestampedFile("BOF_Screenshot_", ".png");
		return true;
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		ToggleWireframe();

		return true;
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		ToggleFPS();

		return true;
	}

	for(KeyBoardListenerListType::iterator it = _keyboardListeners.begin();
		it != _keyboardListeners.end(); ++it)
	{
		if ( (*it)->keyPressed(keyEventRef) )
		{
			return true;
		}
	}

	return true;
}


bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	if(_hWnd && ::GetFocus() != _hWnd)
		return true;

	for(KeyBoardListenerListType::iterator it = _keyboardListeners.begin();
		it != _keyboardListeners.end(); ++it)
	{
		if ( (*it)->keyReleased(keyEventRef) )
		{
			return true;
		}
	}

	return true;
}


OIS::MouseEvent OgreFramework::_adjustMousePosition(const OIS::MouseEvent &evt)
{
	CRect rcParent;
	CRect rcView;

	CWnd* pWnd = AfxGetMainWnd();
	pWnd->GetWindowRect(&rcParent);

	GetWindowRect(_hWnd, &rcView);

	static OIS::MouseState	ms;
	ms = evt.state;
	ms.X.abs	-= rcView.left;
	ms.Y.abs	-= rcView.top;
	ms.X.abs	+= rcParent.left;
	ms.Y.abs	+= rcParent.top;

	return OIS::MouseEvent(const_cast<OIS::Object*>(evt.device), ms);
}


bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
	if (!_bRenderWndHasMouse)
		return true;

	for(MouseListenerType::iterator it = _mouseListeners.begin();
		it != _mouseListeners.end(); ++it)
	{
		if (_hWnd)
		{
			if ( (*it)->mouseMoved(_adjustMousePosition(evt)) )
				return true;
		}
		else
		{
			if ( (*it)->mouseMoved(evt) )
				return true;
		}
	}

	return true;
}

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if (!_bRenderWndHasMouse)
		return true;

	for(MouseListenerType::iterator it = _mouseListeners.begin();
		it != _mouseListeners.end(); ++it)
	{
		if (_hWnd)
		{
			if ( (*it)->mousePressed(_adjustMousePosition(evt), id) )
				return true;
		}
		else
		{
			if ( (*it)->mousePressed(evt, id) )
				return true;
		}
	}

	return true;
}

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if (!_bRenderWndHasMouse)
		return true;

	for(MouseListenerType::iterator it = _mouseListeners.begin();
		it != _mouseListeners.end(); ++it)
	{
		if (_hWnd)
		{
			if ( (*it)->mouseReleased(_adjustMousePosition(evt), id) )
				return true;
		}
		else
		{
			if ( (*it)->mouseReleased(evt, id) )
				return true;
		}
	}

	return true;
}


void OgreFramework::AddActor(Actor* p)
{
	_Actors[p->GetName()] = p;
}

Actor* OgreFramework::GetActor(const Ogre::String& name)
{
	NameActorMapType::iterator it = _Actors.find(name);

	if (it != _Actors.end())
	{
		return it->second;
	}

	return 0;
}

Actor* OgreFramework::GetCurrentActor()
{
	return GetActor(_strCurActorName);
}

void OgreFramework::SetCurrentActor(const Ogre::String& name)
{
	Actor* pActor = GetActor(name);

	if (pActor)
	{
		_strCurActorName = name;

		_pCameraController->StartLookAt();
	}
}

Ogre::Vector3 OgreFramework::GetLookAt()
{
	Actor* pActor = GetCurrentActor();
	if (pActor)
	{
		return pActor->GetCenter();
	}

	return Vector3(0, 0, 0);
}

Ogre::Real OgreFramework::GetVolumeLength()
{
	Actor* pActor = GetCurrentActor();
	if (pActor)
	{
		return pActor->GetVolumeLength();
	}

	return 100;
}

void OgreFramework::OnReset()
{
	//_Actors
	for (NameActorMapType::iterator it = _Actors.begin(); it != _Actors.end(); ++it)
	{
		delete it->second;
	}
	_Actors.clear();

	_strCurActorName = "";
}

void OgreFramework::RegisterReflection()
{
	BaseProperty* pProp = 0;

	M_ReflectionInit(0);

	pProp = M_RegisterPropertySimple(Ogre::ColourValue, AmbientColor, OgreFramework, Rendering, "Scene ambient color.", BaseProperty::eDefault, _ambientColor);
	pProp->SetValueSpecify(eValueColor, "");
	pProp = M_RegisterPropertySimple(Ogre::ColourValue, BackgroundColor, OgreFramework, Rendering, "Background color.", BaseProperty::eDefault, _backgroundColor);
	pProp->SetValueSpecify(eValueColor, "");


	M_RegisterPropertySimple(bool, ShowGrid, OgreFramework, Grid, "Whether show the grid.", BaseProperty::eDefault, _bShowGrid);
	pProp = M_RegisterPropertySimple(int, GridSize, OgreFramework, Grid, "Grid size.", BaseProperty::eDefault, _gridSize);
	pProp->SetValueSpecify(eValueRange, "100;2000");
	pProp = M_RegisterPropertySimple(int, GridUnitSize, OgreFramework, Grid, "Grid unit size.", BaseProperty::eDefault, _gridUnitSize);
	pProp->SetValueSpecify(eValueRange, "10;100");

	
	pProp = M_RegisterPropertySimple(Ogre::String, TextureFiltering, OgreFramework, Rendering, "Texture filtering mode.", BaseProperty::eDefault, _textureFiltering);
	pProp->SetValueSpecify(eValueList, "None;Bilinear;Trilinear;Anisotropic");

	//pProp = M_RegisterPropertySimple(Ogre::String, FSAA, OgreFramework, Rendering, "Full screen anti aliasing.", BaseProperty::eDefault, _strFSAA);
	//pProp->SetValueSpecify(eValueList, "0;2;4;8;16");
}

void OgreFramework::OnPropertyChanged(const std::string& propName)
{
	if ("AmbientColor" == propName)
	{
		m_pSceneMgr->setAmbientLight(_ambientColor);
	}
	else if ("BackgroundColor" == propName)
	{
		m_pViewport->setBackgroundColour(_backgroundColor);
	}
	else if ("ShowGrid" == propName)
	{
		_pGridNode->setVisible(_bShowGrid);
	}
	else if ("GridSize" == propName)
	{
		_setupGrid(_gridSize, _gridUnitSize);
	}
	else if ("GridUnitSize" == propName)
	{
		_setupGrid(_gridSize, _gridUnitSize);
	}
	else if ("TextureFiltering" == propName)
	{
		if ("None" == _textureFiltering)
		{
			MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_NONE);
			MaterialManager::getSingleton().setDefaultAnisotropy(1);
		}
		else if ("Bilinear" == _textureFiltering)
		{
			MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_BILINEAR);
			MaterialManager::getSingleton().setDefaultAnisotropy(1);
		}
		else if ("Trilinear" == _textureFiltering)
		{
			MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_TRILINEAR);
			MaterialManager::getSingleton().setDefaultAnisotropy(8);
		}
		else
		{
			MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
			MaterialManager::getSingleton().setDefaultAnisotropy(8);
		}
	}
	else if ("FSAA" == propName)
	{
		// does not work

		//Ogre::ConfigOptionMap	optionMap = m_pRoot->getRenderSystem()->getConfigOptions();
		//if (optionMap["FSAA"].currentValue != _strFSAA)
		//{
		//	optionMap["FSAA"].currentValue = _strFSAA;
		//	for (Ogre::ConfigOptionMap::iterator it = optionMap.begin(); it != optionMap.end(); ++it)
		//	{
		//		m_pRoot->getRenderSystem()->setConfigOption(it->first, it->second.currentValue);
		//	}
		//}
	}
}

