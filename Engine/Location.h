#pragma once

template<typename T>
class _Location
{
public:
	_Location& operator+=(const _Location& loc)
	{
		this->x += loc.x;
		this->y += loc.y;
		return *this;
	}
	bool operator==(const _Location& loc) const
	{
		return (x == loc.x) && (y == loc.y);
	}
	_Location operator*(const int multiplier) const
	{
		return _Location({ x*multiplier, y*multiplier });
	}
	bool operator!=(const _Location& loc) const
	{
		return (this->x != loc.x) || (this->y != loc.y);
	}
	_Location operator+(const _Location& loc) const
	{
		return { x + loc.x, y + loc.y };
	}
	_Location operator-(const _Location& loc) const
	{
		return {x - loc.x, y - loc.y};
	}
public:
	T x;
	T y;
};

typedef _Location<int> Location;