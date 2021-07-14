#pragma once

// Possible states a tile can have
enum class TileState
{
	HIDDEN,
	CLICKED,
	REVEALED,
};

// Possible contents a tile can have
enum class TileContent
{
	EMPTY,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	MINE,
};

// Possible marks a tile can have
enum class TileMark
{
	NONE,
	FLAG,
	QUESTION_MARK,
};

// Possible states the smile can have
enum class SmileState
{
	SMILE,
	SMILE_OPEN_MOUTH,
	SMILE_SUNGLASSES,
	SMILE_DEAD
};