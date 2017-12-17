#pragma once
#include "Location.h"
#include "Colors.h"
#include "Board.h"
#include <random>

class Apple
{
public:
	Apple(Board& brd,const Location& loc, std::mt19937& rng);
	void Respawn();
	void Draw() const;
	const Location& GetLocation() const;
private:
	void Relocate(const Location& loc);
private:
	static constexpr Color default_color = { 171,49,40 };
	std::mt19937& rng;
	Color col;
	Location loc;
	Board& brd;
};