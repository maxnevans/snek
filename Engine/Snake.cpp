#include "Snake.h"
#include "Board.h"
#include <random>
#include <chrono>


Snake::Snake(Board& brd, std::mt19937& rng)
	:
	snake(start_size),
	rng(rng),
	update_rate(std::chrono::steady_clock::now()),
	_temp_direction ( start_direction ),
	speed( start_speed ),
	brd ( brd ),
	distance( 0 )
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

void Snake::Control(MainWindow& wnd)
{
	if (wnd.kbd.KeyIsPressed('W') && direction != dirDown)
	{
		_temp_direction = dirUp;
	}
	else if (wnd.kbd.KeyIsPressed('S') && direction != dirUp)
	{
		_temp_direction = dirDown;
	}
	else if (wnd.kbd.KeyIsPressed('A') && direction != dirRight)
	{
		_temp_direction = dirLeft;
	}
	else if (wnd.kbd.KeyIsPressed('D') && direction != dirLeft)
	{
		_temp_direction = dirRight;
	}
}

void Snake::UpdateDelta()
{
	switch (_temp_direction)
	{
	case Snake::dirUp:
		delta.x = 0;
		delta.y = -1;
		break;
	case Snake::dirDown:
		delta.x = 0;
		delta.y = 1;
		break;
	case Snake::dirLeft:
		delta.x = -1;
		delta.y = 0;
		break;
	case Snake::dirRight:
		delta.x = 1;
		delta.y = 0;
		break;
	}
	direction = _temp_direction;
}

void Snake::Draw() const
{
	for (int i = (int)snake.size() - 1; i > 0; i--)
	{
		brd.Draw(snake[i].loc, snake[i].c, 2);
	}
	brd.Draw(snake.front().loc, snake[0].c);
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
		Snake::UpdateDelta();
		brd.SetObj(snake.back().loc, Board::Object::Empty);
		for (int i = (int)snake.size() - 1; i > 0; i--)
		{
			snake[i].loc = snake[i - 1].loc;
			brd.SetObj(snake[i].loc, Board::Object::Snake);
		}
		snake.front().loc += delta;
		brd.SetObj(snake.front().loc, Board::Object::Snake);
		update_rate = std::chrono::steady_clock::now();
		distance++;
	}
}

void Snake::Respawn(const bool rand_pos)
{
	if (rand_pos)
	{
		std::uniform_int_distribution<int> xDist(0, brd.GetWidth() - 1);
		std::uniform_int_distribution<int> yDist(0, brd.GetWidth() - 1);
		Location start_location;
		do
			start_location = { xDist(rng), yDist(rng) };
		while (Board::Object::Empty != brd.testLocation(start_location));
		InitializeSnake(start_location);
	}
	else
	{
		InitializeSnake();
	}
	_temp_direction = start_direction;
	snake.resize(3);
	speed = start_speed;
	distance = 0;
}

bool Snake::testCollisionBoard() const
{
	return  !((snake.front().loc.x >= 0) &&
		(snake.front().loc.x < brd.GetWidth()) &&
		(snake.front().loc.y >= 0) &&
		(snake.front().loc.y < brd.GetHeight()));
}

bool Snake::testCollisionSnake() const
{	
	return (isOnTheTail() && (distance > snake.size() - 1));
}

bool Snake::testCollisionApple() const
{
	return  brd.testLocation(snake.front().loc) == Board::Apple;
}

void Snake::EatYourself()
{
	for (int i = 1; i < snake.size(); i++)
	{
		if (snake.front().loc == snake[i].loc)
		{
			snake.resize(i);
			return;
		}
	}
	
}

bool Snake::isOnTheTail() const
{
	for (int i = 1; i < snake.size(); i++)
	{
		if (snake.front().loc == snake[i].loc)
		{
			return true;
		}
	}
	return false;
}
