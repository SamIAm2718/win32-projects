#pragma once
#include "BaseDialog.h"

#include <Windows.h>

#include "constants.h"

class GameWindow;

class GameOptionsDialog : public BaseDialog<GameOptionsDialog>
{
public:
	GameOptionsDialog();

private:
	GameWindow* m_pGameWindow{ nullptr };
	BOOL m_bQuestionMarksEnabled{ FALSE };
	LRESULT m_uCurrentDifficultyIndex{ 0 };
	UINT m_uCurrentWidth{ constants::BEGINNER_WIDTH };
	UINT m_uCurrentHeight{ constants::BEGINNER_HEIGHT };
	UINT m_uCurrentMines{ constants::BEGINNER_CMINES };

	void InitDifficultyList();

public:
	UINT DialogID() { return IDD_OPTIONS; }
	INT_PTR HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
