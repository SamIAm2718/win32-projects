#include "SmileWindow.h"

#include "constants.h"
#include "GameInfoBarWindow.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

SmileWindow::SmileWindow()
{
	m_lpszClassName = std::make_unique<WCHAR[]>(constants::MAX_LOADSTRING);
	LoadString(GetModuleHandle(nullptr), IDS_SMILE_CLASS, m_lpszClassName.get(), constants::MAX_LOADSTRING);
}

void SmileWindow::SetSmileState(SmileState state)
{
	m_scene.SetSmileState(state);
	m_scene.Render();
}

void SmileWindow::SetCurrentTileContent(TileContent content)
{
	m_scene.SetCurrentTileContent(content);

	if (IsDebugEnabled())
	{
		m_scene.Render();
	}
}

void SmileWindow::ToggleDebug()
{
	m_scene.ToggleDebug();
	m_scene.Render();
}

void SmileWindow::ResetGame()
{
	m_scene.ResetGame();
}

BOOL SmileWindow::isClicked()
{
	return m_scene.IsClicked();
}

BOOL SmileWindow::IsDebugEnabled()
{
	return m_scene.IsDebugEnabled();
}

/*
*	==========================
*	===== Input Handlers =====
*	==========================
*/

LRESULT SmileWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_scene.SetClicked();
	m_scene.Render();
	return 0;
}

LRESULT SmileWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if (m_scene.IsClicked())
	{
		m_pGameInfoBarWindow->ResetGame();
		m_scene.UnsetClicked();
		m_scene.Render();
	}

	return 0;
}

LRESULT SmileWindow::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;

		if (TrackMouseEvent(&tme))
		{
			m_bMouseTracking = TRUE;
		}
	}
	else
	{
		if (wParam & MK_LBUTTON)
		{
			if (!m_scene.IsClicked())
			{
				m_scene.SetClicked();
				m_scene.Render();
			}
		}
	}

	return 0;
}

LRESULT SmileWindow::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_scene.IsClicked())
	{
		m_scene.UnsetClicked();
		m_scene.Render();
	}

	m_bMouseTracking = FALSE;

	return 0;
}

/*
*	============================
*	===== Window Procedure =====
*	============================
*/

LRESULT SmileWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		m_pGameInfoBarWindow = reinterpret_cast<GameInfoBarWindow*>(GetWindowLongPtr(GetParent(m_hWnd), GWLP_USERDATA));
		if (!m_pGameInfoBarWindow)
		{
			return -1;
		}

		if (FAILED(m_scene.Initialize(m_hWnd)))
		{
			return -1;
		}
		return 0;

	case WM_DESTROY:
		m_scene.CleanUp();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		[[fallthrough]];
	case WM_DISPLAYCHANGE:
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		m_scene.Render();
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_SIZE:
	{
		int x = (int)(short)LOWORD(lParam);
		int y = (int)(short)HIWORD(lParam);
		m_scene.Resize(x, y);
		InvalidateRect(m_hWnd, nullptr, FALSE);
	}
	return 0;

	case WM_ERASEBKGND:
		return 1;

	case WM_LBUTTONDOWN:
		return OnLButtonDown(wParam, lParam);

	case WM_LBUTTONUP:
		return OnLButtonUp(wParam, lParam);

	case WM_MOUSEMOVE:
		return OnMouseMove(wParam, lParam);

	case WM_MOUSELEAVE:
		return OnMouseLeave(wParam, lParam);

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	return TRUE;
}