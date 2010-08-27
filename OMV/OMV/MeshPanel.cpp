/*
-----------------------------------------------------------------------------
This source file is part of OMV(Ogre Mesh Viewer)
For the latest info, see http://code.google.com/p/ogremeshviewer/

Copyright (c) 2010 Zhang Kun(zhk.tiger@gmail.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

-----------------------------------------------------------------------------
This software also uses Microsoft Fluent UI.
License terms to copy, use or distribute the Fluent UI are available separately.  
To learn more about our Fluent UI licensing program, please visit 
http://msdn.microsoft.com/officeui.

Generally speaking, Fluent UI is free, if you do not use it make a contest software
like Office.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "MainFrm.h"
#include "MeshPanel.h"
#include "Resource.h"
#include "OMV.h"
#include "OMVDoc.h"

#include "OgreFramework.h"
#include "Actor.h"

using namespace Ogre;

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class MeshPanel;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define M_ID_TREE		(WM_USER + 10)

MeshPanel::MeshPanel()
{
	//m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

MeshPanel::~MeshPanel()
{
}

BEGIN_MESSAGE_MAP(MeshPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//ON_WM_CONTEXTMENU()
	//ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	//ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	//ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	//ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_MESH_OPEN, OnOpenMeshFile)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	//ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int MeshPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndItemTree.Create(dwViewStyle, rectDummy, this, M_ID_TREE))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//CMenu menuSort;
	//menuSort.LoadMenu(IDR_POPUP_SORT);
	//m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));
	//CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));
	//if (pButton != NULL)
	//{
	//	pButton->m_bText = FALSE;
	//	pButton->m_bImage = TRUE;
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
	//	pButton->SetMessageWnd(this);
	//}

	_hRoot = m_wndItemTree.InsertItem(_T("Root"), 0, 0);
	m_wndItemTree.SetItemState(_hRoot, TVIS_BOLD, TVIS_BOLD);

	return 0;
}


void MeshPanel::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndItemTree.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}

void MeshPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndItemTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL MeshPanel::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}


void MeshPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndItemTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void MeshPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndItemTree.SetFocus();
}

#if 0
void MeshPanel::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndItemTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void MeshPanel::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void MeshPanel::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void MeshPanel::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void MeshPanel::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void MeshPanel::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void MeshPanel::OnClassProperties()
{
	// TODO: Add your command handler code here
}
#endif

void MeshPanel::OnNewFolder()
{
}

void MeshPanel::SaveCurrentDir()
{
	char curDir[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, curDir);
	_strOldCurrentDir = curDir;
}

void MeshPanel::RestoreCurrentDir()
{
	::SetCurrentDirectory(_strOldCurrentDir.c_str());
}

Ogre::String MeshPanel::GetDlgDir(const CString& strDlgPathName)
{
	int iIndex = strDlgPathName.ReverseFind('\\');
	assert(iIndex != -1);

	return (LPCTSTR)(strDlgPathName.Left(iIndex));
}

void MeshPanel::OnLoadMeshFile(Ogre::MeshPtr& mesh, const Ogre::String& strName)
{
	HTREEITEM hMesh = m_wndItemTree.InsertItem(strName.c_str(), 1, 1, _hRoot);
	m_wndItemTree.Expand(_hRoot, TVE_EXPAND);

	BuildSharedGeometryInfo(hMesh, mesh);

	_hMaterialGroup = 0;
	BuildGeometryInfo(hMesh, mesh);

	_pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	COMVDoc* pDoc = DYNAMIC_DOWNCAST(COMVDoc, _pMainFrame->GetActiveView()->GetDocument());

#pragma region Skeleton

	if (mesh->hasSkeleton())
	{
		HTREEITEM hSkeleton = m_wndItemTree.InsertItem("Skeleton", 2, 2, hMesh);

		std::ostringstream os;
		os << "Skeleton: " << mesh->getSkeletonName();
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hSkeleton);

		_pMainFrame->GetAnimationPanel().ShowWindow(SW_SHOW);
		_pMainFrame->GetSkeletonControlPanel().BuildSkeletonAnimInfo(mesh->getName());
	}

#pragma endregion


#pragma region Bounds

	HTREEITEM hBounds = m_wndItemTree.InsertItem("Bounds", 2, 2, hMesh);
	{
		std::ostringstream os;
		const AxisAlignedBox AABB = mesh->getBounds();
		os << "Maximum: (" << AABB.getMaximum().x << ", " << AABB.getMaximum().y << ", " << AABB.getMaximum().z << ")";
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hBounds);

		os.str("");
		os << "Minimum: (" << AABB.getMinimum().x << ", " << AABB.getMinimum().y << ", " << AABB.getMinimum().z << ")";
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hBounds);

		os.str("");
		os << "BoundingSphereRadius: " << mesh->getBoundingSphereRadius();
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hBounds);
	}

#pragma endregion

	new Actor(mesh->getName());
	OgreFramework::getSingleton().SetCurrentActor( mesh->getName() );

	_pMainFrame->GetActorPanel().AddActorName(mesh->getName().c_str());

	m_wndItemTree.Expand(hMesh, TVE_EXPAND);

}

void MeshPanel::OnOpenMeshFile()
{
	return;

	/*
	CFileDialog dlg(TRUE, "mesh", 0
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, "Ogre Binary Mesh Files (*.mesh)|*.mesh|All Files (*.*)|*.*||"
		//, "Ogre Binary Mesh Files (*.mesh)|*.mesh|Ogre Mesh XML Files (*.xml)|*.xml|All Files (*.*)|*.*||"
		);

	SaveCurrentDir();
	if( dlg.DoModal() == IDOK )
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(GetDlgDir(dlg.GetPathName()).c_str(), "FileSystem", "MeshData");
		RestoreCurrentDir();

		_pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		COMVDoc* pDoc = DYNAMIC_DOWNCAST(COMVDoc, _pMainFrame->GetActiveView()->GetDocument());


		MeshPtr mesh;
		if( pDoc->LoadMeshFile(mesh, (LPCTSTR)dlg.GetPathName(), (LPCTSTR)dlg.GetFileName()) )
		{
			HTREEITEM hMesh = m_wndItemTree.InsertItem(dlg.GetFileName(), 1, 1, _hRoot);
			m_wndItemTree.Expand(_hRoot, TVE_EXPAND);

			BuildSharedGeometryInfo(hMesh, mesh);

			_hMaterialGroup = 0;
			BuildGeometryInfo(hMesh, mesh);

#pragma region Skeleton

			if (mesh->hasSkeleton())
			{
				HTREEITEM hSkeleton = m_wndItemTree.InsertItem("Skeleton", 2, 2, hMesh);

				std::ostringstream os;
				os << "Skeleton: " << mesh->getSkeletonName();
				m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hSkeleton);

				_pMainFrame->GetAnimationPanel().ShowWindow(SW_SHOW);
				_pMainFrame->GetSkeletonControlPanel().BuildSkeletonAnimInfo(mesh->getName());
			}

#pragma endregion


#pragma region Bounds

			HTREEITEM hBounds = m_wndItemTree.InsertItem("Bounds", 2, 2, hMesh);
			{
				std::ostringstream os;
				const AxisAlignedBox AABB = mesh->getBounds();
				os << "Maximum: (" << AABB.getMaximum().x << ", " << AABB.getMaximum().y << ", " << AABB.getMaximum().z << ")";
				m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hBounds);

				os.str("");
				os << "Minimum: (" << AABB.getMinimum().x << ", " << AABB.getMinimum().y << ", " << AABB.getMinimum().z << ")";
				m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hBounds);

				os.str("");
				os << "BoundingSphereRadius: " << mesh->getBoundingSphereRadius();
				m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hBounds);
			}

#pragma endregion

			new Actor(mesh->getName());
			OgreFramework::getSingleton().SetCurrentActor( mesh->getName() );

			_pMainFrame->GetActorPanel().AddActorName(mesh->getName().c_str());

			m_wndItemTree.Expand(hMesh, TVE_EXPAND);
		}
	}
	else
	{
		RestoreCurrentDir();
	}
	*/

}


