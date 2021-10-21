#include "MinefieldWindow.h"

#include <algorithm>
#include <set>
#include <vector>

#include "constants.h"
#include "enums.h"
#include "GameWindow.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

/*
*	Constructs a minefield with a given width, height, and
*	number of mines. Also by default we set question mark
*	usage as disabled.
*/
MinefieldWindow::MinefieldWindow(UINT width, UINT height, UINT cMines) :
	BaseWindow<MinefieldWindow>{}, m_width{ width }, m_height{ height }, m_cTiles{ width * height }, m_cMines{ cMines },
	m_cRevealedTiles{ 0 }, m_cFlaggedTiles{ 0 }, m_bMouseTracking{ FALSE }, m_bGameLost{ FALSE },
	m_bQuestionMarksEnabled{ FALSE }, m_rng{}, m_scene{}
{
	m_lpszClassName = std::make_unique<WCHAR[]>(constants::MAX_LOADSTRING);
	LoadString(GetModuleHandle(nullptr), IDS_MINEFIELD_CLASS, m_lpszClassName.get(), constants::MAX_LOADSTRING);

	if (m_cMines > m_cTiles)
	{
		m_cMines = m_cTiles;
	}

	m_aMineTiles = std::vector<MineTile>(m_cTiles, MineTile());
}

UINT MinefieldWindow::GetMinefieldWidth() const
{
	return m_width;
}

UINT MinefieldWindow::GetMinefieldHeight() const
{
	return m_height;
}

UINT MinefieldWindow::GetMinefieldSize() const
{
	return m_cTiles;
}

BOOL MinefieldWindow::IsGameLost() const
{
	return m_bGameLost;
}

/*
*	Returns whether the minesweeper game has been won.
*	In particular, a game is won if:
*	a) the game is not currently lost
*	b) we have clicked a tile, and
*	c) all tiles not containing a mine are clicked.
*
*	Normally condition (b) would be writted as
*	m_cTiles - m_cMines <= m_cRevealedTiles,
*	but since each variable is a UINT we move m_cMines to
*	the other side of the inequality.
*/
BOOL MinefieldWindow::IsGameWon() const
{
	return !m_bGameLost && ((m_cTiles <= m_cMines + m_cRevealedTiles) && (m_cRevealedTiles > 0));
}

BOOL MinefieldWindow::Resize(UINT width, UINT height, UINT cMines)
{
	if (m_width != width || m_height != height || m_cMines != cMines)
	{
		m_width = width;
		m_height = height;
		m_cTiles = m_width * m_height;
		m_cMines = cMines;

		if (m_cMines > m_cTiles)
		{
			m_cMines = m_cTiles;
		}

		ResetGame();

		return TRUE;
	}

	return FALSE;
}

// Provides access to Mine tile at index in tile array.
MineTile& MinefieldWindow::operator()(UINT index)
{
	return m_aMineTiles[index];
}

// Provides access to Mine tile at position (x,y) in minefield.
MineTile& MinefieldWindow::operator()(UINT x, UINT y)
{
	return (*this)(x + static_cast<std::size_t>(y) * m_width);
}

/*
*	Toggles whether or not question mark usage is enabled
*	or disabled
*/
void MinefieldWindow::ToggleQuestionMarkUsage()
{
	m_bQuestionMarksEnabled = !m_bQuestionMarksEnabled;

	if (!m_bQuestionMarksEnabled)
	{
		for (UINT x{ 0 }; x < m_width; ++x)
		{
			for (UINT y{ 0 }; y < m_height; ++y)
			{
				if ((*this)(x, y).GetTileMark() == TileMark::QUESTION_MARK)
				{
					(*this)(x, y).SetTileMark(TileMark::NONE);
				}
			}
		}

		m_scene.Render();
	}
}

void MinefieldWindow::ResetGame()
{
	m_bGameLost = FALSE;
	m_cRevealedTiles = 0;
	m_cFlaggedTiles = 0;
	m_aMineTiles = std::vector<MineTile>(m_cTiles, MineTile());
	m_pGameWindow->SetFlagCounter(static_cast<INT32>(m_cMines));
	m_pGameWindow->StopTimer();
	m_pGameWindow->ResetTimer();
	m_pGameWindow->SetSmileState(SmileState::SMILE);
	m_scene.CalculateLayout();
	m_scene.Render();
}

