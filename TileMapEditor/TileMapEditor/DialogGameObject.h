#pragma once

#include "resource.h"
#include "afxwin.h"

#include "TileMapEditorTypes.h"
#include "afxcmn.h"

class ResourceGameObject;
class ResourceGameObjectGroup;

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

	//绘制正在编辑的游戏对象
	void			DrawEditingObject(CDC* pDC);

	//绘制单位网格
	void			DrawGrid(CDC* pDC, CPoint ptGrid, COLORREF ref, bool bSolid);

	//添加/编辑后被触发
	void			AfterSetData(const Cactus::String& strResItem);

	//是否点击中编辑对象判断
	bool			HitTest(CPoint ptPixel);

	//移动编辑对象
	void			MoveGameObject(CPoint ptOffset);

	//添加阻挡点
	bool			AddObstacle(CPoint ptPixel);

	//删除阻挡点
	bool			ClearObstacle(CPoint ptPixel);

protected:

	int				GetPixelWidth(){ return _iTileW * _iTileCount; }
	int				GetPixelHeight(){ return _iTileH * _iTileCount; }
	void			UpdateCenterInfo();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	bool						_bAdd;		//添加还是编辑现有的
	ResourceGameObject*			_pResGO;	//当前游戏对象
	ResourceGameObjectGroup*	_pGOGroup;	//当前游戏对象组


	CString				_strGOGroupName;	//组名
	CString				_strGOName;			//游戏对象名
	int					_iTileW;			//基本Tile宽度，不可编辑
	int					_iTileH;			//基本Tile高度，不可编辑
	int					_iTileCount;		//Tile数量
	CSpinButtonCtrl		_spinTileCount;		//快捷编辑Tile数量
	CString				_strCenterOffset;	//绘制左上角相对图像中心偏移
	CString				_strResArtGroup;	//依赖的美术资源组名
	CComboBox			_comboArt;			//依赖的美术资源
	int					_iMode;				//鼠标模式
	int					_iMapType;			//地图模式
	CString				_strMapType;		//地图模式名称
	CComboBox			_comboAIType;		//AI类型
	ObstacleListType	_obstacles;			//阻挡列表
	CPoint				_ptBaryCentric;		//绘制左上角相对图像中心偏移

	CPoint				_ptSelected;		//当前绘制的起点
	CSize				_szSelected;		//当前绘制区域大小

	afx_msg void OnBnClickedButtonGoCurBrush();
	afx_msg void OnBnClickedButtonGoOk();
	afx_msg void OnBnClickedButtonGoCancel();
	afx_msg void OnEnChangeEditGoTileCount();
	afx_msg void OnCbnSelchangeComboGoArtid();
	afx_msg void OnBnClickedRadioGoSetObstacle();
	afx_msg void OnBnClickedRadioGoClearObstacle();
	afx_msg void OnBnClickedRadioGoSelect();
};
