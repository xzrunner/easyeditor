#pragma once

#include <sprite2/Component.h>

#include <string>
#include <set>

namespace ee0
{

class CompSymEditor : public s2::Component
{
public:
//	CompSymEditor() : m_dirty(false) {}
	
	virtual CompSymEditor* Clone() const;

	const std::string& GetFilepath() const { return m_filepath; }

private:
	std::string m_name;
	//std::string m_tag;

	std::string           m_filepath;
	//std::set<std::string> m_filepaths;

	//bool m_dirty;

}; // CompSymEditor

}