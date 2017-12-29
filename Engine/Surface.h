#pragma once
#include <string>
#include "Colors.h"
#include "Graphics.h"
#include "Location.h"
#include "Rect.h"

class Surface
{
public:
	Surface(const std::string& filename);
	Surface(const int width, const int heigth, const Color& col = Colors::Magenta);
	~Surface();
	void Draw(const Location& destP, Graphics& gfx, Rect crop,
		const Rect& inside = Rect(0,0, Graphics::ScreenWidth - 1, Graphics::ScreenHeight - 1)) const;
	void SetChroma(const Color& col) const;
	void SetSubstituteColor(const Color& col) const;
private:
	int width;
	int height;
	Color* bitmap;
	mutable Color chroma_color;
	mutable Color substitute_color;
};