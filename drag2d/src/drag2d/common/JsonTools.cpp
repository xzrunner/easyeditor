#include "JsonTools.h"

namespace d2d
{

void JsonTools::store(const std::vector<Vector>& points, Json::Value& value)
{
	for (int i = 0, n = points.size(); i < n; ++i)
	{
		value["x"][i] = points[i].x;
		value["y"][i] = points[i].y;
	}
}

void JsonTools::load(const Json::Value& value, std::vector<Vector>& points)
{
	points.reserve(value["x"].size());

	int i = 0;
	Json::Value xval = value["x"][i],
		yval = value["y"][i];
	++i;
	while (!xval.isNull() && !yval.isNull()) {
		d2d::Vector p;
		p.x = xval.asDouble();
		p.y = yval.asDouble();
		points.push_back(p);
		xval = value["x"][i];
		yval = value["y"][i];
		++i;
	}
}

void JsonTools::store(const std::vector<Colorf>& colors, Json::Value& value)
{
	for (int i = 0, n = colors.size(); i < n; ++i)
	{
		value["r"][i] = colors[i].r;
		value["g"][i] = colors[i].g;
		value["b"][i] = colors[i].b;
		value["a"][i] = colors[i].a;
	}
}

void JsonTools::load(const Json::Value& value, std::vector<Colorf>& colors)
{
	colors.reserve(value["r"].size());

	int i = 0;
	Json::Value 
		rval = value["r"][i],
		gval = value["g"][i],
		bval = value["b"][i],
		aval = value["a"][i];

	++i;
	while (!rval.isNull() && !gval.isNull() && !bval.isNull() && !aval.isNull()) {
		Colorf col;
		col.r = rval.asDouble();
		col.g = gval.asDouble();
		col.b = bval.asDouble();
		col.a = aval.asDouble();
		colors.push_back(col);

		rval = value["r"][i];
		gval = value["g"][i];
		bval = value["b"][i];
		aval = value["a"][i];

		++i;
	}
}

void JsonTools::Store(const d2d::Rect& r, Json::Value& value)
{
	value["xmin"] = r.xMin;
	value["xmax"] = r.xMax;
	value["ymin"] = r.yMin;
	value["ymax"] = r.yMax;
}

void JsonTools::Load(const Json::Value& value, d2d::Rect& r)
{
	r.xMin = value["xmin"].asDouble();
	r.xMax = value["xmax"].asDouble();
	r.yMin = value["ymin"].asDouble();
	r.yMax = value["ymax"].asDouble();
}

}