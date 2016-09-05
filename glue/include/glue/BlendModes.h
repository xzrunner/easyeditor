#ifndef _GLUE_BLEND_MODES_H_
#define _GLUE_BLEND_MODES_H_

#include <CU_Singleton.h>

#include <sprite2/BlendMode.h>

#include <vector>

namespace glue
{

class BlendModes
{
public:
	s2::BlendMode Name2Mode(const std::string& name) const;
	std::string ModeToName(s2::BlendMode mode) const;

private:
	 struct Item
	 {
		Item(s2::BlendMode mode, const std::string& name)
			: mode(mode), name(name) {}

	 	s2::BlendMode mode;
	 	std::string name;
	 };

private:
	std::vector<Item> m_modes;

	SINGLETON_DECLARATION(BlendModes);

}; // BlendModes

}

#endif // _EASYEDITOR_BLEND_MODES_H_