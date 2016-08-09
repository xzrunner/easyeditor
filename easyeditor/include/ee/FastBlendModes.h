#ifndef _EASYEDITOR_FAST_BLEND_MODES_H_
#define _EASYEDITOR_FAST_BLEND_MODES_H_

#include <sprite2/FastBlendMode.h>

#include <vector>

namespace ee
{

class FastBlendModes
{
public:
	s2::FastBlendMode GetModeFromNameEN(const std::string& name) const;
	std::string GetNameENFromMode(s2::FastBlendMode mode) const;

	s2::FastBlendMode GetIDFromIdx(int idx) const;
	int GetIdxFromID(s2::FastBlendMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

	static FastBlendModes* Instance();

private:
	FastBlendModes();

private:
	 struct Item
	 {
		Item(s2::FastBlendMode mode, const std::string& name_en, const std::string& name_cn)
			: mode(mode), name_en(name_en), name_cn(name_cn) {}

	 	s2::FastBlendMode mode;
	 	std::string name_en;
	 	std::string name_cn;
	 };

private:
	std::vector<Item> m_modes;

private:
	static FastBlendModes* m_instance;

}; // FastBlendModes


}

#endif // _EASYEDITOR_FAST_BLEND_MODES_H_