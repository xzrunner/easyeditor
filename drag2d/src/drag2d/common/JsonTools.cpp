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


}