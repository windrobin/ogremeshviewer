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

	/**�����������꣬��ȡ��������
	*	@param ptPixel	��ǰ��������
	*	@param ptGrid	���ص���������
	*	@return ����ڵ�ͼ�����ڣ������ɹ�������true������false
	*/
	bool			GetGridCoord(const CPoint& ptPixel, CPoint& ptGrid);

	/**��ȡ��ǰ����İ�Χ���Σ�������������㣩
	*	@param ptGrid	��ǰ����������
	*/
	CRect			GetPixelCoordRect(const CPoint& ptGrid);

	//�������ڱ༭����Ϸ����
	void			DrawEditingObject(CDC* pDC);

	//���Ƶ�λ����
	void			DrawGrid(CDC* pDC, CPoint ptGrid, COLORREF ref, bool bSolid);

	//���/�༭�󱻴���
	void			AfterSetData(const Cactus::String& strResItem);

	//�Ƿ����б༭�����ж�
	bool			HitTest(CPoint ptPixel);

	//�ƶ��༭����
	void			MoveGameObject(CPoint ptOffset);

	//����赲��
	bool			AddObstacle(CPoint ptPixel);

	//ɾ���赲��
	bool			ClearObstacle(CPoint ptPixel);

protected:

	int				GetPixelWidth(){ return _iTileW * _iTileCount; }
	int				GetPixelHeight(){ return _iTileH * _iTileCount; }
	void			UpdateCenterInfo();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	bool						_bAdd;		//��ӻ��Ǳ༭���е�
	ResourceGameObject*			_pResGO;	//��ǰ��Ϸ����
	ResourceGameObjectGroup*	_pGOGroup;	//��ǰ��Ϸ������


	CString				_strGOGroupName;	//����
	CString				_strGOName;			//��Ϸ������
	int					_iTileW;			//����Tile��ȣ����ɱ༭
	int					_iTileH;			//����Tile�߶ȣ����ɱ༭
	int					_iTileCount;		//Tile����
	CSpinButtonCtrl		_spinTileCount;		//��ݱ༭Tile����
	CString				_strCenterOffset;	//�������Ͻ����ͼ������ƫ��
	CString				_strResArtGroup;	//������������Դ����
	CComboBox			_comboArt;			//������������Դ
	int					_iMode;				//���ģʽ
	int					_iMapType;			//��ͼģʽ
	CString				_strMapType;		//��ͼģʽ����
	CComboBox			_comboAIType;		//AI����
	ObstacleListType	_obstacles;			//�赲�б�
	CPoint				_ptOffset;			//�������Ͻ����ͼ������ƫ��

	CPoint				_ptSelected;		//��ǰ���Ƶ����
	CSize				_szSelected;		//��ǰ���������С

	afx_msg void OnBnClickedButtonGoOk();
	afx_msg void OnBnClickedButtonGoCancel();
	afx_msg void OnEnChangeEditGoTileCount();
	afx_msg void OnCbnSelchangeComboGoArtid();
	afx_msg void OnBnClickedRadioGoSetObstacle();
	afx_msg void OnBnClickedRadioGoClearObstacle();
	afx_msg void OnBnClickedRadioGoSelect();
};
