#pragma once
#include "Colors.h"
#include "Graphics.h"
#include "Location.h"
#include <vector>

class Board
{
public:
	enum Object
	{
		Snake,
		Apple,
		Empty
	};
public:
	Board(Graphics& gfx);
	void DrawBorder() const;
	void Draw(const Location& loc, const Color& col, const int padding = 0) const;
	void SetObj(const Location& loc,const Object& obj);
	int GetWidth() const;
	int GetHeight() const;
	const Object& testLocation(const Location& loc) const;
private:
	void InitBoard();
private:
	static constexpr int width = 39;
	static constexpr int height = 29;
	static constexpr Color borderColor = { 80,156,171 };
	static constexpr int borderPadding = 2;
	static constexpr int borderThickness = 5;
	static constexpr int leftPadding = 3;
	static constexpr int topPadding = 3;
	static constexpr int dim = 20;
	Graphics& gfx;
	std::vector<Object> board;
};