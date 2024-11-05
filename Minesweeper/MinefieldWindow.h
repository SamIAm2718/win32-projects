#pragma once
#include "BaseWindow.h"
#include "MinefieldScene.h"

#include <memory>
#include <string>
#include <vector>

#include <windef.h>

#include "MineTile.h"
#include "RNG.h"

class GameWindow;

class MinefieldWindow : public BaseWindow<MinefieldWindow>
{
public:
	MinefieldWindow(UINT width, UINT height, UINT cMines);

	UINT GetMinefieldWidth() const;							// Returns the width of the Minefield.
	UINT GetMinefieldHeight() const;						// Returns the height of the Minefield.
	UINT GetMinefieldSize() const;							// Returns the number of tiles in the Minefield

	BOOL IsGameLost() const;								// Returns if the game is lost.
	BOOL IsGameWon() const;									// Returns if the game is won.
	BOOL IsGameActive() const;								// Returns if game is active or not.
	BOOL Resize(UINT width, UINT height, UINT cMines);		// Resizes the Minefield to width x height with cMines mines.

	MineTile& operator()(UINT index);						// Get tile from tile array at a given index.
	MineTile& operator()(UINT x, UINT y);					// Get tile from tile matrix at given position (x, y)

	void ToggleQuestionMarkUsage();							// Toggles whether question marks are enabled or disabled.
	void ResetGame();										// Resets the game.

private:
	std::unique_ptr<WCHAR[]> m_lpszClassName{ nullptr };	// Pointer to string holding window class name.
	GameWindow* m_pGameWindow{ nullptr };					// Pointer to owning Minesweeper game window.
	UINT m_width{ 0 };										// Width of Minefield.
	UINT m_height{ 0 };										// Height of Minefield.
	UINT m_cTiles{ 0 };										// Count of tiles in Minefield.
	UINT m_cMines{ 0 };										// Count of mines to generate.
	UINT m_cRevealedTiles{ 0 };								// Tracks number of tiles set to revealed.
	UINT m_cFlaggedTiles{ 0 };								// Tracks number of tiles marked with flags.
	POINT m_lastGridPos{};									// Tracks the previous position of the mouse on tile grid.
	BOOL m_bMouseTracking{ FALSE };							// Tracks if mouse is being tracked.
	BOOL m_bChording{ FALSE };								// Tracks if player is currently chording.
	BOOL m_bLRHeldAfterChord{ FALSE };						// Tracks if player is still holding L or R mouse button after chord
	BOOL m_bGameLost{ FALSE };								// Tracks if game is lost, i.e. a mine was revealed.
	BOOL m_bQuestionMarksEnabled{ FALSE };					// Tracks if we can mark with question marks.
	RNG m_rng{};											// The RNG used for generating mine positions.
	MinefieldScene m_scene{};								// Object responsible for rendering graphics.
	std::vector<MineTile> m_aMineTiles{};					// Vector holding tiles in the grid.

	UINT GetNumberAdjacentMines(UINT x, UINT y);
	std::vector<UINT> GetTileGrid(UINT x, UINT y, INT radius);
	POINT MouseToTilePos(LPARAM lParam);
	void GenerateMines(UINT x, UINT y);
	void GenerateNumbers();
	void SetTileRevealed(UINT x, UINT y);
	void BeginChord(UINT x, UINT y);
	void EndChord(UINT x, UINT y);
	void MovePos(POINT oldPos, POINT newPos, UINT tileUpdateRadius, BOOL forceUpdate);

	// Functions that handle different user inputs.
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnMButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnMButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

public:
	// Returns Window class name to satisfy BaseWindow
	LPCTSTR ClassName() const
	{
		return m_lpszClassName.get();
	}

	// Returns default cursor to satisfy BaseWindow
	HCURSOR DefaultCursor() const
	{
		return LoadCursor(nullptr, IDC_HAND);
	}

	// Window procedure to handle messages
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
