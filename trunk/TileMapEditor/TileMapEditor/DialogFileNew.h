#pragma once
#include "afxwin.h"


// DialogFileNew dialog

class DialogFileNew : public CDialog
{
	DECLARE_DYNAMIC(DialogFileNew)

public:
	DialogFileNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogFileNew();

// Dialog Data
	enum { IDD = IDD_DIALOG_FILE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int _iMapWidth;
	int _iMapHeight;
	int _iTileSize;
	CString _strFootnotes;
};
