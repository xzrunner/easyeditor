#ifndef _EASYEDITOR_FAST_BLEND_MODES_H_
#define _EASYEDITOR_FAST_BLEND_MODES_H_

#include <cu/cu_macro.h>

#include <painting2/FastBlendMode.h>

#include <vector>

namespace ee
{

class FastBlendModes
{
public:
	pt2::FastBlendMode ID2Mode(int id) const;
	int Mode2ID(pt2::FastBlendMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

private:
	 struct Item
	 {
		Item(pt2::FastBlendMode mode, const std::string& name)
			: mode(mode), name(name) {}

	 	pt2::FastBlendMode mode;
	 	std::string name;
	 };

private:
	std::vector<Item> m_modes;

	CU_SINGLETON_DECLARATION(FastBlendModes);

}; // FastBlendModes


}

#endif // _EASYEDITOR_FAST_BLEND_MODES_H_