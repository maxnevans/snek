#pragma once
#include "Location.h"

template<typename T>
class _Rect
{
public:
	_Rect(const T x, const T y, const T width, const T height)
		:
		x ( x ),
		y ( y ),
		width ( width ),
		height ( height ),
		x2( x + width),
		y2( y + height)
	{
	}
	_Rect(const _Location<T>& destP, const T width, const T height)
		:
		x ( (T) destP.x ),
		y ( (T) destP.y ),
		width ( width ),
		height ( height ),
		x2 ( (T) destP.x + width ),
		y2 ( (T) destP.y + height )
	{
	}
	T gwidth() const
	{
		return width;
	}
	T gheight() const
	{
		return height;
	}
	T gtop() const
	{
		return y;
	}
	T gright() const
	{
		return x + width;
	}
	T gleft() const
	{
		return x;
	}
	T gbottom() const
	{
		return y + height;
	}
	_Location<T> gtopLeft() const
	{
		return {x,y};
	}
	_Location<T> grightBottom() const
	{
		return {x2, y2};
	}
	_Location<T> gcenter() const
	{
		return { x2 / 2, y2 / 2 };
	}
	void swidth(const T width)
	{
		this->width = width;
		this->x2 = this->x + width;
	}
	void sheight(const T height)
	{
		this->height = height;
		this->y2 = this->y + height;
	}
	void srightBottom(const _Location<T>& loc)
	{
		this->x2 = loc.x;
		this->y2 = loc.y;
		reculcWH();
	}
	void sleftTop(const _Location<T>& loc)
	{
		this->x = loc.x;
		this->y = loc.y;
		reculcWH();
	}
	void mleftTop(const _Location<T>& newleftTop)
	{
		sleftTop(newleftTop);
		this->x2 = this->x + this->width;
		this->y2 = this->y + this->height;
	}
	void mrightBottom(const _Location<T>& newrightBottom)
	{
		srightBottom(newrightBottom);
		this->x = this->x2 - this->width;
		this->y = this->y2 - this->height;
	}
	void stop(const T top)
	{
		this->y = top;
		this->height = this->y2 - top;
	}
	void sbottom(const T bottom)
	{
		this->y2 = bottom;
		this->height = bottom - this->y;
	}
	void sleft(const T left)
	{
		this->x = left;
		this->width = this->x2 - left;
	}
	void sright(const T right)
	{
		this->x2 = right;
		this->width = right - this->x;
	}
private:
	void reculcWH()
	{
		this->width = this->x2 - this->x;
		this->height = this->y2 - this->y;
	}
private:
	T x;
	T y;
	T x2;
	T y2;
	T width;
	T height;
};

typedef _Rect<int> Rect;