/*
*	===========================
*	===== Private Methods =====
*	===========================
*/

/*
*	Returns the number of mines that are adjacent to the
*	that are adjacent to the tile at position (x,y)
*/
UINT MinefieldWindow::GetNumberAdjacentMines(UINT x, UINT y)
{
	UINT cAdjacentMines{ 0 };

	for (const auto tile : GetTileGrid(x, y, 1))
	{
		if ((*this)(tile).GetTileContent() == TileContent::MINE)
		{
			++cAdjacentMines;
		}
	}

	return cAdjacentMines;
}

/*
*	Returns a vector containing all tiles in a square grid
*	centered at (x, y) with a given radius
*/
std::vector<UINT> MinefieldWindow::GetTileGrid(UINT x, UINT y, INT radius)
{
	std::vector<UINT> aAdjacentTiles{};

	if (radius < 0)
	{
		return aAdjacentTiles;
	}

	aAdjacentTiles.reserve((1 + 2 * static_cast<std::size_t>(radius)) * (1 + 2 * static_cast<std::size_t>(radius)));

	for (INT xOffset{ -radius }; xOffset <= radius; ++xOffset)
	{
		if (0 <= x + xOffset && x + xOffset < m_width)
		{
			for (INT yOffset{ -radius }; yOffset <= radius; ++yOffset)
			{
				if (0 <= y + yOffset && y + yOffset < m_height)
				{
					aAdjacentTiles.push_back((x + xOffset) + (y + yOffset) * m_width);
				}
			}
		}
	}

	return aAdjacentTiles;
}

/*
*	Returns the (x,y) position in the tile grid given an
*	lParam that represents the mouse position (From WinProc)
*/

POINT MinefieldWindow::MouseToTilePos(LPARAM lParam)
{
	POINTS mousePosition = MAKEPOINTS(lParam);
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	POINT pos;
	pos.x = static_cast<LONG>((static_cast<DOUBLE>(mousePosition.x) - rc.left) / ((static_cast<DOUBLE>(rc.right) - rc.left) / m_width));
	pos.y = static_cast<LONG>((static_cast<DOUBLE>(mousePosition.y) - rc.top) / ((static_cast<DOUBLE>(rc.bottom) - rc.top) / m_height));
	pos.x = min(max(pos.x, 0), m_width - 1);
	pos.y = min(max(pos.y, 0), m_height - 1);
	return pos;
}

/*
*	Generate the Mine positions given that the the first
*	clicked tile is at positon (x,y) in the minefield grid.
*/
void MinefieldWindow::GenerateMines(UINT x, UINT y)
{
	std::vector<UINT> excludedTiles{ GetTileGrid(x, y, (m_cTiles - m_cMines < 9 ? 0 : 1)) };
	std::sort(excludedTiles.begin(), excludedTiles.end());

	std::vector<MineTile*> tilesToToggle{};
	tilesToToggle.reserve(m_aMineTiles.size());
	auto itLeft = excludedTiles.begin();
	auto itRight = excludedTiles.end();
	for (UINT tile{ 0 }; tile < m_aMineTiles.size(); ++tile)
	{
		if (itLeft == itRight || tile < *itLeft)
		{
			tilesToToggle.push_back(&m_aMineTiles[tile]);
		}
		else
		{
			++itLeft;
		}
	}

	tilesToToggle = m_rng.SampleVector(tilesToToggle, m_cMines);
	for (auto tile : tilesToToggle)
	{
		tile->SetTileContent(TileContent::MINE);
	}

	GenerateNumbers();
}

