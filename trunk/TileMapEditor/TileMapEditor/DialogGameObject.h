#pragma once

#include "resource.h"
#include "afxwin.h"

class CDialogGameObject : public CDialog
{
	DECLARE_DYNAMIC(CDialogGameObject)

public:
	CDialogGameObject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogGameObject();

// Dialog Data
	enum { IDD = IDD_DIALOG_GO_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString _strGOGroupName;
	CString _strGOName;
	int _iTileW;
	int _iTileH;
	int _iTileCountX;
	int _iTileCountY;
	CString _strCenterOffset;
	CComboBox _comboArtSource;
	CComboBox _comboArt;
	afx_msg void OnBnClickedButtonGoCurBrush();
	int _iMode;
};
