#pragma once

enum EToolType
{
	eToolSelect,
	eToolBrush,
	eToolRemove,
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
	EToolType			GetCurrentToolType(){ return _eCurToolType; }

protected:

	CTileMapEditorDoc*	_pDoc;

	EToolType			_eCurToolType;

	typedef Cactus::map<EToolType, ToolBase*>::type		TypToolMapType;
	TypToolMapType		_tools;

};
