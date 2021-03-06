#include "Snake.h"
#include "Board.h"
#include <random>
#include <chrono>
#include <cassert>
#include <fstream>


Snake::Snake(Board& brd, std::mt19937& rng,const Snake::StartData start_data)
	:
	start_data( start_data ),
	snake(start_data.size),
	rng(rng),
	update_rate(std::chrono::steady_clock::now()),
	_temp_delta ( start_data.direction ),
	next_loc(start_data.location + _temp_delta),
	past_loc(start_data.location),
	speed( start_data.speed ),
	brd ( brd )
{
	InitializeSnake(start_data.location);
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
	snake.front().c = start_data.head_color;
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
	std::chrono::duration<float> _update_rate = std::chrono::steady_clock::now() - update_rate;
	current_rate_time = _update_rate.count();
	if (speed*current_rate_time > 1)
	{
		delta = _temp_delta;
		next_loc = snake.front().loc + delta;
		past_loc = snake.back().loc;
		update_rate = std::chrono::steady_clock::now();
		deltaUpdated = true;
	}
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
	if (deltaUpdated)
	{
		for (int i = (int)snake.size() - 1; i > 0; i--)
		{
			snake[i].loc = snake[i - 1].loc;
		}
		snake.front().loc = next_loc;
		deltaUpdated = false;
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
		InitializeSnake(start_data.location);
	}
	_temp_delta = start_data.direction;
	snake.resize(3);
	speed = start_data.speed;
}

bool Snake::onBoard() const
{
	return  (next_loc.x >= 0) &&
		(next_loc.x < brd.GetWidth()) &&
		(next_loc.y >= 0) &&
		(next_loc.y < brd.GetHeight());
}

int Snake::EatYourself()
{
	for (int i = 1; i < snake.size() - 1; i++)
	{
		if (next_loc == snake[i].loc)
		{
			snake.resize(i + 1);
			return i - start_data.size;
		}
	}
	return -1;
}

bool Snake::isOnTheTail() const
{
	for (int i = 1; i < snake.size() - 1; i++)
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
	brd.DrawAnimatedMove(snake[1].loc, snake[0].loc, 1 / speed, current_rate_time, snake[0].c, start_data.head_draw_size_padding);
	for (int i = 1; i < snake.size() - 1; i++)
	{
		brd.DrawAnimatedMove(snake[i + 1].loc,snake[i].loc, 1/speed , current_rate_time, snake[i].c, start_data.tail_draw_size_padding);
	}
	brd.DrawAnimatedMove(past_loc, snake.back().loc, 1 / speed, current_rate_time, snake.back().c, start_data.tail_draw_size_padding);
}
