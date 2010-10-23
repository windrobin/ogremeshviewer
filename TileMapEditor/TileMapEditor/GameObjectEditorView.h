#pragma once

class GameObjectEditorView : public CScrollView
{
public:
	GameObjectEditorView();
	virtual ~GameObjectEditorView();

	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void	LogicInvalidate(CRect rc);

protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

