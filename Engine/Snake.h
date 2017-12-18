#pragma once
#include "Location.h"
#include "Colors.h"
#include <vector>
#include "MainWindow.h"
#include <random>
#include <chrono>
#include "Board.h"

class Snake
{
private:
	struct Segment
	{
		Location loc;
		Color c;
	};
public:
	struct Direction
	{
		static constexpr Location Up = {0, -1};
		static constexpr Location Down = {0, 1};
		static constexpr Location Left = {-1, 0};
		static constexpr Location Right = {1, 0};
	};
public:
	Snake(Board& brd, std::mt19937& rngs);
	void Update();
	bool onBoard() const;
	void Respawn(const bool rand_pos = false);
	void Grow();
	void EatYourself();
	const Location& GetNextLocation() const;
	const Location& Snake::GetCurrentLocation() const;
	void SpeedUp(const float spd_ratio);
	void MoveByDelta(const Location& delta_loc);
	bool isOnTheTail() const;
	void Draw() const;
	void UpdateDelta();
private:
	void InitializeTail();
	void InitializeHead(const Location& loc);
	void InitializeSnake(const Location& loc = start_location);
private:
	static constexpr Location start_location = {0,0};
	static constexpr Location start_direction = Direction::Right;
	static constexpr int start_size = 3;
	static constexpr float start_speed = 5.0f;;
	static constexpr Color start_head_color = { 181,171,32 };
	static constexpr int tail_draw_size_padding = 2;
	static constexpr int head_draw_size_padding = 1;
	std::mt19937& rng;
	Board& brd;
	std::chrono::steady_clock::time_point update_rate;
	std::vector<Segment> snake;
	Location _temp_delta;
	Location delta;
	Location next_loc;
	float speed;
};