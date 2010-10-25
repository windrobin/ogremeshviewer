#pragma once

#include "DialogResDetail.h"

//class ResDetailToolBar : public CClassToolBar
//{
//public:
//
//	void		CreateControls();
//
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor ); 
//
//	DECLARE_MESSAGE_MAP()
//
//protected:
//	CStatic	_editResGroupC;
//	CStatic	_editResGroup;
//	
//	CStatic	_editResTypeC;
//	CStatic	_editResType;
//
//	CButton	_btnEdit;
//
//	CBrush	_brush;
//	CFont	_font;
//};

class TileResView : public CDockablePane
{
public:
	TileResView();
	virtual ~TileResView();

	void			BuildImageAndInfoes(const Cactus::String& strResKey, CImageList* pImage, const Cactus::StringVector& captions);

protected:
	DialogResDetail		_dialogBar;
	CListCtrl			_listImages;
	int					_iOldCheck;
	Cactus::String		_strResKey;

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

