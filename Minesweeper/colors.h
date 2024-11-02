#pragma once
#include <windef.h>

// Macro to convert color given in RGBA hex format to sRGB for use with D2D1::ColorF
#define RGBA(color) (((color & (0xFF << 24)) >> 24) / 255.0f), (((color & (0xFF << 16)) >> 16) / 255.0f), \
					(((color & (0xFF << 8)) >> 8) / 255.0f), ((color & 0xFF) / 255.0f)

// Colors are given in an RGBA hex code
namespace colors
{
	// Helper to convert color given in RGBA hex format to COLORREF
	constexpr COLORREF RGBA_TO_COLORREF(const unsigned int color)
	{
		return static_cast<COLORREF>(RGB((color & (0xFF << 24)) >> 24, (color & (0xFF << 16)) >> 16, \
			(color & (0xFF << 8)) >> 8));
	}

	// Colors related to tile background
	inline constexpr unsigned int tileBackground{ 0xC0C0C0FF };
	inline constexpr unsigned int tileBackgroundMineReveal{ 0xFF0000FF };
	inline constexpr unsigned int tileBackgroundMineGameWin{ 0x008000FF };
	inline constexpr unsigned int tileBackgroundCheatsUsed{ 0x000080FF };

	// Alpha masks for tile edge colors
	inline constexpr unsigned int tileEdgeLightest{ 0xFFFFFF87 };
	inline constexpr unsigned int tileEdgeLight{ 0xFFFFFF5D };
	inline constexpr unsigned int tileEdgeDark{ 0x00000087 };
	inline constexpr unsigned int tileEdgeDarkest{ 0x00000095 };

	// Explicit tile edge colors assuming that the background color is 0xC0C0C0FF
	//inline constexpr unsigned int tileEdgeLightest{0xe1e1e1ff};
	//inline constexpr unsigned int tileEdgeLight{ 0xd7d7d7ff };
	//inline constexpr unsigned int tileEdgeDark{ 0x5a5a5aff };
	//inline constexpr unsigned int tileEdgeDarkest{ 0x505050ff };

	// Colors related to tile text
	inline constexpr unsigned int tileOne{ 0x0001FDFF };
	inline constexpr unsigned int tileTwo{ 0x017E00FF };
	inline constexpr unsigned int tileThree{ 0xFF0000FF };
	inline constexpr unsigned int tileFour{ 0x010180FF };
	inline constexpr unsigned int tileFive{ 0x810101FF };
	inline constexpr unsigned int tileSix{ 0x008080FF };
	inline constexpr unsigned int tileSeven{ 0x000000FF };
	inline constexpr unsigned int tileEight{ 0x808080FF };
	inline constexpr unsigned int tileQuestionMark{ 0x000000FF };

	// Colors related to counter
	inline constexpr unsigned int digitLEDOff{ 0x3B0000FF };
	inline constexpr unsigned int digitLEDOn{ 0xFF0000FF };
	inline constexpr unsigned int counterBackground{ 0x000000FF };
}