#include "FilterModes.h"
#include "ShaderMgr.h"

#include "EdgeDetectionShader.h"
#include "ReliefShader.h"
#include "OutlineShader.h"
#include "BlurShader.h"

namespace ee
{

FilterModes* FilterModes::m_instance = NULL;

FilterModes::FilterModes()
{
// 	InitAllShaders();
// 
// 	m_modes.push_back(Item(FM_NORMAL, "normal", "正常"));
// 	m_modes.push_back(Item(FM_EDGE_DETECTION, "edge detection", "边界检测"));
// 	m_modes.push_back(Item(FM_RELIEF, "relief", "浮雕"));
// 	m_modes.push_back(Item(FM_OUTLINE, "outline", "包围"));
// 	m_modes.push_back(Item(FM_BLUR, "blur", "模糊"));
}

FilterMode FilterModes::GetIDFromNameEN(const std::string& name) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name_en == name) {
			return item.id;
		}
	}
	return FM_UNKNOWN;
}

std::string FilterModes::GetNameENFromID(FilterMode id) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.id == id) {
			return item.name_en;
		}
	}
	return "";
}

FilterMode FilterModes::GetIDFromIdx(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return FM_UNKNOWN;
	} else {
		return m_modes[idx].id;
	}
}

int FilterModes::GetIdxFromID(FilterMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].id == mode) {
			return i;
		}
	}
	return 0;
}

void FilterModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name_cn);
	}
}

int FilterModes::QueryShaderIdx(FilterMode mode) const
{
	std::map<FilterMode, int>::const_iterator itr = 
		m_map2idx.find(mode);
	if (itr == m_map2idx.end()) {
		return 0;
	} else {
		return itr->second;
	}
}

void FilterModes::InitAllShaders()
{
	m_map2idx.insert(std::make_pair(FM_NORMAL, 0));

	ShaderMgr* mgr = ShaderMgr::Instance();
	{
		EdgeDetectionShader* shader = new EdgeDetectionShader;
		shader->Load();
		int idx = mgr->AddSpriteShader(shader);
		m_map2idx.insert(std::make_pair(FM_EDGE_DETECTION, idx));
	}
	{
		ReliefShader* shader = new ReliefShader;
		shader->Load();
		int idx = mgr->AddSpriteShader(shader);
		m_map2idx.insert(std::make_pair(FM_RELIEF, idx));
	}
	{
		OutlineShader* shader = new OutlineShader;
		shader->Load();
		int idx = mgr->AddSpriteShader(shader);
		m_map2idx.insert(std::make_pair(FM_OUTLINE, idx));
	}
	{
		BlurShader* shader = new BlurShader;
		shader->Load();
		int idx = mgr->AddSpriteShader(shader);
		m_map2idx.insert(std::make_pair(FM_BLUR, idx));
	}
	mgr->null();
}

FilterModes* FilterModes::Instance()
{
	if (!m_instance) {
		m_instance = new FilterModes();
	}
	return m_instance;
}

}