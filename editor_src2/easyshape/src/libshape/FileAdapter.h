
#ifndef LIBSHAPE_FILE_ADAPTER_H
#define LIBSHAPE_FILE_ADAPTER_H

//#include <json/json.h>

#include <drag2d.h>

namespace libshape
{
	class FileAdapter : public d2d::IFileAdapter
	{
	public:
		FileAdapter(std::vector<d2d::IShape*>& shapes);

		virtual void load(const char* filename);
		virtual void store(const char* filename);

		static d2d::IShape* loadShape(const Json::Value& value);
		static Json::Value store(d2d::IShape* shape);

	private:
		static d2d::IShape* loadBezier(const Json::Value& value);
		static d2d::IShape* loadPolygon(const Json::Value& value);
		static d2d::IShape* loadChain(const Json::Value& value);
		static d2d::IShape* loadRect(const Json::Value& value);
		static d2d::IShape* loadCircle(const Json::Value& value);

		static Json::Value store(const d2d::BezierShape* bezier);
		static Json::Value store(const d2d::PolygonShape* poly);
		static Json::Value store(const d2d::ChainShape* chain);
		static Json::Value store(const d2d::RectShape* rect);
		static Json::Value store(const d2d::CircleShape* circle);

	public:
		std::vector<d2d::IShape*>& m_shapes;

	}; // FileAdapter
}

#endif // LIBSHAPE_FILE_ADAPTER_H