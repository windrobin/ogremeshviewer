
// TileMapEditorView.h : CTileMapEditorView ��Ľӿ�
//


#pragma once

class CTileMapEditorDoc;

class CTileMapEditorView : public CScrollView
{
protected: // �������л�����
	CTileMapEditorView();
	DECLARE_DYNCREATE(CTileMapEditorView)

	//CBitmap		_bmpBackup;
	//CBitmap*	_pOldBmp;
	//CDC			_memDC;
	CRect		_rcClient;

public:
	CTileMapEditorDoc* GetDocument() const;

	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual ~CTileMapEditorView();

	void	LogicInvalidate(CRect rc);
	//CDC* 	GetDrawingContent(CRect& rc, CSize& szDoc, CPoint& ptScroll);
	void 	GetDrawingContent(CRect& rc, CSize& szDoc, CPoint& ptScroll);
	void	OffsetScrollPos(CPoint pt);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // TileMapEditorView.cpp �еĵ��԰汾
inline CTileMapEditorDoc* CTileMapEditorView::GetDocument() const
   { return reinterpret_cast<CTileMapEditorDoc*>(m_pDocument); }
#endif

