#ifndef _GLUE_FAST_BLEND_MODES_H_
#define _GLUE_FAST_BLEND_MODES_H_

#include <CU_Singleton.h>

#include <sprite2/FastBlendMode.h>

#include <vector>

namespace glue
{

class FastBlendModes
{
public:
	s2::FastBlendMode Name2Mode(const std::string& name) const;
	std::string Mode2Name(s2::FastBlendMode mode) const;

private:
	 struct Item
	 {
		Item(s2::FastBlendMode mode, const std::string& name)
			: mode(mode), name(name) {}

	 	s2::FastBlendMode mode;
	 	std::string name;
	 };

private:
	std::vector<Item> m_modes;

	SINGLETON_DECLARATION(FastBlendModes);

}; // FastBlendModes


}

#endif // _GLUE_FAST_BLEND_MODES_H_