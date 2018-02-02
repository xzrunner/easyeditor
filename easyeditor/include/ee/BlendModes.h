#ifndef _EASYEDITOR_BLEND_MODES_H_
#define _EASYEDITOR_BLEND_MODES_H_

#include <cu/cu_macro.h>

#include <painting2/BlendMode.h>

#include <vector>

namespace ee
{

class BlendModes
{
public:
	pt2::BlendMode ID2Mode(int id) const;
	int Mode2ID(pt2::BlendMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

private:
	 struct Item
	 {
		Item(pt2::BlendMode mode, const std::string& name)
			: mode(mode), name(name) {}

	 	pt2::BlendMode mode;
	 	std::string name;
	 };

private:
	std::vector<Item> m_modes;

	CU_SINGLETON_DECLARATION(BlendModes);

}; // BlendModes

}

#endif // _EASYEDITOR_BLEND_MODES_H_