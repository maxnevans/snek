#pragma once
#include "Colors.h"
#include "Graphics.h"
#include "Location.h"
#include <vector>
#include <chrono>
#include <random>

class Board
{
public:
	struct Cell
	{
		enum Object
		{
			Apple,
			Obstacle,
			Poison,
			Empty
		};
		Object obj;
		Color col;
		int padding;
	};
	struct StartData
	{
		int width;
		int height;
		Color borderColor;
		int borderPadding;
		int borderThickness;
		int dim;
		Color contentsColors[Cell::Object::Empty];
		int contentsPadding[Cell::Object::Empty];
	};
public:
	Board(Graphics& gfx, std::mt19937& rng, const StartData& start_data);
	Board& operator=(const Board& source) = delete;
	Board(const Board& source) = delete;
	~Board();
	void DrawBorder() const;
	void Draw() const;
	void DrawCell(const Location& loc,const Color& col, const int padding) const;
	void DrawAnimatedMove(const Location& oLOC, const Location& nLOC, const float time,const float current_time, const Color& col, const int padding);
	void SpawnObjects(const Cell::Object& what, const int howMany);
	bool SpawnObject(const Cell::Object& what, const Location& loc);
	bool RespawnObject(const Location& loc);
	int GetWidth() const;
	int GetHeight() const;
	const Cell::Object& testLocation(const Location& loc) const;
	void Respawn();
private:
	int leftPadding;
	int topPadding;
	StartData start_data;
	static constexpr Cell empty_cell = { Cell::Empty, NULL, NULL };
	std::vector<int> amount_objects;
	Graphics& gfx;
	std::mt19937& rng;
	Cell* board = nullptr;
	std::chrono::time_point<std::chrono::steady_clock> tMark;
};