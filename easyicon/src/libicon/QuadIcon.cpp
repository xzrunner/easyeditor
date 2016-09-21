#include "QuadIcon.h"

#include <ee/ImageSymbol.h>

namespace eicon
{

QuadIcon::QuadIcon(ee::ImageSymbol* img, const sm::vec2* src, 
				   const sm::vec2* screen)
{
	memcpy(m_src, src, sizeof(m_src));
	memcpy(m_screen, screen, sizeof(m_screen));
	SetImage(img);
}

void QuadIcon::LoadFromFile(const Json::Value& value)
{	
	for (int i = 0; i < 4; ++i) {
		m_src[i].x    = static_cast<float>(value["src"][i]["x"].asDouble());
		m_src[i].y    = static_cast<float>(value["src"][i]["y"].asDouble());
		m_screen[i].x = static_cast<float>(value["screen"][i]["x"].asDouble());
		m_screen[i].y = static_cast<float>(value["screen"][i]["y"].asDouble());
	}
}

void QuadIcon::StoreToFile(Json::Value& value) const
{
	for (int i = 0; i < 4; ++i) {
		value["src"][i]["x"] = m_src[i].x;
		value["src"][i]["y"] = m_src[i].y;
		value["screen"][i]["x"] = m_screen[i].x;
		value["screen"][i]["y"] = m_screen[i].y;
	}
}

}