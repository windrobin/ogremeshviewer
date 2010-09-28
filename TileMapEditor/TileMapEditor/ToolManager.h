#pragma once

enum EToolType
{
	eToolSelect,
	eToolBrush,
	eToolDelete,
	eToolMax
};

class ToolBase;

class ToolManager : public Cactus::Singleton<ToolManager>
{
public:
	ToolManager();
	~ToolManager();

	ToolBase*			SelectTool(EToolType e);
	ToolBase*			GetCurrentTool();

protected:

	EToolType			_eCurToolType;

	typedef Cactus::map<EToolType, ToolBase*>::type		TypToolMapType;
	TypToolMapType		_tools;

};
