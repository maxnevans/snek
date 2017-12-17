#pragma once
#include "Location.h"
#include "Colors.h"
#include <vector>
#include "MainWindow.h"
#include <random>
#include <chrono>
#include "Board.h"
#include "Apple.h"

class Snake
{
private:
	struct Segment
	{
		Location loc;
		Color c;
	};
	enum Direction
	{
		dirUp,
		dirDown,
		dirLeft,
		dirRight
	};
public:
	Snake(Board& brd, std::mt19937& rngs);
	void Control(MainWindow& wnd);
	void Update();
	void Draw() const;
	void testCollision(const bool Eat = false);
	bool testCollision(const Apple& apple) const;
	bool testCollision(const Board& brd) const;
	void Respawn(const bool rand_pos);
	void Grow();
private:
	void UpdateDelta();
	void InitializeTail();
	void InitializeHead(const Location& loc);
	void InitializeSnake(const Location& loc = start_location);
	int isOnTheTail() const;
	void EatYourself(const int wasEaten);
private:
	static constexpr Location start_location = {0,0};
	static constexpr Direction start_direction = dirRight;
	static constexpr int start_size = 3;
	static constexpr float start_speed = 5.0f;;
	static constexpr Color start_head_color = { 181,171,32 };
	static constexpr int tail_draw_size = 2;
	static constexpr int head_draw_size = 1;
	std::mt19937& rng;
	Board& brd;
	std::chrono::steady_clock::time_point update_rate;
	std::vector<Segment> snake;
	Direction direction;
	Direction _temp_direction;
	Location delta;
	int distance;
	float speed;
};