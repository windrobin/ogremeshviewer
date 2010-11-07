#pragma once

class GameObjectEditorView : public CScrollView
{
	DECLARE_DYNAMIC(GameObjectEditorView)

public:
	GameObjectEditorView();
	virtual ~GameObjectEditorView();

	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual void OnInitialUpdate(); // 构造后第一次调用

protected:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};

