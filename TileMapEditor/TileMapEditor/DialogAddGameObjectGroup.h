#pragma once
#include "afxwin.h"


// DialogAddGameObjectGroup dialog

class DialogAddGameObjectGroup : public CDialog
{
	DECLARE_DYNAMIC(DialogAddGameObjectGroup)

public:
	DialogAddGameObjectGroup(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogAddGameObjectGroup();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADD_GAMEOBJECTGRPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString _strGroupName;
	Cactus::String	_strArtGroup;
	CComboBox _comboArList;
	int _iTileW;
	int _iTileH;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	Cactus::StringVector	_ArtGroups;
};
