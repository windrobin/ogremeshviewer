#pragma once


// DialogNewLayer dialog

class DialogNewLayer : public CDialog
{
	DECLARE_DYNAMIC(DialogNewLayer)

public:
	DialogNewLayer(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogNewLayer();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEWLAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	CString _strLayerName;
};
