#pragma once

#include "DialogGameObject.h"
#include "GameObjectEditorView.h"

class ResourceGameObjectGroup;

class GameObjectEditor : public CDockablePane
{
public:
	GameObjectEditor();
	virtual ~GameObjectEditor();

	void	AddGameObject(ResourceGameObjectGroup* pGOGroup);
	void	EditGameObject(ResourceGameObjectGroup* pGOGroup, ResourceGameObject* pGO);

protected:
	CDialogGameObject		_dlgPanel;
	GameObjectEditorView*	_pView;

// ��д
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
