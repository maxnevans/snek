#include "Board.h"
#include <assert.h>

Board::Board(Graphics& gfx, std::mt19937& rng, const StartData& start_data)
	:
	gfx( gfx),
	rng(rng),
	start_data(start_data),
	board(new Cell[start_data.width*start_data.height]),
	amount_objects(Board::Cell::Object::Empty),
	leftPadding( (gfx.ScreenWidth - start_data.borderPadding*2 - start_data.borderThickness*2 - start_data.width*start_data.dim)/2 ),
	topPadding((gfx.ScreenHeight - start_data.borderPadding * 2 - start_data.borderThickness * 2 - start_data.height*start_data.dim) / 2)
{
	for (int s = 0; s < start_data.width*start_data.height; s++)
	{
		board[s] = empty_cell;
	}
}

Board::~Board()
{
	delete [] board;
}

int Board::GetWidth() const
{
	return start_data.width;
}

int Board::GetHeight() const
{
	return start_data.height;
}

const Board::Cell::Object& Board::testLocation(const Location & loc) const
{
	return board[loc.y*start_data.width + loc.x].obj;
}

void Board::DrawBorder() const
{
	for (int x = 0; x <= start_data.width*start_data.dim + start_data.borderThickness + start_data.borderPadding*2 ; x++)
	{
		for (int t = 0; t < start_data.borderThickness; t++)
		{
			gfx.PutPixel(x+ leftPadding , topPadding + t, start_data.borderColor);
			gfx.PutPixel(x + leftPadding, topPadding + start_data.borderThickness + start_data.borderPadding*2 + start_data.height*start_data.dim + t, start_data.borderColor);
		}
	}
	for (int y = 0; y <= start_data.height*start_data.dim + start_data.borderThickness + start_data.borderPadding*2; y++)
	{
		for (int t = 0 ; t < start_data.borderThickness; t++)
		{
			gfx.PutPixel(leftPadding + t, y + topPadding, start_data.borderColor);
			gfx.PutPixel(leftPadding + start_data.borderThickness + start_data.borderPadding*2 + start_data.width*start_data.dim + t, y+ topPadding, start_data.borderColor);
		}
	}
}

void Board::Draw() const
{
	for (int c = 0; c < start_data.width*start_data.height; c++)
	{ 
		if (board[c].obj != Board::Cell::Object::Empty)
		{
			int x = c % start_data.width;
			int y = c / start_data.width;
			gfx.DrawRect(x*start_data.dim + leftPadding + start_data.borderThickness + start_data.borderPadding + board[c].padding,
				y*start_data.dim + topPadding + start_data.borderThickness + start_data.borderPadding + board[c].padding, start_data.dim - board[c].padding * 2,
				start_data.dim - board[c].padding * 2, board[c].col);
		}
	}
	
}

void Board::DrawCell(const Location& loc, const Color& col, const int padding)
{
	gfx.DrawRect(loc.x*start_data.dim + leftPadding + start_data.borderThickness + start_data.borderPadding + padding,
		loc.y*start_data.dim + topPadding + start_data.borderThickness + start_data.borderPadding + padding, start_data.dim - padding * 2,
		start_data.dim - padding * 2, col);
}

void Board::SpawnObjects(const Board::Cell::Object& what,const int howMany)
{
	amount_objects[what] = howMany;
	for (int i = 0; i < howMany; i++)
	{
		Location _temp_loc;
		do
		{
			std::uniform_int_distribution<int> xDist(0, start_data.width - 1);
			std::uniform_int_distribution<int> yDist(0, start_data.height - 1);

			_temp_loc = { xDist(rng), yDist(rng) };
			
		} while (board[_temp_loc.y*start_data.width + _temp_loc.x].obj != Cell::Empty );
		board[_temp_loc.y*start_data.width + _temp_loc.x].col = start_data.contentsColors[what];
		board[_temp_loc.y*start_data.width + _temp_loc.x].obj = what;
		board[_temp_loc.y*start_data.width + _temp_loc.x].padding = start_data.contentsPadding[what];
	}
}

bool Board::SpawnObject(const Board::Cell::Object& what, const Location& loc)
{
	assert(loc.x >= 0);
	assert(loc.x < start_data.width);
	assert(loc.y >= 0);
	assert(loc.y < start_data.height);
	if (board[loc.y* start_data.width + loc.x].obj != Board::Cell::Empty)
	{
		return false;
	}
	else
	{
		amount_objects[what]++;
		board[loc.y*start_data.width + loc.x].col = start_data.contentsColors[what];
		board[loc.y*start_data.width + loc.x].obj = what;
		board[loc.y*start_data.width + loc.x].padding = start_data.contentsPadding[what];
		return true;
	}
}

bool Board::RespawnObject(const Location& loc)
{
	assert(loc.x >= 0);
	assert(loc.x < start_data.width);
	assert(loc.y >= 0);
	assert(loc.y < start_data.height);
	if (board[loc.y*start_data.width + loc.x].obj == Board::Cell::Object::Empty)
	{
		return false;
	}
	else
	{
		Location _temp_loc;
		do
		{
			std::uniform_int_distribution<int> xDist(0, start_data.width - 1);
			std::uniform_int_distribution<int> yDist(0, start_data.height - 1);

			_temp_loc = { xDist(rng), yDist(rng) };

		} while (board[_temp_loc.y*start_data.width + _temp_loc.x].obj != Cell::Empty);

		board[_temp_loc.y*start_data.width + _temp_loc.x].col = board[loc.y*start_data.width + loc.x].col;
		board[_temp_loc.y*start_data.width + _temp_loc.x].obj = board[loc.y*start_data.width + loc.x].obj;
		board[_temp_loc.y*start_data.width + _temp_loc.x].padding = board[loc.y*start_data.width + loc.x].padding;
		board[loc.y*start_data.width + loc.x].col = NULL;
		board[loc.y*start_data.width + loc.x].obj = Board::Cell::Object::Empty;
		board[loc.y*start_data.width + loc.x].padding = NULL;
		return true;
	}
}

void Board::Respawn()
{
	for (int s = 0; s <start_data.width*start_data.height; s++)
	{
		board[s] = empty_cell;
	}
	for (int obj = Cell::Object::Apple; obj < Cell::Object::Empty; obj++ )
	{
		SpawnObjects(static_cast<Cell::Object> (obj), amount_objects[obj]);
	}
}