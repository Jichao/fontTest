#include "stdafx.h"
#include "parse.h"
#include "config.h"

bool ParseContour(const tinyxml2::XMLElement* root, std::vector<FTPoint>* pRes)
{
	auto xmlPt = root->FirstChildElement("pt");
	FTPoint lastPt;
	lastPt.on = true;
	while (xmlPt) {
		FTPoint pt;
		xmlPt->QueryAttribute("x", &pt.x);
		xmlPt->QueryAttribute("y", &pt.y);
		xmlPt->QueryAttribute("on", &pt.on);
		if (g_lineStyle == kLineStyle_2Bezier) {
			if (!lastPt.on && !pt.on) {
				FTPoint midPt;
				midPt.x = (lastPt.x + pt.x) / 2;
				midPt.y = (lastPt.y + pt.y) / 2;
				midPt.on = true;
				pRes->push_back(midPt);
			}
		}
		pRes->push_back(pt);
		lastPt = pt;
		xmlPt = xmlPt->NextSiblingElement("pt");
	}
	if (!pRes->empty()) {
		pRes->push_back(pRes->front());
	}
	return true;
}

bool ParseGlyph(const tinyxml2::XMLElement* root, FTGlyph* pRes)
{
	root->QueryAttribute("xMin", &pRes->xMin);
	root->QueryAttribute("xMax", &pRes->xMax);
	root->QueryAttribute("yMin", &pRes->yMin);
	root->QueryAttribute("yMax", &pRes->yMax);
	root->QueryAttribute("lineDraw", &pRes->lineDraw);
	root->QueryAttribute("unitsPerEm", &pRes->unitsPerEm);
	auto nameAttr = root->FindAttribute("name");
	if (nameAttr) {
		pRes->name = nameAttr->Value();
	}

	auto xmlContour = root->FirstChildElement("contour");
	while (xmlContour) {
		std::vector<FTPoint> contour;
		if (!ParseContour(xmlContour, &contour)) {
			return false;
		}
		pRes->contours.push_back(contour);
		xmlContour = xmlContour->NextSiblingElement("contour");
	}
	return true;
}

bool ParseGlyphs(const std::string& fileName, std::vector<FTGlyph>* pRes) 
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError hr = doc.LoadFile("data.xml");
	if (hr != tinyxml2::XML_SUCCESS)
		return false;

	auto xmlGlyph = doc.RootElement()->FirstChildElement("TTGlyph");
	while (xmlGlyph) {
		FTGlyph glyph;
		if (!ParseGlyph(xmlGlyph, &glyph)) {
			return false;
		}
		pRes->push_back(glyph);
		xmlGlyph = xmlGlyph->NextSiblingElement("TTGlyph");
	}
	return true;
}

