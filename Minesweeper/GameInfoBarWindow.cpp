#include "GameInfoBarWindow.h"

#include "colors.h"
#include "constants.h"
#include "GameWindow.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

GameInfoBarWindow::GameInfoBarWindow(int cMines, int iTime) : m_flagCounter{ cMines }, m_timerCounter{ iTime }
{
	m_lpszClassName = std::make_unique<WCHAR[]>(constants::MAX_LOADSTRING);
	LoadString(GetModuleHandle(nullptr), IDS_GAME_INFOBAR_CLASS, m_lpszClassName.get(), constants::MAX_LOADSTRING);
}

RECT GameInfoBarWindow::FlagCounterBoundingBox()
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	
	DOUBLE boxUnit = rc.bottom / 6.;
	rc.top += static_cast<LONG>(boxUnit + 0.5);
	rc.bottom -= static_cast<LONG>(boxUnit + 0.5);

	DOUBLE digitWidth{ (static_cast<DOUBLE>(rc.bottom) - rc.top) / 2 };
	rc.left += static_cast<LONG>(boxUnit + 0.5);
	rc.right = static_cast<LONG>(rc.left + constants::COUNTER_SIZE * digitWidth + 0.5);

	return rc;
}

RECT GameInfoBarWindow::TimerBoundingBox()
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);

	DOUBLE boxUnit = rc.bottom / 6.;
	rc.top += static_cast<LONG>(boxUnit + 0.5);
	rc.bottom -= static_cast<LONG>(boxUnit + 0.5);

	DOUBLE digitWidth{ (static_cast<DOUBLE>(rc.bottom) - rc.top) / 2 };
	rc.right -= static_cast<LONG>(boxUnit + 0.5);
	rc.left = static_cast<LONG>(rc.right - constants::COUNTER_SIZE * digitWidth + 0.5);

	return rc;
}

RECT GameInfoBarWindow::SmileBoundingBox()
{
	RECT rc;

	GetClientRect(m_hWnd, &rc);

	DOUBLE boxUnit = rc.bottom / 6.;
	rc.top += static_cast<LONG>(boxUnit + 0.5);
	rc.bottom -= static_cast<LONG>(boxUnit + 0.5);

	DOUBLE height{ static_cast<DOUBLE>(rc.bottom) - rc.top };
	DOUBLE center{ (static_cast<DOUBLE>(rc.right) - rc.left) / 2 };
	
	rc.left = static_cast<LONG>(center - height / 2);
	rc.right = static_cast<LONG>(center + height / 2);

	return rc;
}

BOOL GameInfoBarWindow::IsDebugEnabled()
{
	return m_smile.IsDebugEnabled();
}

void GameInfoBarWindow::ResetGame()
{
	m_pGameWindow->ResetGame();
	m_smile.ResetGame();
}

void GameInfoBarWindow::SetFlagCounter(INT32 count) 
{
	m_flagCounter.SetCounter(count); 
}

void GameInfoBarWindow::StartTimer() 
{
	m_bTimerRunning = TRUE;
}

void GameInfoBarWindow::StopTimer() 
{
	m_bTimerRunning = FALSE; 
}

void GameInfoBarWindow::ResetTimer()
{
	m_iElapsedTime = 0;
	m_timerCounter.SetCounter(0);
}

void GameInfoBarWindow::ToggleDebug()
{
	m_smile.ToggleDebug();
}

void GameInfoBarWindow::SetSmileState(SmileState state)
{
	m_smile.SetSmileState(state);
}

void GameInfoBarWindow::SetCurrentTileContent(TileContent content)
{
	m_smile.SetCurrentTileContent(content);
}

/*
*	============================
*	===== Window Procedure =====
*	============================
*/

LRESULT GameInfoBarWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		m_pGameWindow = reinterpret_cast<GameWindow*>(GetWindowLongPtr(GetParent(m_hWnd), GWLP_USERDATA));
		if (!m_pGameWindow)
		{
			return -1;
		}

		RECT rc{ FlagCounterBoundingBox() };
		if (!m_flagCounter.Create(L"FlagCounter", WS_CHILD | WS_VISIBLE, 0, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), m_hWnd, 0))
		{
			return -1;
		}

		rc = TimerBoundingBox();
		if (!m_timerCounter.Create(L"Timer", WS_CHILD | WS_VISIBLE, 0, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), m_hWnd, 0))
		{
			return -1;
		}

		rc = SmileBoundingBox();
		if (!m_smile.Create(L"Smile", WS_CHILD | WS_VISIBLE, 0, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), m_hWnd, 0))
		{
			return -1;
		}

		if (m_pTimer = SetTimer(m_hWnd, 1, 1000, nullptr); !m_pTimer)
		{
			return -1;
		}
	}
	return 0;

	case WM_DESTROY:
		KillTimer(m_hWnd, m_pTimer);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		[[fallthrough]];
	case WM_DISPLAYCHANGE:
	{
		PAINTSTRUCT ps;
		HDC hDc{ BeginPaint(m_hWnd, &ps) };
		FillRect(hDc, &ps.rcPaint, CreateSolidBrush(colors::RGBA_TO_COLORREF(colors::tileBackground)));
		EndPaint(m_hWnd, &ps);
		if (m_bFirstDraw)
		{
			SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN);
			SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_SHOWWINDOW);
			m_bFirstDraw = FALSE;
		}
	}
	return 0;

	case WM_TIMER:
		if (m_bTimerRunning)
		{
			m_timerCounter.SetCounter(++m_iElapsedTime);
		}
		return 0;

	case WM_SIZE:
	{
		RECT rc{ FlagCounterBoundingBox() };
		MoveWindow(m_flagCounter.Window(), rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);
		rc = TimerBoundingBox();
		MoveWindow(m_timerCounter.Window(), rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);
		rc = SmileBoundingBox();
		MoveWindow(m_smile.Window(), rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);
		GetClientRect(m_hWnd, &rc);
		InvalidateRect(m_hWnd, nullptr, FALSE);
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	return TRUE;
}