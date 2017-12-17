#include "Apple.h"

Apple::Apple(Board& brd,const Location& loc, std::mt19937& rng)
	:
	loc( loc ),
	brd ( brd ),
	col ( default_color ),
	rng (rng)
{

}

void Apple::Draw() const
{
	brd.Draw(loc, col, 0);
	brd.SetObj(loc, Board::Object::Apple);
}

void Apple::Respawn()
{
	Location spawnLoc;
	do
	{
		std::uniform_int_distribution<int> xDist(0, brd.GetWidth() - 1);
		std::uniform_int_distribution<int> yDist(0, brd.GetHeight() - 1);
		spawnLoc = { xDist(rng), yDist(rng) };
		Relocate(spawnLoc);
	} while (brd.testLocation(spawnLoc) != Board::Object::Empty);
}

void Apple::Relocate(const Location& loc)
{
	brd.SetObj(this->loc, Board::Object::Empty);
	this->loc = loc;
}

const Location& Apple::GetLocation() const
{
	return loc;
}