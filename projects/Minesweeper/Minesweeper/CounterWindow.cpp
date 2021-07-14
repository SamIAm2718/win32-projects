#include "CounterWindow.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

CounterWindow::CounterWindow(INT32 initialCount)
{
	m_lpszClassName = std::make_unique<WCHAR[]>(constants::MAX_LOADSTRING);
	LoadString(GetModuleHandle(nullptr), IDS_COUNTER_CLASS, m_lpszClassName.get(), constants::MAX_LOADSTRING);
	m_scene.SetCounter(initialCount);
}

/*
*	============================
*	===== Window Procedure =====
*	============================
*/

LRESULT CounterWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
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

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	return TRUE;
}

void CounterWindow::SetCounter(INT32 count)
{
	m_scene.SetCounter(count);
	m_scene.Render();
}