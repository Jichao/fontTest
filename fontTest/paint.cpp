#include "stdafx.h"
#include "trace.h"
#include "types.h"
#include "parse.h"
#include "config.h"
#include <math.h>
#include "StringUtils.h"

static const int kGridSize = 600;
static const int kGridMargin = 50;
static const Gdiplus::Color kLineColor(0xff, 0x00, 0x00, 0x00);
static const Gdiplus::Color kPointColor(0xff, 0xff, 0x00, 0x00);
static const Gdiplus::Color kControlPointColor(0xff, 0x00, 0xff, 0x00);

bool GetNextOffPointCount(const std::vector<FTPoint>& points, size_t index, int* count)
{
	*count = 0;
	index += 1;
	while (index <= points.size() - 1) {
		if (points[index].on)
			break;
		(*count)++;
		index++;
	}
	if ((int)(index + count) == (int)(points.size() - 1))
		return false;
	return true;
}

bool GetPointType(const std::vector<FTPoint>& points, size_t index, int* power)
{
	assert(points[index].on && index < points.size() - 1);
	int count = 0;
	if (!GetNextOffPointCount(points, index, &count)) {
		return false;
	}
	*power = count + 1;
	return true;
}

float GetBinomialCo(int n, int k)
{
	int i;
	float b;
	if (0 == k || n == k) {
		return 1;
	}
	if (k > n) {
		return 0;
	}
	if (k > (n - k)) {
		k = n - k;
	}
	if (1 == k) {
		return n;
	}
	b = 1;
	for (i = 1; i <= k; ++i) {
		b *= (n - (k - i));
		if (b < 0)
			return -1;
		b /= i;
	}
	return b;
}

void DrawPoints(Gdiplus::Graphics& g, const std::vector<FTPoint>& points, int unitsPerEm)
{

	Gdiplus::SolidBrush sBrush(kPointColor);
	Gdiplus::Pen endPen(&sBrush, 1.0f);

	Gdiplus::SolidBrush cBrush(kControlPointColor);
	Gdiplus::Pen controlPen(&cBrush, 1.0f);

	size_t index = 0;
	while (index < points.size() - 1) {
		auto pt = points[index].GetPointF(unitsPerEm, kGridSize);
		Gdiplus::RectF rc(pt.X - 1, pt.Y - 1, 2, 2);
		if (points[index].on) {
			g.DrawArc(&endPen, rc, 0., 360.);
		} else {
			g.DrawArc(&controlPen, rc, 0., 360.);
		}

		std::wstring keke = StringUtils::format(L"%d", index);
		Gdiplus::Font myFont(L"Arial", 6);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);

		Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0xff));
		Gdiplus::RectF textRc(pt.X + 5, pt.Y + 5, 40, 40);
		g.DrawString(keke.c_str(), keke.size(), &myFont, textRc, &format, &blackBrush);
		index++;
	}
}

void DrawGenericBezier(Gdiplus::Graphics& g, const FTPoint* points, int size, int unitsPerEm) 
{
	Gdiplus::SolidBrush sBrush(kLineColor);
	Gdiplus::Pen pen(&sBrush, 1.0f);
	float t = 0;
	float step = 0.01f / size;
	for (; t < 1; t += step) {
		float sumx = 0;
		float sumy = 0;
		for (int i = 0; i <= size; ++i) {
			sumx += GetBinomialCo(size, i) * pow(1 - t, size - i) * pow(t, i) * (((float)points[i].x) / unitsPerEm * kGridSize);
			sumy += GetBinomialCo(size, i) * pow(1 - t, size - i) * pow(t, i) * ((unitsPerEm - (float)points[i].y) / unitsPerEm * kGridSize);
		}
		g.DrawRectangle(&pen, sumx, sumy, 1.f, 1.f);
	}
}

void RenderContourByLine(Gdiplus::Graphics& g, const std::vector<FTPoint>& points, int unitsPerEm)
{
	Gdiplus::SolidBrush sBrush(kLineColor);
	Gdiplus::Pen pen(&sBrush, 1.0f);

	std::vector<Gdiplus::PointF> resPoints;
	for (size_t i = 0; i < points.size(); ++i) {
		if (points[i].on) {
			resPoints.push_back(points[i].GetPointF(unitsPerEm, kGridSize));
		}
	}
	g.DrawLines(&pen, resPoints.data(), resPoints.size());
	DrawPoints(g, points, unitsPerEm);
}

FTPoint ConvertToRealPt(const FTPoint& pt, int unitsPerEm) {
	FTPoint realPt;
	realPt.x = (((float)pt.x) / unitsPerEm * kGridSize);
	realPt.y = (unitsPerEm - (float)pt.y) / unitsPerEm * kGridSize;
	return realPt;
}

