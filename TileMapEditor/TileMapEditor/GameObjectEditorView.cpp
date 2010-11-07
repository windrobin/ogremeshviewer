#include "stdafx.h"
#include "GameObjectEditorView.h"

#include "GameObjectEditorObjects.h"
#include "ToolManager.h"
#include "Map.h"

#include "MemDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(GameObjectEditorView, CScrollView)

BEGIN_MESSAGE_MAP(GameObjectEditorView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

// GameObjectEditorView ����/����

GameObjectEditorView::GameObjectEditorView()
{
}

GameObjectEditorView::~GameObjectEditorView()
{
}

BOOL GameObjectEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// GameObjectEditorView ����

void GameObjectEditorView::OnDraw(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	MemDC memDC(pDC);
	memDC.FillSolidRect(rcClient, RGB(255, 255, 255));

	Map* pMap = ToolManager::getSingleton().GetMap();
	if (pMap)
	{
		CDialogGameObject* pDlg = GetGODlg();

		CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
		CPen* pOldPen = memDC.SelectObject(&pen);

		int iMapW = pDlg->_iTileW * pDlg->_iTileCountX;
		int iMapH = pDlg->_iTileH * pDlg->_iTileCountY;

		if (pMap->GetType() == eRectangle)
		{
			//������
			for (int i = 0; i <= pDlg->_iTileCountY; i++)
			{
				memDC.MoveTo(0, i * pDlg->_iTileH);
				memDC.LineTo(iMapW, i * pDlg->_iTileH);
			}

			//������
			for (int i = 0; i <= pDlg->_iTileCountX; i++)
			{
				memDC.MoveTo(i * pDlg->_iTileW, 0);
				memDC.LineTo(i * pDlg->_iTileW, iMapH);
			}
		}
		else
		{
			//��б����
			for (int i = 0; i <= pDlg->_iTileCountY; i++)
			{
				memDC.MoveTo(iMapW/2 - i * pDlg->_iTileW / 2
					, i * pDlg->_iTileH / 2);

				memDC.LineTo(iMapW - i * pDlg->_iTileW / 2
					, iMapH/2 + i * pDlg->_iTileH / 2);
			}

			//��б����
			for (int i = 0; i <= pDlg->_iTileCountX; i++)
			{
				memDC.MoveTo(i * pDlg->_iTileW / 2
					, iMapH/2 + i * pDlg->_iTileH / 2);

				memDC.LineTo(iMapW/2 + i * pDlg->_iTileW / 2
					, i * pDlg->_iTileH / 2);
			}
		}

		memDC.SelectObject(pOldPen);
	}
}

void GameObjectEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx	= 500;
	sizeTotal.cy	= 500;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

// GameObjectEditorView ��Ϣ�������

void GameObjectEditorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void GameObjectEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);
}

void GameObjectEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonUp(nFlags, point);
}

void GameObjectEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);
}

BOOL GameObjectEditorView::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC);

	return TRUE;
}

int GameObjectEditorView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
