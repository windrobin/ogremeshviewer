
// TileMapEditorDoc.h : CTileMapEditorDoc ��Ľӿ�
//


#pragma once


class CTileMapEditorDoc : public CDocument
{
protected: // �������л�����
	CTileMapEditorDoc();
	DECLARE_DYNCREATE(CTileMapEditorDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CTileMapEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


