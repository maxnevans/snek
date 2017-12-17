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
	const Board::Cell::Object& testCollision() const;
	bool onBoard() const;
	void Respawn(const bool rand_pos = false);
	void Grow();
	void EatYourself();
	const Location& GetCurrentLocation() const;
	void Pass() const;
	void SpeedUp(const float spd_ratio);
private:
	void CleanTrack() const;
	void UpdateDelta();
	void InitializeTail();
	void InitializeHead(const Location& loc);
	void InitializeSnake(const Location& loc = start_location);
	bool isOnTheTail() const;
private:
	static constexpr Location start_location = {0,0};
	static constexpr Direction start_direction = dirRight;
	static constexpr int start_size = 3;
	static constexpr float start_speed = 5.0f;;
	static constexpr Color start_head_color = { 181,171,32 };
	static constexpr int tail_draw_size_padding = 2;
	static constexpr int head_draw_size_padding = 1;
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