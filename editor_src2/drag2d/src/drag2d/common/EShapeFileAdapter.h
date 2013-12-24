#pragma once

#include <json/json.h>

#include "IFileAdapter.h"

namespace d2d
{
	class IShape;
	class BezierShape;
	class PolygonShape;
	class ChainShape;
	class RectShape;
	class CircleShape;

	class EShapeFileAdapter : public IFileAdapter
	{
	public:
		EShapeFileAdapter(std::vector<IShape*>& shapes);

		virtual void load(const char* filename);
		virtual void store(const char* filename);

		static IShape* loadShape(const Json::Value& value);
		static Json::Value store(IShape* shape);

	private:
		static IShape* loadBezier(const Json::Value& value);
		static IShape* loadPolygon(const Json::Value& value);
		static IShape* loadChain(const Json::Value& value);
		static IShape* loadRect(const Json::Value& value);
		static IShape* loadCircle(const Json::Value& value);

		static Json::Value store(const BezierShape* bezier);
		static Json::Value store(const PolygonShape* poly);
		static Json::Value store(const ChainShape* chain);
		static Json::Value store(const RectShape* rect);
		static Json::Value store(const CircleShape* circle);

	public:
		std::vector<IShape*>& m_shapes;

	}; // EShapeFileAdapter
}

