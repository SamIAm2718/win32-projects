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

	inline constexpr std::wstring_view BOMB_CHAR{ L"💣" };
	inline constexpr std::wstring_view FLAG_CHAR{ L"🚩" };
	inline constexpr std::wstring_view QUESTION_MARK_CHAR{ L"?" };
	inline constexpr std::wstring_view ONE_CHAR{ L"1" };
	inline constexpr std::wstring_view TWO_CHAR{ L"2" };
	inline constexpr std::wstring_view THREE_CHAR{ L"3" };
	inline constexpr std::wstring_view FOUR_CHAR{ L"4" };
	inline constexpr std::wstring_view FIVE_CHAR{ L"5" };
	inline constexpr std::wstring_view SIX_CHAR{ L"6" };
	inline constexpr std::wstring_view SEVEN_CHAR{ L"7" };
	inline constexpr std::wstring_view EIGHT_CHAR{ L"8" };
	inline constexpr std::wstring_view SMILE_CHAR{ L"🙂" };
	inline constexpr std::wstring_view SMILE_OPEN_MOUTH_CHAR{ L"😮" };
	inline constexpr std::wstring_view SMILE_SUNGLASSES_CHAR{ L"😎" };
	inline constexpr std::wstring_view SMILE_DEAD_CHAR{ L"💀" };
}