#include "Surface.h"
#include <fstream>
#include "ChiliWin.h"
#include <cassert>
Surface::Surface(const std::string& filename)
	:
	chroma_color(NULL),
	substitute_color(NULL)
{
	std::ifstream file(filename);
	if (file)
	{
		BITMAPFILEHEADER _FILEHEADER;
		BITMAPINFOHEADER _INFOHEADER;
		file.read(reinterpret_cast<char*>(&_FILEHEADER), sizeof(_FILEHEADER));
		if (_FILEHEADER.bfType == 0x4d42)
		{
			file.read(reinterpret_cast<char*>(&_INFOHEADER), sizeof(_INFOHEADER));

			file.seekg(_FILEHEADER.bfOffBits, std::ios::beg);

			width = _INFOHEADER.biWidth;
			height = _INFOHEADER.biHeight;

			int startPos = height - 1;
			int endPos = 0;
			int delta = -1;

			if (height < 0)
			{
				height *= -1;
				startPos = 0;
				endPos = height - 1;
				delta = 1;
			}

			bitmap = new Color[width*height];


			if (_INFOHEADER.biCompression == BI_RGB)
			{
				if (_INFOHEADER.biBitCount == 24)
				{
					const int padding = (4 - (width * 3 % 4)) % 4;
					for (int y = startPos; y != endPos; y += delta)
					{
						for (int x = 0; x < width; x++)
						{
							bitmap[y*width + x] = Color({ (unsigned char)file.get(),(unsigned char)file.get(),(unsigned char)file.get() });
						}
						file.seekg(padding, std::ios::cur);
					}
				}
				else if (_INFOHEADER.biBitCount == 32)
				{
					for (int y = startPos; y != endPos; y += delta)
					{
						for (int x = 0; x < width; x++)
						{
							bitmap[y*width + x] = Color({(unsigned char) file.get(),(unsigned char)file.get(),(unsigned char)file.get()});
							file.seekg(1, std::ios::cur);
						}
					}
				}
				else
				{
					throw std::runtime_error("Unsupported bit capacity: " + _INFOHEADER.biBitCount);
				}
			}
			else
			{
				throw std::runtime_error("Unsupported compression type! " + _INFOHEADER.biCompression);
			}
		}
		else
		{
			throw std::runtime_error("Wrong filetype: " + _FILEHEADER.bfType);
		}
	}
	else
	{
		throw std::runtime_error("Failed to open " + filename);
	}
}

Surface::Surface(const int width, const int heigth, const Color& col)
	:
	width(width),
	height(height),
	chroma_color(NULL),
	substitute_color(col),
	bitmap(nullptr)
{
}

Surface::~Surface()
{
	delete [] bitmap;
}

void Surface::Draw(const Location& destP, Graphics& gfx, Rect crop, const Rect& inside) const
{
	assert(crop.gtop() >= 0);
	assert(crop.gbottom() <= height);
	assert(crop.gleft() >= 0);
	assert(crop.gright() <= width);

	for (int y = crop.gtop(); y < crop.gbottom(); ++y)
	{
		for (int x = crop.gleft(); x < crop.gright(); ++x)
		{
			if (bitmap[y*width + x] != chroma_color)
			{
				if (substitute_color != NULL)
				{
					gfx.PutPixel(destP.x + x - crop.gleft(), destP.y + y - crop.gtop(), substitute_color);
				} 
				else
				{ 
					gfx.PutPixel(destP.x + x - crop.gleft(), destP.y + y - crop.gtop(), bitmap[y*width + x]);
				}
			}
		}
	}
}

void Surface::SetChroma(const Color & col) const
{
	chroma_color = col;
}

void Surface::SetSubstituteColor(const Color & col) const
{
	substitute_color = col;
}