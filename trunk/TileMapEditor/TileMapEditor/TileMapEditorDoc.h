
// TileMapEditorDoc.h : CTileMapEditorDoc 类的接口
//


#pragma once


class CTileMapEditorDoc : public CDocument
{
protected: // 仅从序列化创建
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


