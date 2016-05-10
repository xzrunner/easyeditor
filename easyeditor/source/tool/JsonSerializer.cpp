#include "JsonSerializer.h"
#include "Rect.h"

namespace ee
{

void JsonSerializer::Store(const std::vector<sm::vec2>& points, Json::Value& value)
{
	for (int i = 0, n = points.size(); i < n; ++i)
	{
		value["x"][i] = points[i].x;
		value["y"][i] = points[i].y;
	}
}

void JsonSerializer::Load(const Json::Value& value, std::vector<sm::vec2>& points)
{
	if (value.isArray())
	{
		int sz = value.size() / 2;
		points.reserve(sz);
		int i = 0;
		Json::Value xval = value[i++],
			        yval = value[i++];
		while (!xval.isNull() && !yval.isNull()) {
			sm::vec2 p;
			p.x = static_cast<float>(xval.asDouble());
			p.y = static_cast<float>(yval.asDouble());
			points.push_back(p);
			xval = value[i++];
			yval = value[i++];
		}
	}
	else
	{
		points.reserve(value["x"].size());

		int i = 0;
		Json::Value xval = value["x"][i],
			yval = value["y"][i];
		++i;
		while (!xval.isNull() && !yval.isNull()) {
			sm::vec2 p;
			p.x = static_cast<float>(xval.asDouble());
			p.y = static_cast<float>(yval.asDouble());
			points.push_back(p);
			xval = value["x"][i];
			yval = value["y"][i];
			++i;
		}	
	}
}

void JsonSerializer::Store(const std::vector<s2::Color>& colors, Json::Value& value)
{
	for (int i = 0, n = colors.size(); i < n; ++i)
	{
		value["r"][i] = colors[i].r;
		value["g"][i] = colors[i].g;
		value["b"][i] = colors[i].b;
		value["a"][i] = colors[i].a;
	}
}

void JsonSerializer::Load(const Json::Value& value, std::vector<s2::Color>& colors)
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
		s2::Color col;
		col.r = static_cast<int>(rval.asDouble() * 255);
		col.g = static_cast<int>(gval.asDouble() * 255);
		col.b = static_cast<int>(bval.asDouble() * 255);
		col.a = static_cast<int>(aval.asDouble() * 255);
		colors.push_back(col);

		rval = value["r"][i];
		gval = value["g"][i];
		bval = value["b"][i];
		aval = value["a"][i];

		++i;
	}
}

void JsonSerializer::Store(const Rect& r, Json::Value& value)
{
	value["xmin"] = r.xmin;
	value["xmax"] = r.xmax;
	value["ymin"] = r.ymin;
	value["ymax"] = r.ymax;
}

void JsonSerializer::Load(const Json::Value& value, Rect& r)
{
	r.xmin = static_cast<float>(value["xmin"].asDouble());
	r.xmax = static_cast<float>(value["xmax"].asDouble());
	r.ymin = static_cast<float>(value["ymin"].asDouble());
	r.ymax = static_cast<float>(value["ymax"].asDouble());
}

void JsonSerializer::Store(const s2::Color& col, Json::Value& value)
{
	value["r"] = col.r;
	value["g"] = col.g;
	value["b"] = col.b;
	value["a"] = col.a;
}

void JsonSerializer::Load(const Json::Value& value, s2::Color& col)
{
	col.r = static_cast<int>(value["r"].asDouble() * 255);
	col.g = static_cast<int>(value["g"].asDouble() * 255);
	col.b = static_cast<int>(value["b"].asDouble() * 255);
	col.a = static_cast<int>(value["a"].asDouble() * 255);
}

}