#include "StdAfx.h"
#include "ToolManager.h"

#include "ToolSelect.h"
#include "ToolBrush.h"
#include "ToolRemove.h"

#include "TileMapEditorDoc.h"

using namespace Cactus;

ToolManager* Cactus::Singleton<ToolManager>::s_pSingleton = 0;

ToolManager::ToolManager()
: _eCurToolType(eToolSelect)
, _pDoc(0)
{
	_tools[eToolSelect]	= new ToolSelect;
	_tools[eToolBrush]	= new ToolBrush;
	_tools[eToolRemove]	= new ToolRemove;

	_tools[_eCurToolType]->OnTurnOn();
}

ToolManager::~ToolManager()
{
	for (TypToolMapType::iterator it = _tools.begin(); it != _tools.end(); ++it)
	{
		delete it->second;
	}
}

ToolBase* ToolManager::GetTool(EToolType e)
{
	return _tools[e];
}


void ToolManager::SetDocument(CTileMapEditorDoc* pDoc)
{
	_pDoc = pDoc;
}

ToolBase* ToolManager::SelectTool(EToolType e)
{
	if (e != _eCurToolType)
	{
		_tools[_eCurToolType]->OnTurnOff();

		_eCurToolType = e;

		_tools[_eCurToolType]->OnTurnOn();
	}

	return _tools[_eCurToolType];
}

ToolBase* ToolManager::GetCurrentTool()
{
	return _tools[_eCurToolType];
}

