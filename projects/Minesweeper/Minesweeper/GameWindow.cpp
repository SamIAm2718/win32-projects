#include "GameWindow.h"

#include "constants.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

GameWindow::GameWindow(UINT width, UINT height, UINT cMines) :
	m_field{ width, height, cMines }, m_infobar{ cMines }
{
	m_lpszClassName = std::make_unique<WCHAR[]>(constants::MAX_LOADSTRING);
	LoadString(GetModuleHandle(nullptr), IDS_GAME_WINDOW_CLASS, m_lpszClassName.get(), constants::MAX_LOADSTRING);
}

void GameWindow::ToggleQuestionMarkUsage()
{
	m_field.ToggleQuestionMarkUsage();
}

void GameWindow::ResetGame()
{
	m_field.ResetGame();
}

void GameWindow::ResizeMinefield(UINT width, UINT height, UINT cMines)
{
	if (m_field.Resize(width, height, cMines))
	{
		RECT rc{ MinefieldBoundingBox() };
		MoveWindow(m_field.Window(), rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);
		rc = InfoBarBoundingBox();
		MoveWindow(m_infobar.Window(), rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);
		GetClientRect(m_hWnd, &rc);
		InvalidateRect(m_hWnd, &rc, TRUE);
	}
}

void GameWindow::SetFlagCounter(INT32 count)
{ 
	m_infobar.SetFlagCounter(count); 
}

void GameWindow::StartTimer()
{
	m_infobar.StartTimer(); 
}

void GameWindow::StopTimer()
{
	m_infobar.StopTimer(); 
}

void GameWindow::ResetTimer()
{ 
	m_infobar.ResetTimer(); 
}

void GameWindow::SetSmileState(SmileState state)
{
	m_infobar.SetSmileState(state);
}

DOUBLE GameWindow::GetTileSize()
{
	return m_dTileSize;
}

RECT GameWindow::MinefieldBoundingBox()
{
	UpdateTileSize();

	RECT rc{};
	GetClientRect(m_hWnd, &rc);

	LONG width = static_cast<LONG>(m_field.GetMinefieldWidth() * m_dTileSize);
	LONG height = static_cast<LONG>(m_field.GetMinefieldHeight() * m_dTileSize);

	rc.left = (rc.right - width) / 2;
	rc.right = rc.left + width;
	rc.top = static_cast<LONG>((2 * m_dTileSize + rc.bottom) / 2) - height / 2;
	rc.bottom = rc.top + height;

	return rc;
}

RECT GameWindow::InfoBarBoundingBox()
{
	UpdateTileSize();

	RECT rc{ MinefieldBoundingBox() };

	rc.top = static_cast<LONG>(rc.top - 2 * m_dTileSize);
	rc.bottom = static_cast<LONG>(rc.top + 1.5 * m_dTileSize);

	return rc;
}

/*
*	===========================
*	===== Private Methods =====
*	===========================
*/

void GameWindow::UpdateTileSize()
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);

	LONG width{ static_cast<LONG>(m_field.GetMinefieldWidth()) };
	LONG height{ static_cast<LONG>(m_field.GetMinefieldHeight()) };
	m_dTileSize = min(static_cast<DOUBLE>(rc.right) / (0.5 + width), static_cast<DOUBLE>(rc.bottom) / (2.5 + height));
}

/*
*	============================
*	===== Window Procedure =====
*	============================
*/

LRESULT GameWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		if (FAILED(m_border.Initialize(m_hWnd)))
		{
			return -1;
		}

		RECT rc{ MinefieldBoundingBox() };
		if (!m_field.Create(L"Minefield", WS_CHILD | WS_VISIBLE, 0, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), m_hWnd, 0))
		{
			return -1;
		}

		rc = InfoBarBoundingBox();
		if (!m_infobar.Create(L"InfoBar", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), m_hWnd, 0))
		{
			return -1;
		}

		SetClassLongPtr(m_hWnd, GCLP_HICON, (LONG_PTR)LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_MINESWEEPER)));
	}
	return 0;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			DestroyWindow(m_hWnd);
			break;

		case ID_ACCELERATOR_RESET:
			[[fallthrough]];
		case ID_GAME_RESET:
		{
			if (MessageBox(m_hWnd, L"Are you sure you want to reset the game?", L"Reset Game", MB_YESNO) == IDYES)
			{
				m_field.ResetGame();
			}
		}
		break;

		case ID_GAME_OPTIONS:
			m_gameOptionsDialog.OpenDialog(m_hWnd);
			break;

		default:
			return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		}
	}
	return 0;

	case WM_DESTROY:
		m_border.CleanUp();
		PostQuitMessage(0);
		return 0;

	case WM_DPICHANGED:
	{
		const RECT* pRcNewWindow{ reinterpret_cast<RECT*>(lParam) };
		SetWindowPos(m_hWnd, nullptr, pRcNewWindow->left, pRcNewWindow->top, pRcNewWindow->right - pRcNewWindow->left,
			pRcNewWindow->bottom - pRcNewWindow->top, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	return 0;

	case WM_PAINT:
		[[fallthrough]];
	case WM_DISPLAYCHANGE:
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		m_border.Render();
		EndPaint(m_hWnd, &ps);
		if (m_bFirstDraw)
		{
			SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
			SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_SHOWWINDOW);
			m_bFirstDraw = FALSE;
		}
	}
	return 0;

	case WM_SIZE:
	{
		int x = (int)(short)LOWORD(lParam);
		int y = (int)(short)HIWORD(lParam);
		m_border.Resize(x, y);
		RECT rc{ MinefieldBoundingBox() };
		MoveWindow(m_field.Window(), rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);
		rc = InfoBarBoundingBox();
		MoveWindow(m_infobar.Window(), rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);
		GetClientRect(m_hWnd, &rc);
		InvalidateRect(m_hWnd, &rc, TRUE);
	}
	return 0;

	case WM_ERASEBKGND:
		return 1;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	return TRUE;
}