#ifndef _EASYEDITOR_FAST_BLEND_MODES_H_
#define _EASYEDITOR_FAST_BLEND_MODES_H_

#include <CU_Singleton.h>

#include <sprite2/FastBlendMode.h>

#include <vector>

namespace ee
{

class FastBlendModes
{
public:
	s2::FastBlendMode ID2Mode(int id) const;
	int Mode2ID(s2::FastBlendMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

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

#endif // _EASYEDITOR_FAST_BLEND_MODES_H_