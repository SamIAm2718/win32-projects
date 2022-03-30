#pragma once
#include "BaseWindow.h"
#include "CounterScene.h"

#include <memory>

class CounterWindow : public BaseWindow<CounterWindow>
{
public:
	CounterWindow(INT32 initialCount = 0);

	void SetCounter(INT32 count);

private:
	std::unique_ptr<WCHAR[]> m_lpszClassName{ nullptr };
	CounterScene m_scene{};

public:
	LPCTSTR ClassName() const
	{
		return m_lpszClassName.get();
	}

	HCURSOR DefaultCursor() const
	{
		return LoadCursor(nullptr, IDC_ARROW);
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

