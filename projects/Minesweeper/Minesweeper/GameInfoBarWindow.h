#pragma once
#include "BaseWindow.h"
#include "CounterWindow.h"
#include "SmileWindow.h"

#include <memory>

#include "colors.h"
#include "enums.h"

class GameWindow;

class GameInfoBarWindow : public BaseWindow<GameInfoBarWindow>
{
public:
	GameInfoBarWindow(int cMines = 0, int iTime = 0);

	RECT FlagCounterBoundingBox();
	RECT TimerBoundingBox();
	RECT SmileBoundingBox();
	void ResetGame();
	void SetFlagCounter(INT32 count);
	void StartTimer();
	void StopTimer();
	void ResetTimer();
	void SetSmileState(SmileState state);

private:
	std::unique_ptr<WCHAR[]> m_lpszClassName{ nullptr };
	GameWindow* m_pGameWindow{ nullptr };
	UINT_PTR m_pTimer{};
	BOOL m_bTimerRunning{};
	BOOL m_bFirstDraw{ TRUE };
	INT32 m_iElapsedTime{};
	CounterWindow m_flagCounter{};
	CounterWindow m_timerCounter{};
	SmileWindow m_smile{};

public:
	LPCTSTR ClassName() const
	{
		return m_lpszClassName.get();
	}

	HCURSOR DefaultCursor() const
	{
		return LoadCursor(nullptr, IDC_ARROW);
	}

	HBRUSH DefaultBackground() const
	{
		return CreateSolidBrush(colors::RGBA_TO_COLORREF(colors::tileBackground));
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
