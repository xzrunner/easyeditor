#include "LabelLayout.h"

namespace d2d
{

LabelLayout* LabelLayout::m_instance = NULL;

LabelLayout* LabelLayout::Instance()
{
	if (!m_instance)
	{
		m_instance = new LabelLayout();
	}
	return m_instance;
}

LabelLayout::LabelLayout()
{	
}

LabelLayout::~LabelLayout()
{
	std::map<const char*, const Layout*>::iterator itr
		= m_str2layout.begin();
	for ( ; itr != m_str2layout.end(); ++itr) {
		delete itr->second;
	}
}

const LabelLayout::Layout* LabelLayout::Query(const char* text) const
{
	std::map<const char*, const Layout*>::const_iterator itr
		= m_str2layout.find(text);
	if (itr != m_str2layout.end()) {
		return itr->second;
	}
	return NULL;
}

void LabelLayout::Insert(const char* text, const Layout* layout)
{
	std::map<const char*, const Layout*>::iterator itr
		= m_str2layout.find(text);
	if (itr != m_str2layout.end()) {
		delete itr->second;
		itr->second = layout;
	} else {
		m_str2layout.insert(std::make_pair(text, layout));
	}
}

}