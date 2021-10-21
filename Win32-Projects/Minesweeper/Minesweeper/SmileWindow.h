#pragma once
#include "BaseWindow.h"
#include "SmileScene.h"

#include <memory>

class GameInfoBarWindow;

class SmileWindow : public BaseWindow<SmileWindow>
{
public:
	SmileWindow();

	void SetSmileState(SmileState state);
	BOOL isClicked();

private:
	std::unique_ptr<WCHAR[]> m_lpszClassName{ nullptr };
	GameInfoBarWindow* m_pGameInfoBarWindow{ nullptr };
	BOOL m_bMouseTracking{ FALSE };
	SmileScene m_scene{};

	// Functions that handle different user inputs.
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

public:
	LPCTSTR ClassName() const
	{
		return m_lpszClassName.get();
	}

	HCURSOR DefaultCursor() const
	{
		return LoadCursor(nullptr, IDC_HAND);
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};