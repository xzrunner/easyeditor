#ifndef _EASYEDITOR_BLEND_MODES_H_
#define _EASYEDITOR_BLEND_MODES_H_

#include <sprite2/BlendMode.h>

#include <vector>

namespace ee
{

class BlendModes
{
public:
	s2::BlendMode GetModeFromNameEN(const std::string& name) const;
	std::string GetNameENFromMode(s2::BlendMode mode) const;

	s2::BlendMode GetIDFromIdx(int idx) const;
	int GetIdxFromID(s2::BlendMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

	static BlendModes* Instance();

private:
	BlendModes();

private:
	 struct Item
	 {
		Item(s2::BlendMode mode, const std::string& name_en, const std::string& name_cn)
			: mode(mode), name_en(name_en), name_cn(name_cn) {}

	 	s2::BlendMode mode;
	 	std::string name_en;
	 	std::string name_cn;
	 };

private:
	std::vector<Item> m_modes;

private:
	static BlendModes* m_instance;

}; // BlendModes

}

#endif // _EASYEDITOR_BLEND_MODES_H_