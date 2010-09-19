
// TileMapEditorView.h : CTileMapEditorView ��Ľӿ�
//


#pragma once


class CTileMapEditorView : public CScrollView
{
protected: // �������л�����
	CTileMapEditorView();
	DECLARE_DYNCREATE(CTileMapEditorView)

// ����
public:
	CTileMapEditorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CTileMapEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TileMapEditorView.cpp �еĵ��԰汾
inline CTileMapEditorDoc* CTileMapEditorView::GetDocument() const
   { return reinterpret_cast<CTileMapEditorDoc*>(m_pDocument); }
#endif