void MeshPanel::BuildSharedGeometryInfo(HTREEITEM hMesh, const Ogre::MeshPtr& mesh)
{
	if (!mesh->sharedVertexData)
		return;

	HTREEITEM hSharedVertexDeclaration = m_wndItemTree.InsertItem("Shared Vertex Declaration", 2, 2, hMesh);

	VertexDeclaration *vertexDeclaration = mesh->sharedVertexData->vertexDeclaration;

	BuildVertexDeclarationInfo(hSharedVertexDeclaration, vertexDeclaration);
}

void MeshPanel::BuildVertexDeclarationInfo(HTREEITEM hVertexDecl, Ogre::VertexDeclaration* vDecl)
{
	for (size_t t = 0; t < vDecl->getElementCount(); ++t)
	{
		const VertexElement* ve = vDecl->getElement(t);

		std::ostringstream os;
		switch (ve->getSemantic())
		{
		case VES_POSITION:
			os << "VES_POSITION";
			break;
		case VES_BLEND_WEIGHTS:
			os << "VES_BLEND_WEIGHTS";
			break;
		case VES_BLEND_INDICES:
			os << "VES_BLEND_INDICES";
			break;
		case VES_NORMAL:
			os << "VES_NORMAL";
			break;
		case VES_DIFFUSE:
			os << "VES_DIFFUSE";
			break;
		case VES_SPECULAR:
			os << "VES_SPECULAR";
			break;
		case VES_TEXTURE_COORDINATES:
			os << "VES_TEXTURE_COORDINATES";
			break;
		case VES_BINORMAL:
			os << "VES_BINORMAL";
			break;
		case VES_TANGENT:
			os << "VES_TANGENT";
			break;
		}
		HTREEITEM hSemantic = m_wndItemTree.InsertItem(os.str().c_str(), 3, 3, hVertexDecl);

		os.str("");
		os << "Index: " << ve->getIndex();
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hSemantic);

		os.str("");
		os << "Offset: " << ve->getOffset();
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hSemantic);

		os.str("");
		os << "Type: ";
		switch(ve->getType())
		{
		case VET_FLOAT1:
			os << "VET_FLOAT1, size: 4";
			break;
		case VET_FLOAT2:
			os << "VET_FLOAT2, size: 8";
			break;
		case VET_FLOAT3:
			os << "VET_FLOAT3, size: 12";
			break;
		case VET_FLOAT4:
			os << "VET_FLOAT4, size: 16";
			break;
		case VET_COLOUR:
			os << "VET_COLOUR, size: 4";
			break;
		case VET_SHORT1:
			os << "VET_SHORT1, size: 2";
			break;
		case VET_SHORT2:
			os << "VET_SHORT2, size: 4";
			break;
		case VET_SHORT3:
			os << "VET_SHORT3, size: 6";
			break;
		case VET_SHORT4:
			os << "VET_SHORT4, size: 8";
			break;
		case VET_UBYTE4:
			os << "VET_FLOAT1, size: 4";
			break;
		case VET_COLOUR_ARGB:
			os << "VET_COLOUR_ARGB, size: 4";
			break;
		case VET_COLOUR_ABGR:
			os << "VET_COLOUR_ABGR, size: 4";
			break;
		}
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hSemantic);

		os.str("");
		os << "Source: " << ve->getSource();
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hSemantic);
	}
}

