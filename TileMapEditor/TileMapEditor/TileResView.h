#pragma once

#include "PanelToolBar.h"

class TileResView : public CDockablePane
{
public:
	TileResView();
	virtual ~TileResView();

	void			BuildImageAndInfoes(const Cactus::String& strResKey, CImageList* pImage, const Cactus::StringVector& captions);

protected:
	CClassToolBar	m_wndToolBar;
	CListCtrl		_listImages;
	int				_iOldCheck;
	Cactus::String	_strResKey;

// ÖØÐ´
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClassAddMemberFunction();

	DECLARE_MESSAGE_MAP()
};

