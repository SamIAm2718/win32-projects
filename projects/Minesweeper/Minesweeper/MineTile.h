#pragma once
#include <atlbase.h>
#include <d2d1.h>
#include <windef.h>

#include "enums.h"

class MineTile
{
public:
	MineTile() :
		m_tileState{ TileState::HIDDEN },
		m_tileContent{ TileContent::EMPTY },
		m_tileMark{ TileMark::NONE },
		m_drawRect{ D2D1::RectF(0,0,0,0) } {}

	TileState GetTileState() const { return m_tileState; }
	void SetTileState(TileState state) { m_tileState = state; }

	TileContent GetTileContent() const { return m_tileContent; }
	void SetTileContent(TileContent content) { m_tileContent = content; }

	TileMark GetTileMark() const { return m_tileMark; }
	void SetTileMark(TileMark mark) { m_tileMark = mark; }

	const D2D1_RECT_F& GetDrawRect() const { return m_drawRect; }

	void SetDrawRect(D2D1_RECT_F drawRect)
	{
		m_drawRect = drawRect;
	}

	void SetDrawRect(float left, float top, float nWidth, float nHeight)
	{
		m_drawRect = D2D1::RectF(left, top, left + nWidth, top + nHeight);
	}

private:
	TileState m_tileState{ TileState::HIDDEN };
	TileContent m_tileContent{ TileContent::EMPTY };
	TileMark m_tileMark{ TileMark::NONE };

	/*
	*	Stores rectangle that contains the tile position in
	*	the Minefield window for both drawing purposes and
	*	to determine if the mouse cursor is on the tile
	*/
	D2D1_RECT_F m_drawRect{ D2D1::RectF(0,0,0,0) };
};