#pragma once
#include "Surface.h"
#include <vector>
#include "Location.h"
#include "Colors.h"
#include <string>
#include "Rect.h"
#include "Graphics.h"

class Font
{
private:
	struct Text
	{
		Location loc;
		Color col;
		int size;
	};
public:
	Font(const Surface* font, Graphics& gfx)
		:
		font(font),
		gfx( gfx )
	{
	}
	void print(const std::string& text, const Location& loc, const Color& col, const int size)
	{
		int line = 0;
		int carriage = 0;
		font->SetSubstituteColor(col);
		for (int i = 0; i < text.size(); i++)
		{
			if (text[i] == '\n')
			{
				line++;
				carriage = 0;
				continue;
			}
			int serial = text[i] - ' ';
			int col = serial % row_size;
			int row = serial / row_size;
			Location delta = {font_size_width*carriage, line*font_size_height};
			Rect glyph = { col*font_size_width, row* font_size_height, font_size_width, font_size_height };
			font->Draw(loc + delta, gfx, glyph);
			carriage++;
		}
	}
private:
	const Surface* const font;
	Graphics& gfx;
	static constexpr int row_size = 32;
	static constexpr int col_size = 3;
	static constexpr int font_size_height = 72;
	static constexpr int font_size_width = 36;
};