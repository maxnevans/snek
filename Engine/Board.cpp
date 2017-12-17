#include "Board.h"
#include <assert.h>

Board::Board(Graphics& gfx)
	:
	gfx( gfx),
	board(width*height, Empty)
{
}

int Board::GetWidth() const
{
	return width;
}

int Board::GetHeight() const
{
	return height;
}

const Board::Object& Board::testLocation(const Location & loc) const
{
	return board[loc.y*width + loc.x];
}

void Board::DrawBorder() const
{
	for (int x = 0; x <= width*dim + borderThickness + borderPadding*2 ; x++)
	{
		for (int t = 0; t < borderThickness; t++)
		{
			gfx.PutPixel(x+leftPadding ,topPadding + t,borderColor);
			gfx.PutPixel(x + leftPadding, topPadding + borderThickness + borderPadding*2 + height*dim + t, borderColor);
		}
	}
	for (int y = 0; y <= height*dim +  borderThickness + borderPadding*2; y++)
	{
		for (int t = 0 ; t < borderThickness; t++)
		{
			gfx.PutPixel(leftPadding + t, y + topPadding, borderColor);
			gfx.PutPixel(leftPadding + borderThickness + borderPadding*2 + width*dim + t, y+ topPadding, borderColor);
		}
	}
}

void Board::Draw( const Location& loc, const Color& col, const int padding) const
{
	assert(loc.x >= 0);
	assert(loc.x < width);
	assert(loc.y >= 0);
	assert(loc.y < height);

	
	gfx.DrawRect(loc.x*dim + leftPadding + borderThickness+ borderPadding + padding, loc.y*dim + topPadding + borderThickness + borderPadding + padding, dim - padding * 2, dim - padding * 2, col);
}

void Board::SetObj(const Location& loc, const Board::Object& obj)
{
	assert(loc.x >= 0);
	assert(loc.x < width);
	assert(loc.y >= 0);
	assert(loc.y < height);
	board[loc.y*width + loc.x] = obj;
}

