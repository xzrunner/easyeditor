#ifndef _EASYEDITOR_BLEND_MODES_H_
#define _EASYEDITOR_BLEND_MODES_H_

#include <CU_Singleton.h>

#include <sprite2/BlendMode.h>

#include <vector>

namespace ee
{

class BlendModes
{
public:
	s2::BlendMode ID2Mode(int id) const;
	int Mode2ID(s2::BlendMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

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