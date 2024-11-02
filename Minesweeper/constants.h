#pragma once
#include <stddef.h>

#include <string_view>
#include <WinDef.h>

#include "resource.h"

namespace constants
{
	inline constexpr size_t MAX_LOADSTRING{ 100 };

	inline constexpr UINT BEGINNER_WIDTH{ 9 };
	inline constexpr UINT BEGINNER_HEIGHT{ 9 };
	inline constexpr UINT BEGINNER_CMINES{ 10 };

	inline constexpr UINT INTERMEDIATE_WIDTH{ 16 };
	inline constexpr UINT INTERMEDIATE_HEIGHT{ 16 };
	inline constexpr UINT INTERMEDIATE_CMINES{ 40 };

	inline constexpr UINT EXPERT_WIDTH{ 30 };
	inline constexpr UINT EXPERT_HEIGHT{ 16 };
	inline constexpr UINT EXPERT_CMINES{ 99 };;

	inline constexpr UINT MAX_FIELD_DIMENSION{ 99 };

	inline constexpr UINT COUNTER_SIZE{ 5 };

	inline constexpr UINT DIGIT_STATES[]{ 0b01110111, 0b00100100, 0b01011101, 0b01101101, 0b00101110, 0b01101011, 
											0b01111011, 0b00100101, 0b01111111, 0b01101111, 0b00001000 };

	inline constexpr std::wstring_view FONT_NUMBER{ L"Cambria Math" };
	inline constexpr std::wstring_view FONT_EMOJI{ L"Segoe UI Emoji" };

	inline constexpr std::wstring_view CHAR_QUESTION_MARK{ L"?" };
	inline constexpr std::wstring_view CHAR_ONE{ L"1" };
	inline constexpr std::wstring_view CHAR_TWO{ L"2" };
	inline constexpr std::wstring_view CHAR_THREE{ L"3" };
	inline constexpr std::wstring_view CHAR_FOUR{ L"4" };
	inline constexpr std::wstring_view CHAR_FIVE{ L"5" };
	inline constexpr std::wstring_view CHAR_SIX{ L"6" };
	inline constexpr std::wstring_view CHAR_SEVEN{ L"7" };
	inline constexpr std::wstring_view CHAR_EIGHT{ L"8" };

	inline constexpr std::wstring_view EMOJI_BOMB{ L"💣" };
	inline constexpr std::wstring_view EMOJI_FLAG{ L"🚩" };
	inline constexpr std::wstring_view EMOJI_SMILE{ L"🙂" };
	inline constexpr std::wstring_view EMOJI_SMILE_OPEN_MOUTH{ L"😮" };
	inline constexpr std::wstring_view EMOJI_SMILE_SUNGLASSES{ L"😎" };
	inline constexpr std::wstring_view EMOJI_SMILE_DEAD{ L"💀" };
}