#pragma once

#include "resource.h"
#include "afxwin.h"

#include "TileMapEditorTypes.h"
#include "afxcmn.h"

class ResourceGameObject;

class CDialogGameObject : public CDialog
{
	DECLARE_DYNAMIC(CDialogGameObject)

public:
	CDialogGameObject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogGameObject();

// Dialog Data
	enum { IDD = IDD_DIALOG_GO_EDITOR };

	/**根据像素坐标，获取网格坐标
	*	@param ptPixel	当前像素坐标
	*	@param ptGrid	返回的网格坐标
	*	@return 如果在地图区域内，函数成功，返回true，否则false
	*/
	bool			GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid);

	/**获取当前网格的包围矩形（以像素坐标计算）
	*	@param ptGrid	当前的网格坐标
	*/
	CRect			GetPixelCoordRect(const CPoint& ptGrid);

	void			DrawEditingObject(CDC* pDC);

	void			EnumArtResItem(const Cactus::String& strResItem);

	bool			HitTest(CPoint pt);
	void			MoveGameObject(CPoint ptOffset);

protected:

	int				GetPixelWidth(){ return _iTileW * _iTileCount; }
	int				GetPixelHeight(){ return _iTileH * _iTileCount; }
	void			UpdateCenterInfo();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CString _strGOGroupName;
	CString _strGOName;
	int _iTileW;
	int _iTileH;
	int _iTileCount;
	CString _strCenterOffset;
	CString _strResArtGroup;
	CComboBox _comboArt;
	int _iMode;
	int _iMapType;
	CString _strMapType;
	CComboBox _comboAIType;
	ObstacleListType	_obstacles;
	CSpinButtonCtrl _spinTileCount;

	CPoint		_ptSelected;
	CSize		_szSelected;

	afx_msg void OnBnClickedButtonGoCurBrush();
	afx_msg void OnBnClickedButtonGoOk();
	afx_msg void OnBnClickedButtonGoCancel();
	afx_msg void OnEnChangeEditGoTileCount();
	afx_msg void OnCbnSelchangeComboGoArtid();
	afx_msg void OnBnClickedRadioGoSetObstacle();
	afx_msg void OnBnClickedRadioGoClearObstacle();
};
