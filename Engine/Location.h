#pragma once
class Location
{
public:
	Location& operator+=(const Location& loc)
	{
		this->x += loc.x;
		this->y += loc.y;
		return *this;
	}
	bool operator==(const Location& loc) const
	{
		return (x == loc.x) && (y == loc.y);
	}
	Location operator*(const int multiplier) const
	{
		return Location({ x*multiplier, y*multiplier });
	}
	bool operator!=(const Location& loc) const
	{
		return (this->x != loc.x) || (this->y != loc.y);
	}
	Location operator+(const Location& loc) const
	{
		return { x + loc.x, y + loc.y };
	}
	Location operator-(const Location& loc) const
	{
		return {x - loc.x, y - loc.y};
	}
public:
	int x;
	int y;
};