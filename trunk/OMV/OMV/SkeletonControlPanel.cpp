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
#include "OMV.h"
#include "MainFrm.h"
#include "OMVDoc.h"
#include "SkeletonControlPanel.h"

#include "OgreFramework.h"
#include "Actor.h"

using namespace Ogre;


IMPLEMENT_DYNAMIC(SkeletonControlPanel, CDialog)

SkeletonControlPanel::SkeletonControlPanel(CWnd* pParent /*=NULL*/)
	: CDialog(SkeletonControlPanel::IDD, pParent)
	, _bCheckLoop(TRUE)
	, _interPolateMode(0)
	, _fAnimSpeed(1)
{

}

SkeletonControlPanel::~SkeletonControlPanel()
{
	_mesh.setNull();
}

void SkeletonControlPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ANIMS, _comboAnims);
	DDX_Text(pDX, IDC_EDIT_SPEED, _fAnimSpeed);
	DDX_Control(pDX, IDC_TREE_DETAILS, _treeDetails);
	DDX_Control(pDX, IDC_SLIDER_POS, _sliderPos);
	DDX_Check(pDX, IDC_CHECK_LOOP, _bCheckLoop);
	DDX_Radio(pDX, IDC_RADIO_LINEAR, _interPolateMode);
	DDX_Control(pDX, IDC_EDIT_SELECTED_BONE, _editSelectedBone);
}


BEGIN_MESSAGE_MAP(SkeletonControlPanel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &SkeletonControlPanel::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &SkeletonControlPanel::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &SkeletonControlPanel::OnBnClickedButtonStop)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_ANIMS, &SkeletonControlPanel::OnCbnSelchangeComboAnims)
	ON_EN_CHANGE(IDC_EDIT_SPEED, &SkeletonControlPanel::OnEnChangeEditSpeed)
	ON_BN_CLICKED(IDC_CHECK_LOOP, &SkeletonControlPanel::OnBnClickedCheckLoop)
	ON_BN_CLICKED(IDC_RADIO_LINEAR, &SkeletonControlPanel::OnBnClickedRadioInterpolation)
	ON_BN_CLICKED(IDC_RADIO_SPLINE, &SkeletonControlPanel::OnBnClickedRadioInterpolation)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DETAILS, &SkeletonControlPanel::OnTvnSelchangedTreeDetails)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


BOOL SkeletonControlPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	_sliderPos.SetRange(0, 100); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// SkeletonControlPanel message handlers

void SkeletonControlPanel::OnBnClickedButtonPlay()
{
	if (!_mesh.getPointer())
	{
		return;
	}

	int iSelect = _comboAnims.GetCurSel();
	CString strAnimName;
	if (iSelect == -1)
		return;

	UpdateData();

	_comboAnims.GetLBText(iSelect, strAnimName);

	Actor* pActor = OgreFramework::getSingletonPtr()->GetActor(_strActorName);
	if (pActor)
	{
		_bAnimPlaying = true;

		//if( !pActor->IsAnimPlaying((LPCTSTR)strAnimName) )
			pActor->PlayAnim((LPCTSTR)strAnimName);
			pActor->SetAnimLoop(_bCheckLoop == TRUE);
			pActor->SetAnimSpeed(_fAnimSpeed);
	}
}


void SkeletonControlPanel::OnBnClickedButtonPause()
{
	Actor* pActor = OgreFramework::getSingletonPtr()->GetActor(_strActorName);
	if (pActor)
	{
		pActor->TogglePlayAnim();
	}
}

void SkeletonControlPanel::OnBnClickedButtonStop()
{
	Actor* pActor = OgreFramework::getSingletonPtr()->GetActor(_strActorName);
	if (pActor)
	{
		pActor->StopAnim();
	}
}

void SkeletonControlPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

}

void SkeletonControlPanel::BuildSkeletonAnimInfo(const Ogre::String& strActorName)
{
	if (_strActorName == strActorName)
	{
		return;
	}

	_bAnimPlaying	= false;
	_strActorName	= strActorName;

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	COMVDoc* pDoc = DYNAMIC_DOWNCAST(COMVDoc, pMainFrame->GetActiveView()->GetDocument());
	_mesh			= pDoc->GetMesh(_strActorName);

	_comboAnims.ResetContent();
	_treeDetails.DeleteAllItems();

	SkeletonPtr skeleton = _mesh->getSkeleton();
	if (!skeleton.getPointer())
		return;

	for (size_t t = 0; t < skeleton->getNumAnimations(); ++t)
	{
		Animation* pAnim = skeleton->getAnimation(t);
		_comboAnims.AddString( pAnim->getName().c_str() );
	}

	if (_comboAnims.GetCount())
	{
		_comboAnims.SetCurSel(0);
		OnCbnSelchangeComboAnims();
	}
}