/*
*	Given a minefield with filled in mines, generates the
*	numbers that each tile should have.
*/
void MinefieldWindow::GenerateNumbers()
{
	for (UINT x{ 0 }; x < m_width; ++x)
	{
		for (UINT y{ 0 }; y < m_height; ++y)
		{
			if ((*this)(x, y).GetTileContent() != TileContent::MINE)
			{
				switch (GetNumberAdjacentMines(x, y))
				{
				case 1:
					(*this)(x, y).SetTileContent(TileContent::ONE);
					break;
				case 2:
					(*this)(x, y).SetTileContent(TileContent::TWO);
					break;
				case 3:
					(*this)(x, y).SetTileContent(TileContent::THREE);
					break;
				case 4:
					(*this)(x, y).SetTileContent(TileContent::FOUR);
					break;
				case 5:
					(*this)(x, y).SetTileContent(TileContent::FIVE);
					break;
				case 6:
					(*this)(x, y).SetTileContent(TileContent::SIX);
					break;
				case 7:
					(*this)(x, y).SetTileContent(TileContent::SEVEN);
					break;
				case 8:
					(*this)(x, y).SetTileContent(TileContent::EIGHT);
					break;
				case 0:
				default:
					(*this)(x, y).SetTileContent(TileContent::EMPTY);
					break;
				}
			}
		}
	}
}

/*
*	Sets the Tile at (x,y) as revealed and if reveals the
*	adjacent tile if said tile is empty.
*/
void MinefieldWindow::SetTileRevealed(UINT x, UINT y)
{
	if ((*this)(x, y).GetTileState() != TileState::REVEALED
		&& (*this)(x, y).GetTileMark() != TileMark::FLAG)
	{
		(*this)(x, y).SetTileState(TileState::REVEALED);
		++m_cRevealedTiles;

		if ((*this)(x, y).GetTileContent() == TileContent::MINE)
		{
			m_bGameLost = TRUE;
		}
		else if ((*this)(x, y).GetTileContent() == TileContent::EMPTY)
		{
			std::vector<UINT> tilesToProcess{};
			std::vector<UINT> newTilesToProcess{};
			tilesToProcess.push_back(x + y * m_width);

			while (tilesToProcess.size() != 0)
			{
				for (const UINT tileToProcess : tilesToProcess)
				{
					for (const UINT tile : GetTileGrid(tileToProcess% m_width, tileToProcess / m_width, 1))
					{
						if ((*this)(tile).GetTileState() != TileState::REVEALED && (*this)(tile).GetTileMark() != TileMark::FLAG)
						{
							(*this)(tile).SetTileState(TileState::REVEALED);
							++m_cRevealedTiles;

							if ((*this)(tile).GetTileContent() == TileContent::EMPTY)
							{
								newTilesToProcess.push_back(tile);
							}
						}
					}
				}

				tilesToProcess = std::move(newTilesToProcess);
			}
		}
	}
}

/*
*	==========================
*	===== Input Handlers =====
*	==========================
*/

LRESULT MinefieldWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (!(IsGameLost() || IsGameWon()))
	{
		m_pGameWindow->SetSmileState(SmileState::SMILE_OPEN_MOUTH);
		POINT gridPos{ MouseToTilePos(lParam) };
		m_lastGridPos = gridPos;
		MineTile* tile = &(*this)(gridPos.x, gridPos.y);

		if (tile->GetTileState() == TileState::HIDDEN && tile->GetTileMark() != TileMark::FLAG)
		{
			tile->SetTileState(TileState::CLICKED);
			m_scene.Render();
		}
	}

	return 0;
}

LRESULT MinefieldWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if (!(IsGameLost() || IsGameWon()))
	{
		POINT gridPos{ MouseToTilePos(lParam) };
		MineTile* tile = &(*this)(gridPos.x, gridPos.y);

		if (tile->GetTileState() == TileState::CLICKED)
		{
			if (m_cRevealedTiles == 0)
			{
				GenerateMines(gridPos.x, gridPos.y);
				m_pGameWindow->StartTimer();
			}

			SetTileRevealed(gridPos.x, gridPos.y);
			m_scene.Render();
		}

		m_pGameWindow->SetSmileState(SmileState::SMILE);

		if (IsGameLost())
		{
			m_pGameWindow->StopTimer();
			m_pGameWindow->SetSmileState(SmileState::SMILE_DEAD);
		}
		else if (IsGameWon())
		{
			m_pGameWindow->StopTimer();
			m_pGameWindow->SetFlagCounter(0);
			m_pGameWindow->SetSmileState(SmileState::SMILE_SUNGLASSES);
		}
	}

	return 0;
}

