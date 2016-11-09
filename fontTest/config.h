#pragma once

enum FillMode {
	kFillMode_OnlyContour,
	kFillMode_Fill,
};

enum DrawEngine {
	kDrawEngine_GDI,
	kDrawEngine_GDIPlus
};

enum LineStyle {
	kLineStyle_Line,
	kLineStyle_2Bezier,
	kLineStyle_nBezier
};

extern int g_fillMode;
extern int g_lineStyle;
extern int g_drawEngine;