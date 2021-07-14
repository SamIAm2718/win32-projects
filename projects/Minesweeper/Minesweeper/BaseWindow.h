#pragma once
#include <Windows.h>

template <class DERIVED_TYPE>
class BaseWindow
{
public:
	BaseWindow() : m_hWnd(nullptr) {}
	virtual ~BaseWindow() {}

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = nullptr;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<DERIVED_TYPE*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
			pThis->m_hWnd = hWnd;
		}
		else
		{
			pThis = reinterpret_cast<DERIVED_TYPE*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, HWND hWndParent = 0, HMENU hMenu = 0)
	{
		WNDCLASSEX wc{};

		if (!GetClassInfoEx(GetModuleHandle(nullptr), ClassName(), &wc))
		{
			wc.cbSize = sizeof(wc);
			wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
			wc.hInstance = GetModuleHandle(nullptr);
			wc.hCursor = DefaultCursor();
			wc.hbrBackground = DefaultBackground();
			wc.lpszClassName = ClassName();
			RegisterClassEx(&wc);
		}

		m_hWnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu,
			GetModuleHandle(nullptr), this);

		return (m_hWnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hWnd; }

protected:
	virtual LPCTSTR ClassName() const = 0;
	virtual HCURSOR DefaultCursor() const { return static_cast<HCURSOR>(0); }
	virtual HBRUSH DefaultBackground() const { return reinterpret_cast<HBRUSH>(COLOR_WINDOW); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hWnd{ nullptr };
};