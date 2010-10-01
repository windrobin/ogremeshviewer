
// TileMapEditorView.h : CTileMapEditorView 类的接口
//


#pragma once

class CTileMapEditorView : public CScrollView
{
protected: // 仅从序列化创建
	CTileMapEditorView();
	DECLARE_DYNCREATE(CTileMapEditorView)

public:
	CTileMapEditorDoc* GetDocument() const;

	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual ~CTileMapEditorView();

	void	LogicInvalidate(CRect rc);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // TileMapEditorView.cpp 中的调试版本
inline CTileMapEditorDoc* CTileMapEditorView::GetDocument() const
   { return reinterpret_cast<CTileMapEditorDoc*>(m_pDocument); }
#endif

