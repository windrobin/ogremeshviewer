
// TileMapEditorDoc.h : CTileMapEditorDoc ��Ľӿ�
//


#pragma once


class CTileMapEditorDoc : public CDocument
{
protected: // �������л�����
	CTileMapEditorDoc();
	DECLARE_DYNCREATE(CTileMapEditorDoc)


public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);


public:
	virtual ~CTileMapEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


protected:
	DECLARE_MESSAGE_MAP()
};


