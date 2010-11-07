#pragma once

#include "GameObjectEditor.h"
#include "DialogGameObject.h"
#include "GameObjectEditorView.h"


extern "C"
{
	GameObjectEditor*		GetGOEditor();
	GameObjectEditorView*	GetGOView();
	CDialogGameObject*		GetGODlg();
}