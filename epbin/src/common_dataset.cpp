#include "common_dataset.h"
#include "Exception.h"
#include "tools.h"

#include <assert.h>

namespace epbin
{

//////////////////////////////////////////////////////////////////////////
// class String
//////////////////////////////////////////////////////////////////////////

String::String()
	: m_is_empty(true)
{
}

String::String(const std::string& str)
	: m_str(str)
	, m_is_empty(false)
{
}

void String::SetString(const std::string& str)
{
	m_str = str;
	m_is_empty = false;
}

size_t String::Size() const
{
	if (m_is_empty) {
		return sizeof(uint8_t);
	} else {
		if (m_str.size() >= 255) {
			throw Exception("%s is too long", m_str.c_str());
		}
		return sizeof(uint8_t) + m_str.size();
	}
}

void String::Store(uint8_t** ptr) const
{
	if (m_is_empty) {
		uint8_t c = 255;
		pack2mem(c, ptr);
	} else {
		assert(m_str.size() < 255);
		uint8_t sz = m_str.size();
		pack2mem(sz, ptr);
		for (int i = 0; i < sz; ++i) {
			uint8_t c = m_str[i];
			pack2mem(c, ptr);
		}
	}
}

void String::Store(std::ofstream& fout) const
{
	if (m_is_empty) {
		uint8_t c = 255;
		pack2file(c, fout);
	} else {
		assert(m_str.size() < 255);
		uint8_t sz = m_str.size();
		pack2file(sz, fout);
		for (int i = 0; i < sz; ++i) {
			uint8_t c = m_str[i];
			pack2file(c, fout);
		}
	}
}

}