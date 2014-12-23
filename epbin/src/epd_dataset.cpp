#include "epd_dataset.h"
#include "tools.h"

#include <algorithm>

namespace epbin
{

static const int COMPONENT	= 0;
static const int SWITCH		= 1;
static const int LABEL		= 2;
static const int MOUNT		= 3;

static const int FID		= 0;
static const int FCOLOR		= 1;
static const int FMAT		= 2;
static const int FCLIP		= 4;

//////////////////////////////////////////////////////////////////////////
// class String
//////////////////////////////////////////////////////////////////////////

String::String(const std::string& str, bool is_empty)
	: m_str(str)
	, m_is_empty(is_empty)
{
}

size_t String::Size() const
{
	if (m_is_empty) {
		return sizeof(uint8_t);
	} else {
		return sizeof(uint8_t) + m_str.size() + 1;
	}
}

//////////////////////////////////////////////////////////////////////////
// class Picture
//////////////////////////////////////////////////////////////////////////

size_t Picture::Size() const
{
	size_t sz = 0;
	sz += sizeof(uint8_t);					// type
	sz += sizeof(m_id);						// id
	sz += sizeof(uint16_t);					// size
	sz += sizeof(Part) * m_parts.size();	// parts
	return sz;
}

//////////////////////////////////////////////////////////////////////////
// class Component
//////////////////////////////////////////////////////////////////////////

Component::Component()
	: IComponent(COMPONENT)
{
}

size_t Component::Size() const
{
	size_t sz = 0;
	sz += sizeof(m_type);
	sz += sizeof(m_id);
	return sz;
}

//////////////////////////////////////////////////////////////////////////
// class Switch
//////////////////////////////////////////////////////////////////////////

Switch::Switch(const std::string& name)
	: IComponent(SWITCH)
	, m_name(name)
{
}

size_t Switch::Size() const
{
	size_t sz = 0;
	sz += sizeof(m_type);
	sz += sizeof(m_id);
	sz += m_name.Size();
	return sz;
}

//////////////////////////////////////////////////////////////////////////
// class Mount
//////////////////////////////////////////////////////////////////////////

Mount::Mount(const std::string& name)
	: IComponent(MOUNT)
	, m_name(name)
{
}

size_t Mount::Size() const
{
	size_t sz = 0;
	sz += sizeof(m_type);
	sz += m_name.Size();
	return sz;
}

//////////////////////////////////////////////////////////////////////////
// class Label
//////////////////////////////////////////////////////////////////////////

Label::Label(const std::string& name, const std::string& font)
	: IComponent(LABEL)
	, m_name(name)
	, m_font(font)
{
}

size_t Label::Size() const
{
	size_t sz = 0;
	sz += sizeof(m_type);
	sz += m_name.Size();
	sz += m_font.Size();
	sz += sizeof(m_color);
	sz += sizeof(m_size);
	sz += sizeof(m_align);
	sz += sizeof(m_width);
	sz += sizeof(m_height);
	return sz;
}

//////////////////////////////////////////////////////////////////////////
// class Frame
//////////////////////////////////////////////////////////////////////////

Frame::Frame()
	: m_color(NULL)
	, m_add(NULL)
	, m_mat(NULL)
{
}

Frame::~Frame()
{
	delete m_color;
	delete m_add;
	delete m_mat;
}

size_t Frame::Size() const
{
	size_t sz = 0;
	sz += sizeof(m_type);
	sz += sizeof(m_id);
	if (m_color) {
		sz += sizeof(*m_color);
		sz += sizeof(*m_add);
	}
	if (m_mat) {
		sz += sizeof(*m_mat);
	}
	return sz;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

Animation::~Animation()
{
	for_each(m_components.begin(), m_components.end(), DeletePointerFunctor<IComponent>());
	for_each(m_frames.begin(), m_frames.end(), DeletePointerFunctor<Frame>());
}

}