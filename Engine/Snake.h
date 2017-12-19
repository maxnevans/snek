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
	struct StartData
	{
		Location location;
		Location direction;
		int size;
		float speed;
		Color head_color;
		int tail_draw_size_padding;
		int head_draw_size_padding;
	};
	struct Direction
	{
		static constexpr Location Up = {0, -1};
		static constexpr Location Down = {0, 1};
		static constexpr Location Left = {-1, 0};
		static constexpr Location Right = {1, 0};
	};
public:
	Snake(Board& brd, std::mt19937& rng,const StartData start_data);
	void Update();
	bool onBoard() const;
	void Respawn(const bool rand_pos = false);
	void Grow();
	int EatYourself();
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
	void InitializeSnake(const Location& loc);
private:
	const StartData start_data;
	bool deltaUpdated = false;
	std::mt19937& rng;
	Board& brd;
	std::chrono::steady_clock::time_point update_rate;
	std::vector<Segment> snake;
	Location _temp_delta;
	Location delta;
	Location next_loc;
	float speed;
};