void DrawQudarticBezier(Gdiplus::Graphics& g, const FTPoint* points, int unitsPerEm) 
{
	Gdiplus::SolidBrush sBrush(kLineColor);
	Gdiplus::Pen pen(&sBrush, 1.0f);
	FTPoint q0 = ConvertToRealPt(points[0], unitsPerEm);
	FTPoint q1 = ConvertToRealPt(points[1], unitsPerEm);
	FTPoint q2 = ConvertToRealPt(points[2], unitsPerEm);
	Gdiplus::PointF p0(q0.x, q0.y);
	Gdiplus::PointF p3(q2.x, q2.y);
	Gdiplus::PointF p1;
	Gdiplus::PointF p2;
	p1.X = q0.x + 2 / 3. * (q1.x - q0.x);
	p1.Y = q0.y + 2 / 3. * (q1.y - q0.y);
	p2.X = q1.x + 1 / 3. * (q2.x - q1.x);
	p2.Y = q1.y + 1 / 3. * (q2.y - q1.y);
	g.DrawBezier(&pen, p0, p1, p2, p3);
}

void RenderContour(Gdiplus::Graphics& g, const std::vector<FTPoint>& points, int unitsPerEm)
{
	size_t index = 0;

	Gdiplus::SolidBrush sBrush(kLineColor);
	Gdiplus::Pen pen(&sBrush, 1.0f);
	while (index < points.size() - 1) {
		int power = 0;
		if (!GetPointType(points, index, &power)) {
			assert(false);
			break;
		}
		if (power == 1) {
			g.DrawLine(&pen, points[index].GetPointF(unitsPerEm, kGridSize), points[index + 1].GetPointF(unitsPerEm, kGridSize));
			index += 1;
		} else {
			if (g_lineStyle == kLineStyle_nBezier) {
				DrawGenericBezier(g, points.data() + index, power, unitsPerEm);
			} else {
				assert(power == 2);
				DrawQudarticBezier(g, points.data() + index, unitsPerEm);
			} 
			index += power;
		}
	}
	DrawPoints(g, points, unitsPerEm);
}

void RenderGlyph(Gdiplus::Graphics& g, const FTGlyph& glyph)
{
	for (size_t i = 0; i < glyph.contours.size(); ++i) {
		if (glyph.lineDraw || g_lineStyle == kLineStyle_Line)
			RenderContourByLine(g, glyph.contours[i], glyph.unitsPerEm);
		else
			RenderContour(g, glyph.contours[i], glyph.unitsPerEm);
	}
}

VOID OnDraw(Gdiplus::Graphics& g, const SIZE& size)
{
	g.Clear(Gdiplus::Color(0xff, 0x7f, 0x7f, 0x7f));
	std::vector<FTGlyph> glyphs;
	ParseGlyphs("data.xml", &glyphs);

	int countPerRow = size.cx / (kGridSize + kGridMargin);
	int row = 0;
	int col = 0;
	for (size_t i = 0; i < glyphs.size(); ++i) {
		row = i / countPerRow;
		int y = kGridSize * row + kGridMargin * (row + 1);
		col = i % countPerRow;
		int x = kGridSize * col + kGridMargin * (col + 1);
		g.TranslateTransform(x, y);
		RenderGlyph(g, glyphs[i]);

		Gdiplus::SolidBrush sBrush(Gdiplus::Color(0xff, 0xff, 0xff, 0x00));
		Gdiplus::Pen boxPen(&sBrush, 1.0f);
		g.DrawRectangle(&boxPen, 0, 0, kGridSize, kGridSize);
		g.TranslateTransform(-x, -y);
	}
}

void Test(Gdiplus::Graphics& g)
{
	g.Clear(Gdiplus::Color(0xff, 0x7f, 0x7f, 0x7f));
	Gdiplus::SolidBrush sBrush(kLineColor);
	Gdiplus::Pen pen(&sBrush, 1.0f);
	g.DrawLine(&pen, 0, 0, 200, 100);
}

void OnPaint(HWND hwnd)
{
	RECT rc;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &rc);
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcBuffer, hBmp);

	Gdiplus::Graphics g(hdcBuffer);
	g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	g.SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
	g.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	SIZE sz = { rc.right - rc.left, rc.bottom - rc.top };

	OnDraw(g, sz);
	//Test(g);

	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcBuffer, 0, 0, SRCCOPY);
	SelectObject(hdcBuffer, hOldBmp);
	DeleteObject(hBmp);
	DeleteDC(hdcBuffer);
	EndPaint(hwnd, &ps);
}