LRESULT MinefieldWindow::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (!(IsGameLost() || IsGameWon()))
	{
		POINT gridPos{ MouseToTilePos(lParam) };
		MineTile* tile = &(*this)(gridPos.x, gridPos.y);

		if (tile->GetTileState() == TileState::HIDDEN)
		{
			switch (tile->GetTileMark())
			{
			case TileMark::NONE:
				++m_cFlaggedTiles;
				tile->SetTileMark(TileMark::FLAG);
				break;
			case TileMark::FLAG:
				--m_cFlaggedTiles;
				if (m_bQuestionMarksEnabled)
				{
					tile->SetTileMark(TileMark::QUESTION_MARK);
				}
				else
				{
					tile->SetTileMark(TileMark::NONE);
				}
				break;
			case TileMark::QUESTION_MARK:
				tile->SetTileMark(TileMark::NONE);
				break;
			}

			m_pGameWindow->SetFlagCounter(static_cast<INT32>(m_cMines) - m_cFlaggedTiles);
			m_scene.Render();
		}
	}

	return 0;
}

LRESULT MinefieldWindow::OnMouseMove(WPARAM wParam, LPARAM lParam)
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

		if (wParam & MK_LBUTTON)
		{
			POINT gridPos{ MouseToTilePos(lParam) };
			m_lastGridPos = gridPos;
			MineTile* tile = &(*this)(gridPos.x, gridPos.y);

			if (tile->GetTileState() == TileState::HIDDEN && tile->GetTileMark() != TileMark::FLAG)
			{
				tile->SetTileState(TileState::CLICKED);
				m_scene.Render();
			}
		}
	}
	else
	{
		if (!(IsGameLost() || IsGameWon()))
		{
			if (wParam & MK_LBUTTON)
			{
				m_pGameWindow->SetSmileState(SmileState::SMILE_OPEN_MOUTH);

				POINT gridPos{ MouseToTilePos(lParam) };

				if (gridPos.x != m_lastGridPos.x || gridPos.y != m_lastGridPos.y)
				{
					MineTile* prevTile = &(*this)(m_lastGridPos.x, m_lastGridPos.y);
					MineTile* curTile = &(*this)(gridPos.x, gridPos.y);

					if (prevTile->GetTileState() == TileState::CLICKED)
					{
						prevTile->SetTileState(TileState::HIDDEN);
					}

					if (curTile->GetTileState() == TileState::HIDDEN && curTile->GetTileMark() != TileMark::FLAG)
					{
						curTile->SetTileState(TileState::CLICKED);
					}

					m_scene.Render();
					m_lastGridPos = gridPos;
				}
			}
		}
	}

	return 0;
}

LRESULT MinefieldWindow::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (!(IsGameLost() || IsGameWon()))
	{
		m_pGameWindow->SetSmileState(SmileState::SMILE);
		MineTile* tile = &(*this)(m_lastGridPos.x, m_lastGridPos.y);

		if (tile->GetTileState() == TileState::CLICKED)
		{
			tile->SetTileState(TileState::HIDDEN);
			m_scene.Render();
		}
	}

	m_bMouseTracking = FALSE;

	return 0;
}

/*
*	============================
*	===== Window Procedure =====
*	============================
*/

LRESULT MinefieldWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(m_scene.Initialize(m_hWnd)))
		{
			return -1;
		}

		m_pGameWindow = reinterpret_cast<GameWindow*>(GetWindowLongPtr(GetParent(m_hWnd), GWLP_USERDATA));
		if (!m_pGameWindow)
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

	case WM_RBUTTONDOWN:
		return OnRButtonDown(wParam, lParam);

	case WM_MOUSEMOVE:
		return OnMouseMove(wParam, lParam);

	case WM_MOUSELEAVE:
		return OnMouseLeave(wParam, lParam);

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	return TRUE;
}