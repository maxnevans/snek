#include "Board.h"
#include <assert.h>

Board::Board(Graphics& gfx, std::mt19937& rng)
	:
	gfx( gfx),
	rng(rng),
	board(width*height, empty_cell),
	amount_objects(Board::Cell::Object::size)
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

const Board::Cell::Object& Board::testLocation(const Location & loc) const
{
	return board[loc.y*width + loc.x].obj;
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

void Board::Draw() const
{
	for (int c = 0; c < board.size(); c++)
	{ 
		if (board[c].obj != Board::Cell::Object::Empty)
		{
			int x = c % width;
			int y = c / width;
			gfx.DrawRect(x*dim + leftPadding + borderThickness + borderPadding + board[c].padding,
				y*dim + topPadding + borderThickness + borderPadding + board[c].padding, dim - board[c].padding * 2,
				dim - board[c].padding * 2, board[c].col);
		}
	}
	
}

void Board::DrawCell(const Location& loc, const Color& col, const int padding)
{
	gfx.DrawRect(loc.x*dim + leftPadding + borderThickness + borderPadding + padding,
		loc.y*dim + topPadding + borderThickness + borderPadding + padding, dim - padding * 2,
		dim - padding * 2, col);
}

void Board::SpawnObjects(const Board::Cell::Object& what,const int howMany)
{
	amount_objects[what] = howMany;
	for (int i = 0; i < howMany; i++)
	{
		Location _temp_loc;
		do
		{
			std::uniform_int_distribution<int> xDist(0, width - 1);
			std::uniform_int_distribution<int> yDist(0, height - 1);

			_temp_loc = { xDist(rng), yDist(rng) };
			
		} while (board[_temp_loc.y*width + _temp_loc.x].obj != Cell::Empty );
		board[_temp_loc.y*width + _temp_loc.x].col = contentsColors[what];
		board[_temp_loc.y*width + _temp_loc.x].obj = what;
		board[_temp_loc.y*width + _temp_loc.x].padding = 0;
	}
}

bool Board::SpawnObject(const Board::Cell::Object& what, const Location& loc)
{
	assert(loc.x >= 0);
	assert(loc.x < width);
	assert(loc.y >= 0);
	assert(loc.y < height);
	if (board[loc.y* width + loc.x].obj != Board::Cell::Empty)
	{
		return false;
	}
	else
	{
		amount_objects[what]++;
		board[loc.y*width + loc.x].col = contentsColors[what];
		board[loc.y*width + loc.x].obj = what;
		board[loc.y*width + loc.x].padding = contentsPadding[what];
		return true;
	}
}

bool Board::RespawnObject(const Location& loc)
{
	assert(loc.x >= 0);
	assert(loc.x < width);
	assert(loc.y >= 0);
	assert(loc.y < height);
	if (board[loc.y*width + loc.x].obj == Board::Cell::Object::Empty)
	{
		return false;
	}
	else
	{
		Location _temp_loc;
		do
		{
			std::uniform_int_distribution<int> xDist(0, width - 1);
			std::uniform_int_distribution<int> yDist(0, height - 1);

			_temp_loc = { xDist(rng), yDist(rng) };

		} while (board[_temp_loc.y*width + _temp_loc.x].obj != Cell::Empty);

		board[_temp_loc.y*width + _temp_loc.x].col = board[loc.y*width + loc.x].col;
		board[_temp_loc.y*width + _temp_loc.x].obj = board[loc.y*width + loc.x].obj;
		board[_temp_loc.y*width + _temp_loc.x].padding = board[loc.y*width + loc.x].padding;
		board[loc.y*width + loc.x].col = NULL;
		board[loc.y*width + loc.x].obj = Board::Cell::Object::Empty;
		board[loc.y*width + loc.x].padding = NULL;
		return true;
	}
}

void Board::Respawn()
{
	for (Cell& c : board)
	{
		c = empty_cell;
	}
	for (int obj = Cell::Object::Apple; obj < Cell::Object::size; obj++ )
	{
		SpawnObjects(static_cast<Cell::Object> (obj), amount_objects[obj]);
	}
}