void SkeletonControlPanel::OnCbnSelchangeComboAnims()
{
	int iSelect = _comboAnims.GetCurSel();
	CString strAnimName;
	if (iSelect == -1)
		return;

	_comboAnims.GetLBText(iSelect, strAnimName);

	SkeletonPtr skeleton = _mesh->getSkeleton();
	Animation* pAnim = skeleton->getAnimation((LPCTSTR)strAnimName);

	_treeDetails.DeleteAllItems();

	HTREEITEM hRoot = _treeDetails.InsertItem(strAnimName, 0, 0, 0);

	std::ostringstream os;
	os << "Length: " << pAnim->getLength();
	_treeDetails.InsertItem(os.str().c_str(), 0, 0, hRoot);
	os.str("");


	os << "NodeTracks(" << pAnim->getNumNodeTracks() << ")";
	HTREEITEM hNodeTracks = _treeDetails.InsertItem(os.str().c_str(), 0, 0, hRoot);
	os.str("");


	Animation::NodeTrackList::const_iterator it;
	for (it = pAnim->_getNodeTrackList().begin(); it != pAnim->_getNodeTrackList().end(); ++it)
	{
		const NodeAnimationTrack* pNodeTrack = it->second;
		const Node* pNode = pNodeTrack->getAssociatedNode();

		os << "Bone: " << pNode->getName() << "(" << pNodeTrack->getNumKeyFrames() << ")";
		HTREEITEM hItem = _treeDetails.InsertItem(os.str().c_str(), 0, 0, hNodeTracks);
		os.str("");
	}

	_treeDetails.Expand(hRoot, TVE_EXPAND);
	_treeDetails.Expand(hNodeTracks, TVE_EXPAND);


	if (_bAnimPlaying)
	{
		OnBnClickedButtonPlay();
	}
}

void SkeletonControlPanel::OnEnChangeEditSpeed()
{
	Ogre::Real fOld = _fAnimSpeed;

	UpdateData();

	if (fOld == _fAnimSpeed)
	{
		return;
	}

	Actor* pActor = OgreFramework::getSingletonPtr()->GetActor(_strActorName);
	if (pActor)
	{
		pActor->SetAnimSpeed(_fAnimSpeed);
	}

}

void SkeletonControlPanel::OnBnClickedCheckLoop()
{
	BOOL bOld = _bCheckLoop;

	UpdateData();

	if (bOld == _bCheckLoop)
	{
		return;
	}

	Actor* pActor = OgreFramework::getSingletonPtr()->GetActor(_strActorName);
	if (pActor)
	{
		pActor->SetAnimLoop(_bCheckLoop == TRUE);
	}

}

void SkeletonControlPanel::OnBnClickedRadioInterpolation()
{
	UpdateData();

	int iSelect = _comboAnims.GetCurSel();
	CString strAnimName;
	if (iSelect == -1)
		return;

	_comboAnims.GetLBText(iSelect, strAnimName);

	SkeletonPtr skeleton = _mesh->getSkeleton();
	Animation* pAnim = skeleton->getAnimation((LPCTSTR)strAnimName);

	if (_interPolateMode == 0)
	{
		pAnim->setDefaultInterpolationMode(Animation::IM_LINEAR);
	}
	else
	{
		pAnim->setDefaultInterpolationMode(Animation::IM_SPLINE);
	}
}

void SkeletonControlPanel::OnTvnSelchangedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CString strText = _treeDetails.GetItemText(pNMTreeView->itemNew.hItem);

	int iStart	= strText.Find("Bone: ");
	int iEnd	= strText.Find("(");

	if (iStart == -1 || iEnd == -1)
	{
		*pResult = 0;
		return;
	}

	CString str = strText.Mid(6, iEnd - 6);

	_editSelectedBone.SetWindowText(str);

	Actor* pActor = OgreFramework::getSingletonPtr()->GetActor(_strActorName);
	if (pActor)
	{
		pActor->ShowCertainBone((LPCTSTR)str);
	}

	*pResult = 0;
}

void SkeletonControlPanel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int iPos = _sliderPos.GetPos();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void SkeletonControlPanel::OnReset()
{
	_mesh.setNull();
	_bAnimPlaying	= false;
	_strActorName	= "";

	_comboAnims.ResetContent();
	_treeDetails.DeleteAllItems();
	_editSelectedBone.SetWindowText("");
}
