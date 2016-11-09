#pragma once

enum FillStyle {
	kFillStyle_OnlyContour,
	kFillStyle_Fill,
	kFillStyle_SuckFill,
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

extern int g_fillStyle;
extern int g_lineStyle;
extern int g_drawEngine;