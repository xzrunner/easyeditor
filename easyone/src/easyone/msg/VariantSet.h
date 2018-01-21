#pragma once

#include "msg/Variant.h"

#include <map>

namespace eone
{

class VariantSet
{
public:
	VariantSet() {}

	Variant GetVariant(std::string name) {
		auto& itr = m_variants.find(name);
		return itr != m_variants.end() ? itr->second : Variant();
	}
	void SetVariant(std::string name, Variant& v) {
		m_variants.insert(std::make_pair(name, v));
	}
	void RemoveVariant(std::string name) {
		auto& itr = m_variants.find(name);
		if (itr != m_variants.end()) {
			m_variants.erase(itr);
		}
	}

private:
	std::map<std::string, Variant> m_variants;

}; // VariantSet

}