void MeshPanel::BuildGeometryInfo(HTREEITEM hMesh, const Ogre::MeshPtr& mesh)
{
	HTREEITEM hGeometry = m_wndItemTree.InsertItem("Geometry", 2, 2, hMesh);
	{
		std::ostringstream os;
		os << "EdgeList: " << ( mesh->isEdgeListBuilt() ? "True" : "False" );
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hGeometry);
	}

	int iSubMeshCount = 0;
	Mesh::SubMeshIterator it = mesh->getSubMeshIterator();
	while (it.hasMoreElements())
	{
		SubMesh* pSubMesh = it.getNext();

		bool bIgnoreMaterial = false;

		if(!bIgnoreMaterial && !MaterialManager::getSingleton().resourceExists(pSubMesh->getMaterialName()))
		{
			CFileDialog dlg(TRUE, "material", 0
				, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
				, "Ogre Material Files (*.material)|*.material|All Files (*.*)|*.*||"
				);

			SaveCurrentDir();
			if( dlg.DoModal() == IDOK )
			{
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(GetDlgDir(dlg.GetPathName()).c_str(), "FileSystem", "Material");
				RestoreCurrentDir();

				DataStreamPtr data = Ogre::ResourceGroupManager::getSingleton().openResource((LPCTSTR)dlg.GetPathName(), "Material");
				MaterialManager::getSingleton().parseScript(data, "Material");

				Ogre::MaterialPtr newMaterial = Ogre::MaterialManager::getSingleton().getByName(pSubMesh->getMaterialName(), "Material");
				newMaterial->load();
			}
			else
			{
				bIgnoreMaterial = true;
				RestoreCurrentDir();
			}

		}

		if (_hMaterialGroup == 0)
			_hMaterialGroup = m_wndItemTree.InsertItem("Material", 2, 2, hMesh);
		HTREEITEM hMaterial = m_wndItemTree.InsertItem(pSubMesh->getMaterialName().c_str(), 3, 3, _hMaterialGroup);

		Ogre::MaterialPtr material = MaterialManager::getSingleton().getByName(pSubMesh->getMaterialName());
		if (material.getPointer())
		{
			BuildMaterialInfo(hMaterial, material);
		}

		std::ostringstream os;
		os << "SubMesh " << iSubMeshCount << ", material: " << pSubMesh->getMaterialName();
		HTREEITEM hSubMesh = m_wndItemTree.InsertItem(os.str().c_str(), 3, 3, hGeometry);

		if (pSubMesh->useSharedVertices)
			m_wndItemTree.InsertItem("Using shared geometry data.", 7, 7, hSubMesh);
		else
			m_wndItemTree.InsertItem("Using dedicated geometry data.", 7, 7, hSubMesh);

		os.str("");
		os << "IndexData, count: " << pSubMesh->indexData->indexCount << ", start: " << pSubMesh->indexData->indexStart;
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hSubMesh);

		os.str("");
		os << "Render operation: ";
		switch(pSubMesh->operationType)
		{
		case RenderOperation::OT_POINT_LIST:
			os << "OT_POINT_LIST";
			break;
		case RenderOperation::OT_LINE_LIST:
			os << "OT_LINE_LIST";
			break;
		case RenderOperation::OT_LINE_STRIP:
			os << "OT_LINE_STRIP";
			break;
		case RenderOperation::OT_TRIANGLE_LIST:
			os << "OT_TRIANGLE_LIST";
			break;
		case RenderOperation::OT_TRIANGLE_STRIP:
			os << "OT_TRIANGLE_STRIP";
			break;
		case RenderOperation::OT_TRIANGLE_FAN:
			os << "OT_TRIANGLE_FAN";
			break;
		}
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hSubMesh);
		os.str("");

		if (pSubMesh->vertexData)
		{
			os << "VertexData, count: " << pSubMesh->vertexData->vertexCount << ", start: " << pSubMesh->vertexData->vertexStart;
			HTREEITEM hVertexData = m_wndItemTree.InsertItem(os.str().c_str(), 5, 5, hSubMesh);
			VertexDeclaration* vtDecl = pSubMesh->vertexData->vertexDeclaration;
			if (vtDecl)
			{
				BuildVertexDeclarationInfo(hVertexData, vtDecl);
			}
		}

		iSubMeshCount++;
	}

}

