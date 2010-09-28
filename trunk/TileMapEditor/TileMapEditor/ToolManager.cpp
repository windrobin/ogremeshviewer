#include "StdAfx.h"
#include "ToolManager.h"

#include "ToolSelect.h"
#include "ToolBrush.h"
#include "ToolDelete.h"

using namespace Cactus;

ToolManager* Cactus::Singleton<ToolManager>::s_pSingleton = 0;

ToolManager::ToolManager()
: _eCurToolType(eToolSelect)
{
	_tools[eToolSelect]	= new ToolSelect;
	_tools[eToolBrush]	= new ToolSelect;
	_tools[eToolDelete]	= new ToolDelete;
}

ToolManager::~ToolManager()
{
	for (TypToolMapType::iterator it = _tools.begin(); it != _tools.end(); ++it)
	{
		delete it->second;
	}
}


ToolBase* ToolManager::SelectTool(EToolType e)
{
	if (e != _eCurToolType)
	{
		_eCurToolType = e;
	}

	return _tools[e];
}

ToolBase* ToolManager::GetCurrentTool()
{
	return _tools[_eCurToolType];
}
