#pragma once

#include <string>
#include <vector>

class FTPoint
{
public:
	FTPoint(int x, int y, int on) {
		this->x = x;
		this->y = y;
		this->on = on;
	}
	FTPoint() {
		x = 0;
		y = 0;
		on = 0;
	}
	int x;
	int y;
	int on;

	Gdiplus::PointF GetPointF(float unitPerEm, float gridSize) const {
		return Gdiplus::PointF(this->x / unitPerEm * gridSize, (unitPerEm - this->y) / unitPerEm * gridSize);
	}
};


class FTGlyph
{
public:
	std::string name;
	int xMin;
	int yMin;
	int xMax;
	int yMax;
	std::vector<std::vector<FTPoint>> contours;
	int lineDraw;
	int unitsPerEm;

	FTGlyph();
};