void MeshPanel::BuildMaterialInfo(HTREEITEM hMaterial, const Ogre::MaterialPtr& material)
{
	//_hMaterial

	std::ostringstream os;
	os << "Transparent: " << material->isTransparent();
	m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hMaterial);
	os.str("");

	os << "ReceiveShadows: " << material->getReceiveShadows();
	m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hMaterial);
	os.str("");

	os << "TransparencyCastsShadows: " << material->getTransparencyCastsShadows();
	m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hMaterial);
	os.str("");

	for (size_t t = 0; t < material->getNumTechniques(); ++t)
	{
		Technique* pTech = material->getTechnique(t);

		os << "Technical " << t;
		HTREEITEM hTech = m_wndItemTree.InsertItem(os.str().c_str(), 3, 3, hMaterial);
		os.str("");

		os << "Supported: " << (pTech->isSupported() ? "Yes" : "No");
		m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hTech);
		os.str("");

		for (size_t k = 0; k < pTech->getNumPasses(); ++k)
		{
			Pass* pPass = pTech->getPass(k);

			os << "Pass " << k;
			HTREEITEM hPass = m_wndItemTree.InsertItem(os.str().c_str(), 5, 5, hTech);
			os.str("");

			os << "Programmable: " << ( pPass->isProgrammable() ? "True" : "False");
			m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hPass);
			os.str("");

			for (size_t u = 0; u < pPass->getNumTextureUnitStates(); ++u)
			{
				os << "TextureUnit " << u;
				HTREEITEM hTex = m_wndItemTree.InsertItem(os.str().c_str(), 3, 3, hPass);
				os.str("");

				TextureUnitState* pTex = pPass->getTextureUnitState(u);

				os << "Type: ";
				switch(pTex->getTextureType())
				{
				case TEX_TYPE_1D:
					os << "TEX_TYPE_1D";
					break;
				case TEX_TYPE_2D:
					os << "TEX_TYPE_2D";
					break;
				case TEX_TYPE_3D:
					os << "TEX_TYPE_3D";
					break;
				case TEX_TYPE_CUBE_MAP:
					os << "TEX_TYPE_CUBE_MAP";
					break;
				}
				m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hTex);
				os.str("");

				for (size_t j = 0; j < pTex->getNumFrames(); ++j)
				{
					os << "Frame " << j << ": "  << pTex->getFrameTextureName(j);
					m_wndItemTree.InsertItem(os.str().c_str(), 7, 7, hTex);
					os.str("");
				}
			}
		}
	}
}


void MeshPanel::OnReset()
{
	m_wndItemTree.DeleteAllItems();
}
