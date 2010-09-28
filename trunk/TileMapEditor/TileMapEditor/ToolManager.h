#pragma once

enum EToolType
{
	eToolSelect,
	eToolBrush,
	eToolDelete,
	eToolMax
};

class ToolBase;
class CTileMapEditorDoc;

class ToolManager : public Cactus::Singleton<ToolManager>
{
public:
	ToolManager();
	~ToolManager();

	void				SetDocument(CTileMapEditorDoc* pDoc);
	CTileMapEditorDoc*	GetDocument(){ return _pDoc; }

	ToolBase*			SelectTool(EToolType e);
	ToolBase*			GetCurrentTool();

protected:

	CTileMapEditorDoc*	_pDoc;

	EToolType			_eCurToolType;

	typedef Cactus::map<EToolType, ToolBase*>::type		TypToolMapType;
	TypToolMapType		_tools;

};
