#pragma once

#include <Windows.h>

template<class DERIVED_TYPE>
class BaseDialog
{
protected:
	HWND m_hDlg{ nullptr };

	virtual INT_PTR HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual UINT DialogID() = 0;

	static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = nullptr;

		if (uMsg == WM_INITDIALOG)
		{
			pThis = reinterpret_cast<DERIVED_TYPE*>(lParam);
			SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
			pThis->m_hDlg = hDlg;
		}
		else
		{
			pThis = reinterpret_cast<DERIVED_TYPE*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return static_cast<INT_PTR>(FALSE);
		}
	}

public:
	BaseDialog() {}
	virtual ~BaseDialog() {}

	void OpenDialog(HWND hWnd)
	{
		DialogBoxParam(GetModuleHandle(nullptr), MAKEINTRESOURCE(DialogID()), hWnd, DialogProc, reinterpret_cast<LPARAM>(this));
	}
};