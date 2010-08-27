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
#include "afxwin.h"
#include "afxcmn.h"

#include "resource.h"

// SkeletonControlPanel dialog

class SkeletonControlPanel : public CDialog
{
	DECLARE_DYNAMIC(SkeletonControlPanel)

public:
	SkeletonControlPanel(CWnd* pParent = NULL);   // standard constructor
	virtual ~SkeletonControlPanel();

	void	BuildSkeletonAnimInfo(const Ogre::String& strActorName);

// Dialog Data
	enum { IDD = IDD_DIALOG_ANIMATION };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	Ogre::MeshPtr	_mesh;
	Ogre::String	_strActorName;
	bool			_bAnimPlaying;

	DECLARE_MESSAGE_MAP()
public:
	CComboBox		_comboAnims;
	CTreeCtrl		_treeDetails;
	CSliderCtrl		_sliderPos;
	BOOL			_bCheckLoop;
	int				_interPolateMode;
	float			_fAnimSpeed;
	CEdit			_editSelectedBone;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboAnims();
	afx_msg void OnEnChangeEditSpeed();
	afx_msg void OnBnClickedCheckLoop();
	afx_msg void OnBnClickedRadioInterpolation();
	afx_msg void OnTvnSelchangedTreeDetails(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
