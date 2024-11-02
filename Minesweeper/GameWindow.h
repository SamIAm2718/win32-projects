#pragma once
#include "BaseWindow.h"
#include "MinefieldWindow.h"
#include "GameInfoBarWindow.h"
#include "GameOptionsDialog.h"
#include "BorderScene.h"

#include <memory>

#include "enums.h"

class GameWindow : public BaseWindow<GameWindow>
{
public:
	GameWindow(UINT width, UINT height, UINT cMines);

	void ToggleQuestionMarkUsage();
	void ResetGame();
	void ResizeMinefield(UINT width, UINT height, UINT cMines);
	void SetFlagCounter(INT32 count);
	void StartTimer();
	void StopTimer();
	void ResetTimer();
	void SetSmileState(SmileState state);
	void SetCurrentTileContents(TileContent content);

	DOUBLE GetTileSize();
	RECT MinefieldBoundingBox();
	RECT InfoBarBoundingBox();

private:
	std::unique_ptr<WCHAR[]> m_lpszClassName{ nullptr };
	MinefieldWindow m_field;
	GameInfoBarWindow m_infobar{};
	GameOptionsDialog m_gameOptionsDialog{};
	BorderScene m_border{};
	DOUBLE m_dTileSize{};
	BOOL m_bFirstDraw{ TRUE };

	void UpdateTileSize();

public:
	// Returns Window class name to satisfy BaseWindow
	LPCTSTR ClassName() const
	{
		return m_lpszClassName.get();
	}

	// Returns default cursor to satisfy BaseWindow
	HCURSOR DefaultCursor() const
	{
		return LoadCursor(nullptr, IDC_ARROW);
	}

	// Window procedure to handle messages
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};