#pragma once

#include "DialogGameObject.h"
#include "GameObjectEditorView.h"

class GameObjectEditor : public CDockablePane
{
public:
	GameObjectEditor();
	virtual ~GameObjectEditor();

protected:
	CDialogGameObject		_dlgPanel;
	GameObjectEditorView*	_pView;

// ÖØÐ´
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClassAddMemberFunction();

	DECLARE_MESSAGE_MAP()
};

