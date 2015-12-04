#include "Love2dCode.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/DataMgr.h"

namespace eanim
{
namespace lua = ebuilder::lua;

Love2dCode::Love2dCode(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
//	m_tp_adapter.load(m_ctrl->packer);
}

void Love2dCode::Resolve()
{
	if (DataMgr::Instance()->GetLayers().Size() == 0) {
		return;
	}

	lua::TableAssign ta(m_gen, "frames");

	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(0);
	const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr;
	for (itr = frames.begin(); itr != frames.end(); ++itr)
	{
		KeyFrame* frame = itr->second;			
		if (frame->Size() != 0)
		{
			const d2d::ISprite* sprite = frame->GetSprite(0);
			int index = Query(sprite->GetSymbol().GetFilepath());
			if (index != -1)
			{
				std::string time = wxString::FromDouble(frame->GetTime(), 0);

				std::string x = wxString::FromDouble(sprite->GetPosition().x, 1),
					y = wxString::FromDouble(sprite->GetPosition().y, 1);

				std::string left = wxString::FromDouble(m_tp_adapter.textures[index].region.left, 1),
					low = wxString::FromDouble(m_tp_adapter.textures[index].region.low, 1),
					width = wxString::FromDouble(m_tp_adapter.textures[index].region.width, 1),
					height = wxString::FromDouble(m_tp_adapter.textures[index].region.height, 1);

				lua::call(m_gen, "", "Frame", 7, time, x, y, left, low, width, height);
			}
		}
	}
}

int Love2dCode::Query(const std::string& filepath) const
{
	for (size_t i = 0, n = m_tp_adapter.textures.size(); i < n; ++i)
	{
		if (m_tp_adapter.textures[i].filepath == filepath)
			return i;
	}
	for (size_t i = 0, n = m_tp_adapter.textures.size(); i < n; ++i)
	{
		std::string filename = wxFileName(filepath).GetName();
		if (filepath.find(filename) != std::string::npos)
			return i;
	}
	return -1;
}

} // eanim