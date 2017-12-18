#include "Snake.h"
#include "Board.h"
#include <random>
#include <chrono>
#include <cassert>


Snake::Snake(Board& brd, std::mt19937& rng)
	:
	snake(start_size),
	rng(rng),
	update_rate(std::chrono::steady_clock::now()),
	_temp_delta ( start_direction ),
	speed( start_speed ),
	brd ( brd )
{
	InitializeSnake();
}

void Snake::InitializeTail()
{
	std::uniform_int_distribution<int> colorDistG(100, 175);
	for (int i = 1; i < snake.size(); i++)
	{
		snake[i].loc = snake.front().loc;
		Color _temp(0, colorDistG(rng), 0);
		snake[i].c = _temp;
	}
}

void Snake::InitializeHead(const Location& loc)
{
	snake.front().c = start_head_color;
	snake.front().loc = loc;
}

void Snake::InitializeSnake(const Location& loc)
{
	InitializeHead(loc);
	InitializeTail();
}

void Snake::MoveByDelta(const Location& delta_loc)
{
	assert(abs(delta_loc.y) + abs(delta_loc.x) == 1 );
	if (delta_loc == Direction::Up && delta != Direction::Down
		|| delta_loc == Direction::Down && delta != Direction::Up
		|| delta_loc == Direction::Left && delta != Direction::Right
		|| delta_loc == Direction::Right && delta != Direction::Left)
	{
		_temp_delta = delta_loc;
	}
	
}

void Snake::UpdateDelta()
{
	delta = _temp_delta;
	next_loc = snake.front().loc + delta;
}

void Snake::Grow()
{
	Segment extra_block;
	extra_block.loc = snake[snake.size() - 1].loc;
	std::uniform_int_distribution<int> colorDistG(75, 125);
	Color _temp(0, colorDistG(rng), 0);
	extra_block.c = _temp;
	snake.push_back(extra_block);
}

void Snake::Update()
{
	std::chrono::duration<float> _update_rate = std::chrono::steady_clock::now() - update_rate;
	float rate = _update_rate.count();
	if (speed*rate > 1)
	{
		for (int i = (int)snake.size() - 1; i > 0; i--)
		{
			snake[i].loc = snake[i - 1].loc;
		}
		snake.front().loc = next_loc;
		update_rate = std::chrono::steady_clock::now();
	}
}

void Snake::Respawn(const bool rand_pos)
{
	
	if (rand_pos)
	{
		std::uniform_int_distribution<int> xDist(0, brd.GetWidth() - 1);
		std::uniform_int_distribution<int> yDist(0, brd.GetHeight() - 1);
		Location start_location;
		do
			start_location = { xDist(rng), yDist(rng) };
		while (Board::Cell::Object::Empty != brd.testLocation(start_location));
		InitializeSnake(start_location);
	}
	else
	{
		InitializeSnake();
	}
	_temp_delta = start_direction;
	snake.resize(3);
	speed = start_speed;
}

bool Snake::onBoard() const
{
	return  (next_loc.x >= 0) &&
		(next_loc.x < brd.GetWidth()) &&
		(next_loc.y >= 0) &&
		(next_loc.y < brd.GetHeight());
}

void Snake::EatYourself()
{
	for (int i = 0; i < snake.size() - 1; i++)
	{
		if (next_loc == snake[i].loc)
		{
			snake.resize(i + 1);
			return;
		}
	}
	
}

bool Snake::isOnTheTail() const
{
	for (int i = 0; i < snake.size() - 1; i++)
	{
		if (next_loc == snake[i].loc)
		{
			return true;
		}
	}
	return false;
}

const Location& Snake::GetNextLocation() const
{
	return next_loc;
}

const Location& Snake::GetCurrentLocation() const
{
	return snake.front().loc;
}
void Snake::SpeedUp(const float spd_ratio)
{
	speed += spd_ratio;
}

void Snake::Draw() const
{
	for (const Segment& s : snake)
	{
		brd.DrawCell(s.loc, s.c, tail_draw_size_padding);
	}
	brd.DrawCell(snake.front().loc, snake.front().c, head_draw_size_padding);
}
