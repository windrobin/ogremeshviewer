#pragma once

class GameObjectEditorView : public CScrollView
{
public:
	GameObjectEditorView();
	virtual ~GameObjectEditorView();

	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void	LogicInvalidate(CRect rc);

protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

