#pragma once
#include "Colors.h"
#include "Graphics.h"
#include "Location.h"
#include <vector>
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
			size,
			Snake,
			Empty
		};
		Object obj;
		Color col;
		int padding;
	};
public:
	Board(Graphics& gfx, std::mt19937& rng);
	void DrawBorder() const;
	void Draw() const;
	void SetObj(const Location& loc,const Cell& obj);
	void SpawnObjects(const Cell::Object& what, const int howMany);
	bool SpawnObject(const Cell::Object& what, const Location& loc);
	bool RespawnObject(const Location& loc);
	int GetWidth() const;
	int GetHeight() const;
	const Cell::Object& testLocation(const Location& loc) const;
	void Respawn();
	const Cell& EmptyCell() const;
private:
	static constexpr int width = 39;
	static constexpr int height = 27;
	static constexpr Color borderColor = { 80,156,171 };
	static constexpr int borderPadding = 2;
	static constexpr int borderThickness = 5;
	static constexpr int leftPadding = 3;
	static constexpr int topPadding = 43;
	static constexpr int dim = 20;
	static constexpr Color contentsColors[] = {{ 214,17,73 },
											   { 166,177,184 },
											   { 78,0,104 }};
	static constexpr int contentsPadding[] = {0 , 0 , 0};
	static constexpr Cell empty_cell = { Cell::Empty, NULL, NULL };
	std::vector<int> amount_objects;
	Graphics& gfx;
	std::mt19937& rng;
	std::vector<Cell> board;
};