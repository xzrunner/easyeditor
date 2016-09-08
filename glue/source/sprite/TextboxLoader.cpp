#include "TextboxLoader.h"
#include "trans_color.h"

#include <sprite2/Textbox.h>

#include <fstream>

namespace glue
{

TextboxLoader::TextboxLoader(s2::Textbox& m_tb)
	: m_tb(m_tb)
{
}

void TextboxLoader::LoadJson(const Json::Value& val)
{
	m_tb.width				= val["width"].asInt();
	m_tb.height				= val["height"].asInt();

	m_tb.font_type			= val["font"].asInt();
	m_tb.font_size			= val["font_size"].asInt();
	m_tb.font_color			= str2color(val["font_color"].asString(), PT_RGBA);

	m_tb.has_edge			= val["edge"].asBool();
	m_tb.edge_size			= static_cast<float>(val["edge_size"].asDouble());
	m_tb.edge_color			= str2color(val["edge_color"].asString(), PT_RGBA);

	m_tb.space_hori			= static_cast<float>(val["space_hori"].asDouble());
	m_tb.space_vert			= static_cast<float>(val["space_vert"].asDouble());

	m_tb.align_hori			= (s2::Textbox::HoriAlign)(val["align_hori"].asInt());
	m_tb.align_vert			= (s2::Textbox::VertAlign)(val["align_vert"].asInt());

	m_tb.overflow			= true;
	if (!val["overflow"].isNull()) {
		m_tb.overflow		= val["overflow"].asBool();
	}

	m_tb.richtext			= true;
	if (!val["richtext"].isNull()) {
		m_tb.richtext		= val["richtext"].asBool();
	}
}

}